/** @file KDTree.h
 * Generic KDTree, 
 * supports balanced KDTree generation and range queries.
 * @author: Scott Kircher
 * @date: November 22, 2004
 */

#ifndef __KDTREE_H__
#define __KDTREE_H__

#include <vector>
#include <algorithm>
#include <cfloat>

////KDTree class

#include <fstream>
#include <cmath>

namespace assembly3d
{

    using std::ostream;
    using std::istream;

    class Point
    {
    public:

        // uninitialized Point
//        Point() {x = y = z = 0.0f;}

        Point(float x=0.0f, float y=0.0f, float z=0.0f):
            x(x), y(y), z(z) {}

        ~Point(void) {}

        float lengthSquared()
        {
            return x*x + y*y + z*z;
        }

        float operator[]( int n ) const {
            switch(n)
            {
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            default:
                return x;
            }
        }

        float x, y, z;
        int triangle[3];

    };

    Point operator+(const Point& p1, const Point& p2) {
        return Point(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
    }
    Point operator-(const Point& p1, const Point& p2) {
        return Point(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
    }

//    Point operator+(Point p0, const Point p1);
//    Point operator-(Point p0, Point p1);
//    Point Point::operator+(Point p0, Point p1) {
//            return Point(p0.x+p1.x,p0.y+p1.y,p0.z+p1.z);
//    }
//    Point Point::operator-(Point p0, Point p1) {
//        return Point(p0.x-p1.x,p0.y-p1.y,p0.z-p1.z);
//    }



    /**
         * Storage is always a set of indices, which index the original
         * point set. Thus, you can store anything you want, just put it in a vector
         * that is indexed in the same order as your vector of points.
         */
    template<int dimensions=2,class VectorT=Point> //VectorT needs to have a [] access member, a - operator, and a lengthSquared() method (distance metric)
    class KDTree
    {
    public:
        //constructor
        KDTree()
        {
            kdtree=0;
            kdtreesize=0;
        }
        //destructor
        ~KDTree() {if(kdtree) delete [] kdtree;}

        KDTree &operator=(const KDTree &rhs)
        {
            int kdtreesize=rhs.kdtreesize;
            int validnodes=rhs.validnodes;
            if(kdtree) delete [] kdtree;kdtree=0;
            if(kdtreesize>0)
            {
                kdtree=new DataPoint[kdtreesize];
                for(int c=0;c<kdtreesize;c++)
                {
                    kdtree[c]=rhs.kdtree[c];
                }
            }
            return (*this);
        }

        void generate(const std::vector<VectorT> &pointset) //driver for recursive balancing function
        {
            if(kdtree) delete [] kdtree;
            if(pointset.size()>0.0)
            {
                kdtreesize=2*static_cast<int>(pointset.size()); //note, we use extra space here (times 2) because tree might be slightly unbalanced, causing us to need a little more room
                kdtree = new DataPoint[kdtreesize];

                std::vector<DataPoint> datapointset(pointset.size());
                for(int c=0;c<static_cast<int>(pointset.size());c++)
                {
                    datapointset[c].location=pointset[c];
                    datapointset[c].index=c;
                }

                validnodes=0;
                createBalancedMap(datapointset,0,0,0,static_cast<int>(datapointset.size())-1);
            }
        }

        //radiusQuery
        /**
             * Fill a list of indices of points that are with radius of the given location.
             * Indices are from the original pointset that was passed into generate.
             */
        int radiusQuery(VectorT location,double radius,std::vector<int> &indices) const
        {
            int retVal = 0;
            typename std::vector<DistancedIndex> iWithD;
            double radius2 = radius*radius;
            indices.clear();

            retVal = findPointsInRadius(0,0,location,radius2,NO_MAX,iWithD);
            typename std::vector<DistancedIndex>::iterator i;
            for (i = iWithD.begin(); i != iWithD.end(); ++i) {

                indices.push_back(i->index);
            }

            return retVal;
        }

        //nearestQuery
        /**
             * Fill a list of indices of the k nearest points to the given location.
          * Indices are from the original pointset that was passed into generate.
             */
        int nearestQuery(VectorT location,int k,std::vector<int> &indices) const
        {
            int retVal = 0;
            typename std::vector<DistancedIndex> iWithD;
            double radius2 = DBL_MAX;
            indices.clear();

            // if the user wants 0 or less points, give it to them
            if (k <= 0)
            {
                return 0;
            }

            retVal = findPointsInRadius(0,0,location,radius2,k,iWithD);
            typename std::vector<DistancedIndex>::iterator i;
            for (i = iWithD.begin(); i != iWithD.end(); ++i) {

                indices.push_back(i->index);
            }

            return retVal;
        }

        //nearestInRadiusQuery
        /**
             * Fill a list of indices of the k nearest points to the given location.
          * Indices are from the original pointset that was passed into generate.
             */
        int nearestInRadiusQuery(VectorT location,double radius,int k,std::vector<int> &indices) const
        {
            int retVal = 0;
            typename std::vector<DistancedIndex> iWithD;
            double radius2 = radius*radius;
            indices.clear();

            // if the user wants 0 or less points, give it to them
            if (k <= 0)
            {
                return 0;
            }

            retVal = findPointsInRadius(0,0,location,radius2,k,iWithD);
            typename std::vector<DistancedIndex>::iterator i;
            for (i = iWithD.begin(); i != iWithD.end(); ++i) {

                indices.push_back(i->index);
            }

            return retVal;
        }

    private:

        class DataPoint
        {
            friend class KDTree;
        public:
            DataPoint() {allocated=false;}
        private:
            bool allocated; //used only by the photonmap (not the set)
            VectorT location; //using float instead of double to save space
            int index; //initial index in pointset
        };

        //Used internal while searching for indices within query constraints
        class DistancedIndex {

        public:
            DistancedIndex(int i, double d) {index = i; dist2 =d;}

            int index;
            double dist2;
            bool operator<(const DistancedIndex& rhs) {

                return dist2 < rhs.dist2;
            }
        };

        static const int NO_MAX = 0;   // no maximum number of points indicator

        //KD-tree utilities
        //KD-tree creation (from pointset)
        //O(n*log(n)) run time on average
        void createBalancedMap(std::vector<DataPoint> &pointset,int node,int key,int left,int right)
        {
            int median;
            //make sure everything is in the proper range
            if(node<0 || node>=kdtreesize || left>right)
                return;
            if(left==right)
            {
                //base case (subset contains only one element)
                kdtree[node]=pointset[left];
                kdtree[node].allocated=true;
                validnodes++;
                return;
            }

            //find the median and partition by it
            median=(left+right)/2;
            partitionAtK(pointset,key,left,right,median+1);

            //insert the median into the tree
            kdtree[node]=pointset[median];
            kdtree[node].allocated=true;
            validnodes++;

            //do the left and right subtrees
            createBalancedMap(pointset,(node<<1)+1,(key+1)%dimensions,left,median-1);
            createBalancedMap(pointset,(node<<1)+2,(key+1)%dimensions,median+1,right);
        }

        //KD-tree creation support function (partition a subset of pointset by the k-th smallest element of pointset)
        //(this is just quickSelect in disguise)
        void partitionAtK(std::vector<DataPoint> &pointset, int key,int left,int right,int k) //can be used to select the median and partition the data
        {
            DataPoint temp;

            while(left<right)
            {
                //choose pivot using "median of 3" strategy
                int center=(left+right)/2;
                double pivot;
                if(pointset[center].location[key]<pointset[left].location[key])
                {
                    temp=pointset[center];pointset[center]=pointset[left];pointset[left]=temp;
                }
                if(pointset[right].location[key]<pointset[left].location[key])
                {
                    temp=pointset[right];pointset[right]=pointset[left];pointset[left]=temp;
                }
                if(pointset[right].location[key]<pointset[center].location[key])
                {
                    temp=pointset[right];pointset[right]=pointset[center];pointset[center]=temp;
                }
                //if there are only three elements (left,center, and right), we're done
                if(left+2>right) break;
                //place the pivot at position right-1
                temp=pointset[center];pointset[center]=pointset[right-1];pointset[right-1]=temp;
                pivot=pointset[right-1].location[key];

                //partition the set based on the pivot
                int i=left,j=right-1;
                do
                {
                    while(pointset[++i].location[key]<pivot);
                    while(pointset[--j].location[key]>pivot);
                    if(i<j)
                    {
                        //swap
                        temp=pointset[i];
                        pointset[i]=pointset[j];
                        pointset[j]=temp;
                    }
                }while(i<j);
                //restore pivot
                temp=pointset[i];
                pointset[i]=pointset[right-1];
                pointset[right-1]=temp;

                //iterate on the proper side
                if(k<=i) right=i-1;
                else if(k>i+1) left=i+1;
                else break;
            }
        }

        //KD-tree range query
        int findPointsInRadius(int node,int key,const VectorT &point,double &radius2,int maxpoints,std::vector<DistancedIndex> &found) const
        {
            if(!kdtree) return 0;
            double delta,dist2;
            int numfound=0;
            if(node>=0 && node<kdtreesize && kdtree[node].allocated)
            {
                delta=point[key]-kdtree[node].location[key];
                if(delta<0.0)
                {
                    //the point is on the left side
                    //we certainly have to go down the left
                    numfound+=findPointsInRadius((node<<1)+1,(key+1)%dimensions,point,radius2,maxpoints,found);

                    //do we also have to go down the right?
                    //we do if the partition plane is inside the sphere
                    if(delta*delta<radius2)
                    {
                        numfound+=findPointsInRadius((node<<1)+2,(key+1)%dimensions,point,radius2,maxpoints,found);
                    }
                }else
                {
                    //the point is on the right side
                    //we certainly have to go down the right
                    numfound+=findPointsInRadius((node<<1)+2,(key+1)%dimensions,point,radius2,maxpoints,found);

                    //do we also have to go down the left?
                    //we do if the partition plane is inside the sphere
                    if(delta*delta<radius2)
                    {
                        numfound+=findPointsInRadius((node<<1)+1,(key+1)%dimensions,point,radius2,maxpoints,found);
                    }
                }

                dist2=(point-kdtree[node].location).lengthSquared();
                if(dist2<radius2)
                {
                    if ((maxpoints == NO_MAX) || (static_cast<int>(found.size()) < maxpoints))
                    {
                        found.push_back(DistancedIndex(kdtree[node].index, dist2));
                        numfound++;
                        if ((maxpoints != NO_MAX) && (static_cast<int>(found.size()) == maxpoints))
                        {
                            std::make_heap(found.begin(),found.end());
                            //save time on future searches by restricting the radius
                            radius2=found[0].dist2;
                        }
                    } else
                    {
                        //reform our heap with the new element in it and reset our
                        //search radius
                        std::pop_heap(found.begin(),found.end());
                        found.back()=DistancedIndex(kdtree[node].index,dist2);
                        std::push_heap(found.begin(),found.end());
                        radius2=found[0].dist2;
                    }
                }
            }

            return numfound;
        }

        //KD-tree
        //binary search tree with a key for every dimension
        int kdtreesize;
        int validnodes; //num valid nodes
        DataPoint *kdtree;
    };
}

#endif
