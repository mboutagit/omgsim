#ifndef Utils_hh
#define Utils_hh

#include <iostream>
#include <vector>


using namespace std;

class Vector3D {

    public: 
        double x;
        double y;
        double z;

        Vector3D();
        Vector3D(double); // they all conatin the same
        Vector3D(double, double, double);

        friend std::ostream& operator<<(std::ostream& os, const Vector3D& v);
};



#endif // Utils_hh
