#include "Utils.hh"

Vector3D::Vector3D(): x(0.), y(0.), z(0.) {}
Vector3D::Vector3D(double val): x(val), y(val), z(val) {}
Vector3D::Vector3D(double valx, double valy, double valz): x(valx), y(valy), z(valz) {}

std::ostream& operator<<(std::ostream& os, const Vector3D& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}


