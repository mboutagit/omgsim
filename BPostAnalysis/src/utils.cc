#include "utils.hh"

namespace UTILS
{

     
    BBox::BBox(float x1, float x2, float y1, float y2, int lw, int lc){
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
    BBox::~BBox(){};

    void BBox::Draw(){
        lx1.Draw("same");
        lx2.Draw("same");
        ly1.Draw("same");
        ly2.Draw("same");
    };

    
} // namespace UTILS
