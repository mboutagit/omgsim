#ifndef Utils_hh
#define Utils_hh

#include "TObject.h"
#include "TLine.h"

namespace UTILS
{

    class Shape : public TObject
    {
    private:
        /* data */
    public:
        Shape(/* args */){};
        ~Shape(){};
    };


    /*
    *
    *          _____________
    *         |             |
    *         |             |   
    *         |             |
    *         |_____________|
    *  
    */
    
    class BBox : public Shape
    {
    private:
        /* data */
        TLine lx1, lx2, ly1, ly2;
    public:
        BBox(float x, float x2, float y1, float y2, int lw = 1, int lc = 1);
        ~BBox();
        void Draw();
    };
    
    
} // namespace UTILS



#endif