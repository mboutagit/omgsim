#include "utils.hh"

#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

namespace UTILS
{
    BSquare::BSquare(float x1, float x2, float y1, float y2, Color_t lw, Width_t lc){
        lx1 = TLine(x1, y1, x1, y2);
        lx2 = TLine(x2, y1, x2, y2);
        ly1 = TLine(x1, y1, x2, y1);
        ly2 = TLine(x1, y2, x2, y2);
        
        lx1.SetLineColor(lc);
        lx2.SetLineColor(lc);
        ly1.SetLineColor(lc);
        ly2.SetLineColor(lc);

        lx1.SetLineWidth(lw);
        lx2.SetLineWidth(lw);
        ly1.SetLineWidth(lw);
        ly2.SetLineWidth(lw);
    }

    BSquare::BSquare(){
        BSquare(-1000, 1000, -1000, 1000, 2, 2);
    }

    BSquare::~BSquare(){};

    void BSquare::Draw(){
        lx1.Draw("same");
        lx2.Draw("same");
        ly1.Draw("same");
        ly2.Draw("same");
    };

    
    void BSquare::setColor(Color_t lc){
        lx1.SetLineColor(lc);
        lx2.SetLineColor(lc);
        ly1.SetLineColor(lc);
        ly2.SetLineColor(lc);
    }
    void BSquare::setWidth(Width_t lw){
        lx1.SetLineWidth(lw);
        lx2.SetLineWidth(lw);
        ly1.SetLineWidth(lw);
        ly2.SetLineWidth(lw);
    }

    void BSquare::printPostion(std::ostream& out){
        out << "{ (" << lx1.GetX1() << "," << lx1.GetY1() << ")"
            << ", (" << lx1.GetX2() << "," << lx1.GetY2() << ") }"
            << std::endl;
    }

    G4ThreeVector BSquare::getPosition(){
        return G4ThreeVector(   (lx1.GetX1() + lx1.GetX2())/2.,
                                (ly1.GetY1() + ly1.GetY2())/2.,
                                0.
        );
    }
    void BSquare::setPosition(G4ThreeVector new_pos){
        G4ThreeVector old_pos = getPosition();
        G4ThreeVector delte_pos = old_pos - new_pos;
        //l1
        lx1.SetX1(lx1.GetX1() + delte_pos.getX());  lx1.SetY1(lx1.GetY1() + delte_pos.getY()); 
        lx1.SetX2(lx1.GetX2() + delte_pos.getX());  lx1.SetY2(lx1.GetY2() + delte_pos.getY()); 

        //l2
        lx2.SetX1(lx2.GetX1() + delte_pos.getX());  lx2.SetY1(lx2.GetY1() + delte_pos.getY()); 
        lx2.SetX2(lx2.GetX2() + delte_pos.getX());  lx2.SetY2(lx2.GetY2() + delte_pos.getY()); 

        //l3
        ly1.SetX1(ly1.GetX1() + delte_pos.getX());  ly1.SetY1(ly1.GetY1() + delte_pos.getY()); 
        ly1.SetX2(ly1.GetX2() + delte_pos.getX());  ly1.SetY2(ly1.GetY2() + delte_pos.getY()); 

        //l4
        ly2.SetX1(ly2.GetX1() + delte_pos.getX());  ly2.SetY1(ly2.GetY1() + delte_pos.getY()); 
        ly2.SetX2(ly2.GetX2() + delte_pos.getX());  ly2.SetY2(ly2.GetY2() + delte_pos.getY()); 
    }

    G4ThreeVector BSquare::getDimension(){
        return G4ThreeVector(
            abs( lx1.GetX2() - lx1.GetX1() ),
            abs( lx1.GetY2() - lx1.GetY1() ),
            0
        );
    }

    void BSquare::setDimension(G4ThreeVector new_dim){
        G4ThreeVector pos = getPosition();
        //l1
        lx1.SetX1(pos.getX() - new_dim.getX());  lx1.SetY1(pos.getY() + new_dim.getY()); 
        lx1.SetX2(pos.getX() - new_dim.getX());  lx1.SetY2(pos.getY() + new_dim.getY()); 

        //l2
        lx2.SetX1(pos.getX() - new_dim.getX());  lx2.SetY1(pos.getY() + new_dim.getY()); 
        lx2.SetX2(pos.getX() - new_dim.getX());  lx2.SetY2(pos.getY() + new_dim.getY()); 

        //l3
        ly1.SetX1(pos.getX() - new_dim.getX());  ly1.SetY1(pos.getY() + new_dim.getY()); 
        ly1.SetX2(pos.getX() - new_dim.getX());  ly1.SetY2(pos.getY() + new_dim.getY()); 

        //l4
        ly2.SetX1(pos.getX() - new_dim.getX());  ly2.SetY1(pos.getY() + new_dim.getY()); 
        ly2.SetX2(pos.getX() - new_dim.getX());  ly2.SetY2(pos.getY() + new_dim.getY()); 
    }

    void BSquare::centerX(){
        lx1.SetX1( - (lx1.GetX1() + lx1.GetX2())/2.);  lx1.SetX2(- (lx1.GetX1() + lx1.GetX2())/2.);
        lx2.SetX1( - (lx2.GetX1() + lx2.GetX2())/2.);  lx2.SetX2(- (lx2.GetX1() + lx2.GetX2())/2.);
        ly1.SetX1( - (ly1.GetX1() + ly1.GetX2())/2.);  ly1.SetX2(- (ly1.GetX1() + ly1.GetX2())/2.);
        ly2.SetX1( - (ly2.GetX1() + ly2.GetX2())/2.);  ly2.SetX2(- (ly2.GetX1() + ly2.GetX2())/2.);
    }

    void BSquare::centerY(){
        lx1.SetY1( - (lx1.GetY1() + lx1.GetY2())/2.);  lx1.SetY2(- (lx1.GetY1() + lx1.GetY2())/2.);
        lx2.SetY1( - (lx2.GetY1() + lx2.GetY2())/2.);  lx2.SetY2(- (lx2.GetY1() + lx2.GetY2())/2.);
        ly1.SetY1( - (ly1.GetY1() + ly1.GetY2())/2.);  ly1.SetY2(- (ly1.GetY1() + ly1.GetY2())/2.);
        ly2.SetY1( - (ly2.GetY1() + ly2.GetY2())/2.);  ly2.SetY2(- (ly2.GetY1() + ly2.GetY2())/2.);
    }

    void BSquare::translate(double tx, double ty){
        setPosition(G4ThreeVector(tx, ty, 0));
    }

    void BSquare::center(){
        centerX();
        centerY();
    }
    
} // namespace UTILS
