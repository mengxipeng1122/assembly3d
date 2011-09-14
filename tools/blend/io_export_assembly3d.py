# Copyright (c) 2011 Michael Nischt
# All rights reserved.

# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:

# Redistributions of source code must retain the above copyright notice,
# this list of conditions and the following disclaimer.

# Redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution.

# Neither the name of the project's author nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.

# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
# TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


bl_info = {
    "name": "Assembly3D Exporter (.scene, .mesh, .anim)",
    "author": "Michael Nischt",
    "version": (0,0,1),
    "blender": (2,5,7),
    "location": "File > Export...",
    "description": "Export to Assembly3D xml and binary formats",
    "warning": "",
#    "wiki_url": "http://wiki.blender.org/index.php/Extensions:2.5/Py/"\
#        "Scripts/File_I-O/io_export_assembly3d",
#    "tracker_url": "http://www.interaction3d.org",
    "category": "Import-Export"}

# To support reload properly, try to access a package var, if it's there, reload everything (F8)
if "bpy" in locals():
    print("pre")
    import imp
    if "io_export_assembly3d" in locals():
        print("reload")
        imp.reload(io_export_assembly3d)

import bpy,mathutils, array,struct,math,os,time,sys,itertools

#exporter settings
class Settings:
  def __init__(self,
               savepath,
               exportMode,
               ):
    self.savepath = savepath
    self.exportMode = exportMode



def isZero(a, eps=0.0001):
  return abs(a) < eps

def isOne(a, eps=0.0001):
  return abs(a - 1) < eps



def save(settings):
	print( "SAVE")
	
	if settings and not os.path.exists(settings.savepath):
		os.makedirs(settings.savepath)

	save_test(settings)
	save_scene(settings)
	save_meshes(settings)
	save_anim(settings)


def save_test(settings):	

	objects = []

	for obj in bpy.context.scene.objects:
		
		if ((obj.type == 'MESH') and ( len(obj.data.vertices.values()) > 0 )):
			objects.append(obj)


	armatures = set([])

	for obj in objects:

		print(obj.name)
		if obj.parent:
			print("\t" + obj.parent_type)
			if obj.parent_type == 'BONE':
				print("\t" + obj.parent_bone)
				armatures.add(obj.parent)
			elif obj.parent_type == 'ARMATURE':
				print("\t" + obj.parent_bone)
				armatures.add(obj.parent)
			elif obj.parent_type == 'OBJECT' and obj.parent.type == 'ARMATURE':
				print("\t" + obj.parent.name)
				armatures.add(obj.parent)

	for obj in armatures:
		save_armature(obj)



def save_armature(armature):
	for bone in armature.data.bones:
		print(bone.name)
#		if(bone.parent):
#			print(bone.parent_index(bone.parent))




def save_scene(settings):	

	objects = []

	for obj in bpy.context.selected_objects:
		
		if ((obj.type != 'MESH') or ( len(obj.data.vertices.values()) == 0 )):
			continue
		
		print(obj.name)
		if obj.parent:
			print("\t" + obj.parent_type)
			if obj.parent_type == 'BONE':
				print("\t" + obj.parent_bone)
		
		objects.append(obj)



	file_xml = open(settings.savepath + ".world.xml", 'w')
	file_xml.write( '<?xml version="1.0" encoding="UTF-8"?>\n' )
	file_xml.write( '<Scene xmlns="http://assembly.interaction3d.org/scene" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://assembly.interaction3d.org/scene http://assembly.interaction3d.org/scene.xsd">\n' )
	file_xml.write( '\t<World objects="%d">\n' % (len(objects)) )

	for obj in objects:
		translation, rotation, scale = obj.matrix_world.decompose()
		if scale[0] != 1 and scale[0] == scale[1] and scale[1] == scale[2]:
			file_xml.write( '\t\t<Object name="%s" scale="%f">\n' % (obj.name, scale[0]) )
		else:
			file_xml.write( '\t\t<Object name="%s">\n' % (obj.name) )
			
		file_xml.write( '\t\t\t<Position x="%f" y="%f" z="%f"/>\n' % (translation[0], translation[1], translation[2]) )
#		file_xml.write( '\t\t\t<Orientation x="%f" y="%f" z="%f" w="%f"/>\n' % (rotation[1], rotation[2], rotation[3], rotation[0]) )
		file_xml.write( '\t\t\t<Orientation x="%f" y="%f" z="%f"/>\n' % (rotation[1], rotation[2], rotation[3]) )
		file_xml.write( '\t\t</Object>\n' )
		
	file_xml.write( '\t</World>\n' )
	file_xml.write( '</Scene>\n' )
	file_xml.close();

def save_meshes(settings):

	for obj in bpy.context.selected_objects:
		
		if ((obj.type != 'MESH') or ( len(obj.data.vertices.values()) == 0 )):
			continue

		save_mesh(obj, settings.savepath)



def save_mesh(obj, savepath):

	class Face:
		def __init__(self, a, b, c):
			self.a = a
			self.b = b
			self.c = c

	class Vertex:
	
		def __init__(self, position, normal, texcoord = mathutils.Vector((0,0)), weights = []):
			self.position = position
			self.normal = normal
			self.texcoord = texcoord
			self.weights = weights
			self.split_normals = []
			self.split_texcoords = []

	mesh = obj.data

	matrix_world = obj.matrix_world
	matrix_normal = matrix_world.copy();
	matrix_normal.invert();
	matrix_normal.transpose();
	
	#shape keys
#		if mesh.shape_keys:
#			print(mesh.shape_keys)

	has_uv = len(mesh.uv_textures) > 0
	has_materials = len(obj.material_slots) > 0

	vertices = [None] * len(mesh.vertices)
	split_normals = []
	split_texcoords = []

	face_groups = []
	for i in range(max(1, len(obj.material_slots))):
		face_groups.append( [] )

	for face_index, face in enumerate(mesh.faces):

		if len(face.vertices) < 3:
			print("warning: face has less than 3 vertices")
			continue

		faces = face_groups[face.material_index]
		face_vertices = []

#			normal = None
#			if not face.use_smooth:
#				normal = face.normal
#				print(normal)
#			print(face.edge_keys[0])

		for i in range(len(face.vertices)):
			v = face.vertices[i]
			
			position = mesh.vertices[v].co
			normal = None
			texcoord = None
			
			if face.use_smooth:
				normal = mesh.vertices[v].normal
			else:
				normal= face.normal
			
			if has_uv:
				uv = mesh.uv_textures.active.data[face_index].uv[i]
				texcoord = mathutils.Vector((uv[0], uv[1]))
			else:
				texcoord = mathutils.Vector((0, 0))
			
			#print("V: p(%s) n(%s) t(%s)" % (str(position),str(normal),str(texcoord)) )
			vertex = Vertex(position, normal, texcoord)
			
			if not vertices[v]:
				vertices[v] = vertex
				face_vertices.append( (v, 0) )
				continue
			
			
			if not face.use_smooth:
				face_vertices.append( (len(split_normals), 1) ) 
				#vertices[v].split_normals.append ( vertex )
				split_normals.append( vertex )
				continue
			
			if (texcoord.x != vertices[v].texcoord.x) or (texcoord.y != vertices[v].texcoord.y):
				split = True
				for clone_index, clone in enumerate(split_texcoords):
					uv = clone.texcoord;
					if (texcoord.x == clone.texcoord.x) and (texcoord.y == clone.texcoord.y) and (normal.x == clone.normal.x) and (normal.y == clone.normal.y) and (normal.z == clone.normal.z) and (position.x == clone.position.x) and (position.y == clone.position.y) and (position.z == clone.position.z):
						split = False
						face_vertices.append( (clone_index, 2) )
						break
#					for clone in vertices[v].split_texcoords:
#						uv = clone.texcoord;
#						if (texcoord.x == uv.x) and (texcoord.y == uv.y):
#							split = False
#							break
				
				if split:
					face_vertices.append( (len(split_texcoords), 2) ) 
					#vertices[v].split_texcoords.append ( vertex )
					split_texcoords.append( vertex )
					continue
				
			else:
				face_vertices.append( (v, 0) )
			
		assert len(face.vertices) == len(face_vertices)
		
		for i in range(1, len(face.vertices) - 1):
			faces.append ( Face(face_vertices[0], face_vertices[i], face_vertices[i+1]) )
		
	print("split v(%s) n(%s) t(%s)" % (str(len(vertices)), str(len(split_normals)), str(len(split_texcoords))) )
	print("groups: " + str(len(face_groups)) )
	for faces in face_groups:
		print("\tgroup: " + str(len(faces)) )

	totalVertices = len(vertices) + len(split_normals) + len(split_texcoords)
	print("total: " + str(totalVertices))
	
	positions = array.array('f', [0.0] * 3 * totalVertices)
	normals = array.array('f', [0.0] * 3 * totalVertices)
	texcoords = array.array('f', [0.0] * 2 * totalVertices)

	s_vec3 = struct.Struct('<fff')
	s_vec2 = struct.Struct('<ff')
	index = 0
	for vertex in list(itertools.chain(*[vertices, split_normals, split_texcoords])):
		#print(str(vertex.texcoord))
		s_vec3.pack_into(positions, s_vec3.size*index, *vertex.position.xyz)
		s_vec3.pack_into(normals, s_vec3.size*index, *vertex.normal.xyz)
		s_vec2.pack_into(texcoords, s_vec2.size*index, *vertex.texcoord.xy)
		index += 1
	
	totalFaces = 0
	for faces in face_groups:
		totalFaces += len(faces)
	
	accum = [ 0, len(vertices), len(vertices)+len(split_normals) ]
	faces = array.array('d', [0] * 3 * totalFaces)

	s_int = 'I'
	index_type = "UNSIGNED_INT"
	if totalVertices < 256:
		s_int = 'B'
		index_type = "UNSIGNED_BYTE"
	elif totalVertices < 65536:
		s_int = 'H'
		index_type = "UNSIGNED_SHORT"

	s_int3 = struct.Struct('<' + s_int + s_int + s_int)
	
	faces = array.array(s_int, [0] * 3 * totalFaces)
	
	index = 0
	for face in list(itertools.chain(*face_groups)):
		a = face.a[0]+accum[face.a[1]]
		b = face.b[0]+accum[face.b[1]]
		c = face.c[0]+accum[face.c[1]]
		#print("%d %d %d" % (a , b, c ) )
		s_int3.pack_into(faces, s_int3.size*index, *[a,b,c])
		index += 1

	if not savepath:
		return

	file_dat = open(savepath + os.sep + obj.name + ".mesh.dat", 'wb')
	positions.tofile(file_dat)
	normals.tofile(file_dat)
	if has_uv:
		texcoords.tofile(file_dat)
	faces.tofile(file_dat)
	file_dat.close();
	
	file_xml = open(savepath + os.sep + obj.name + ".mesh.xml", 'w')
	file_xml.write( '<?xml version="1.0" encoding="UTF-8"?>\n' )
	file_xml.write( '<Mesh xmlns="http://assembly.interaction3d.org/mesh" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://assembly.interaction3d.org/mesh http://assembly.interaction3d.org/mesh.xsd">\n' )	  
	totalAttributes = 2
	if has_uv:
		totalAttributes += 1
	file_xml.write( '\t<Vertices count="%d" attributes="%d">\n' %  (totalVertices, totalAttributes) )

	file_xml.write( '\t\t<Attribute name="%s" size="%d" type="FLOAT"/>\n' %  ("POSITION", 3) )
	file_xml.write( '\t\t<Attribute name="%s" size="%d" type="FLOAT"/>\n' %  ("NORMAL", 3) )
	if has_uv:
		file_xml.write( '\t\t<Attribute name="%s" size="%d" type="FLOAT"/>\n' %  ("TEXCOORD", 2) )
		
	file_xml.write( '\t</Vertices>\n' )

	file_xml.write( '\t<Triangles type="%s" groups="%d">\n' %  (index_type, len(face_groups)) )
	if not has_materials:
		file_xml.write( '\t<Group name="%s" count="%d"/>\n' %  ("default", totalFaces) )
	else:
		for i in range(len(face_groups)):
			file_xml.write( '\t\t<Group name="%s" count="%d"/>\n' %  (obj.material_slots[i].material.name, len(face_groups[i])) )

	file_xml.write( '\t</Triangles>\n' )
	file_xml.write( '</Mesh>\n' )
	file_xml.close();



def save_anim(settings):
	
	frame_start = bpy.context.scene.frame_start
	frame_end = bpy.context.scene.frame_end
	frame_count = (frame_end - frame_start)+1
	frame_current = bpy.context.scene.frame_current
	frame_duration = 1.0 / bpy.context.scene.render.fps
	
	channels = []
	
  
	for obj in bpy.context.selected_objects:
		
		if not obj.animation_data or not (obj.animation_data.action or obj.animation_data.use_nla):
			print( "no action or nla:" + obj.name )
			continue		
		
		
		anim_name = obj.name
		if obj.animation_data.action:
			anim_name = obj.animation_data.action.name
		
		useTranslation = False
		useRotation = False
		useScale = False
		
		s_vec3 = struct.Struct('<fff')
		T = array.array('f', [0.0] * 3 * frame_count)
		R = array.array('f', [0.0] * 3 * frame_count)
		S = array.array('f', [0.0] * 3 * frame_count)
		
		frame = frame_start
		while frame <= frame_end: 
  		#time = frame * frame_duration
			#print( "Time: " + str(time))
			bpy.context.scene.frame_set(frame)
			translation, rotation, scale = obj.matrix_world.decompose()

			useTranslation = useTranslation or not (isZero(translation[0]) and isZero(translation[1]) and isZero(translation[2]))
			useRotation = useRotation or not (isOne(rotation[0]) and isZero(rotation[1]) and isZero(rotation[2]) and isZero(rotation[3]))
			useScale = useScale or not (isOne(scale[0]) and isOne(scale[1]) and isOne(scale[2]))
			offset = (frame - frame_start)*s_vec3.size; 
			s_vec3.pack_into(T, offset, *translation)
			s_vec3.pack_into(R, offset, *rotation[1:4])
			if useScale:
				s_vec3.pack_into(S, offset, *scale)
			frame += 1
		
		print("l(" + str(useTranslation) + "),r(" + str(useRotation) + "),s(" + str(useScale) + ")")
		print("T: " + str(len(T)))
		print("R: " + str(len(R)))
		
		if not (useTranslation or useRotation or useScale):
			continue
		
		file_dat = open(settings.savepath + os.sep + anim_name + ".anim.dat", 'wb')
		
		num_attributes = 0;
		if useTranslation:
			T.tofile(file_dat)
			num_attributes += 1
		if useRotation:
			R.tofile(file_dat)
			num_attributes += 1
		if useScale:
			S.tofile(file_dat)
			num_attributes += 1
		
		file_dat.close();
		

		file_xml = open(settings.savepath + os.sep + anim_name + ".anim.xml", 'w')
		file_xml.write( '<?xml version="1.0" encoding="UTF-8"?>\n' )
		file_xml.write( '<Animation xmlns="http://assembly.interaction3d.org/anim" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://assembly.interaction3d.org/anim http://assembly.interaction3d.org/anim.xsd">\n' )	  
		file_xml.write( '\t<Sampler duration="%f" channels="%d">\n' %  (frame_count * frame_duration, 1) )
		file_xml.write( '\t\t<Channel name="%s" keyframes="%d" attributes="%d">\n' %  (obj.name, frame_count, num_attributes) )

		if useTranslation:
			file_xml.write( '\t\t\t<Attribute name="%s" size="%d" type="FLOAT"/>\n' %  ("POSITION", 3) )
		if useRotation:
			file_xml.write( '\t\t\t<Attribute name="%s" size="%d" type="FLOAT"/>\n' %  ("ORIENTATION", 3) )
		if useScale:
			file_xml.write( '\t\t\t<Attribute name="%s" size="%d" type="FLOAT"/>\n' %  ("SCALING", 3) )

		file_xml.write( '\t\t</Channel>\n' )

		file_xml.write( '\t</Sampler>\n' )
		file_xml.write( '</Animation>\n' )
		file_xml.close();
	
	bpy.context.scene.frame_set(frame_current)


##########
#export class registration and interface
from bpy.props import *
from bpy_extras.io_utils import (ExportHelper, path_reference_mode, axis_conversion)
                                 
class ExportAssembly3D(bpy.types.Operator):#, ExportHelper):
  '''Export to Assembly3D formats (.xml)'''
  bl_idname = "assembly.3d"
  bl_label = 'Export Assembly3D'
             
  exportModes = [("mesh", "Mesh", "Export .mesh.xml."),
                 ("anim", "Animation", "Export .anim.xml.")]

  filepath = StringProperty(subtype='DIR_PATH',name="Directory Path", description="Directory for exporting", maxlen= 1024, default= os.environ['HOME'])
  exportList = EnumProperty(name="Export", items=exportModes, description="Choose export mode.", default='anim')

  def execute(self, context):
   settings = Settings(savepath = self.properties.filepath,
                          exportMode = self.properties.exportList
                          )
   save(settings)
   return {'FINISHED'}

  def invoke(self, context, event):
        WindowManager = context.window_manager
        WindowManager.fileselect_add(self)
        return {"RUNNING_MODAL"}  

def menu_func(self, context):
#  default_path = os.path.splitext(bpy.data.filepath)[0]
#  self.layout.operator(ExportAssembly3D.bl_idname, text="Assembly3D", icon='BLENDER').filepath = default_path
#  default_path = os.path.splitext(bpy.data.filepath)[0]
  self.layout.operator(ExportAssembly3D.bl_idname, text="Assembly3D")
  
def register():
  bpy.utils.register_module(__name__)
  bpy.types.INFO_MT_file_export.append(menu_func)

def unregister():
  bpy.utils.unregister_module(__name__)
  bpy.types.INFO_MT_file_export.remove(menu_func)

if __name__ == "__main__":
	register()
