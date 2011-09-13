/*
 * Copyright (c) 2011 Peter Vasil
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the project's author nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "Utils.h"
#include <math.h>

#ifdef TARGET_WIN32
#include <direct.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>

#include "config.h"
#ifdef A3D_FREEIMAGE
static const int IMAGE_FORMAT_COUNT = 44;
static const char * const IMAGE_FORMATS[] = {"bmp","dds","exr","g3","gif",
                                             "hdr","ico","iff","lbm","j2k",
                                             "j2c","jng","jp2","jpg","jif",
                                             "jpeg","jpe","koa","mng","pbm",
                                             "pbm","pcd","pcx","pfm","pgm",
                                             "pgm","pct","pict","pic","png",
                                             "ppm","ppm","psd","ras", "sgi",
                                             "tga","targa","tif","tiff","wap",
                                             "wbmp","wbm","xbm","xpm"};
#else
static const int IMAGE_FORMAT_COUNT = 1;
static const char * const IMAGE_FORMATS[] = {"tga"};
#endif

bool Utils::checkIfFileExists(const char* path)
{
    struct stat fileInfo;
    if (stat(path, &fileInfo) != 0) {  // Use stat( ) to get the info
        //      std::cerr << "Error: " << strerror(errno) << '\n';
        return false;
    }
    if ((fileInfo.st_mode & S_IFMT) == S_IFDIR)
        return false;
    else
        return true;
}

std::string Utils::getTextureImagePathWithExt(const char* path)
{
    using namespace std;
    string result;
    for(int i = 0; i < IMAGE_FORMAT_COUNT; ++i)
    {
        string pathWithExt = path;
        pathWithExt.append(".");
        pathWithExt.append(IMAGE_FORMATS[i]);
        if(checkIfFileExists(pathWithExt.c_str()) == true)
        {
            result = pathWithExt;
            break;
        }
    }
    return result;
}

float Utils::calculateQuaternionW(float x, float y, float z)
{
    // http://tfc.duke.free.fr/coding/md5-specs-en.html
    float t = 1.0f - (x * x) - (y * y) - (z * z);
    if (t < 0.0f)
        return 0.0f;
    else
        return -sqrt(t);
}
