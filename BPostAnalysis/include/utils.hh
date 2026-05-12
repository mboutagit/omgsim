#ifndef UTILS_HH
#define UTILS_HH

#include <iostream>
#include "TObject.h"
#include "TLine.h"
#include "TColor.h"

#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

using namespace std;
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
    
    class BSquare : public Shape
    {
    private:
        /* data */
        TLine lx1, lx2, ly1, ly2;
    public:
        BSquare();
        BSquare(float x1, float x2, float y1, float y2, Color_t lw = 1, Width_t lc = 1);
        ~BSquare();

        void setColor(Color_t color);
        void setWidth(Width_t wd);
        void printPostion(std::ostream& out = std::cout);
        void centerX();
        void centerY();
        void center();
        G4ThreeVector getPosition();
        void setPosition(G4ThreeVector new_pos);
        G4ThreeVector getDimension();
        void setDimension(G4ThreeVector new_dim);
        void translate(double tx, double ty);
        void Draw();
    };


    class Object3D
    {
    private:
        double x_pos;
        double y_pos;
        double z_pos;

        double x_dim;
        double y_dim;
        double z_dim;

    public:
        Object3D(/* args */){};
        ~Object3D(){};
    };
    
    
} // namespace UTILS



#endif