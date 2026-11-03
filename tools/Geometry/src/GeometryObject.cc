#include "GeometryObject.hh"
#include "G4SystemOfUnits.hh"
#include <iostream>

// --- Constructors ---
GeometryObject::GeometryObject()
    : fPosition(0,0,0),
      fDimensions(1,1,1),
      fRotation() {}

GeometryObject::GeometryObject(const G4ThreeVector& position,
                               const G4ThreeVector& dimensions)
    : fPosition(position),
      fDimensions(dimensions),
      fRotation() {}

GeometryObject::~GeometryObject() {}

// --- Getters ---
const G4ThreeVector& GeometryObject::GetPosition() const {
    return fPosition;
}

const G4ThreeVector& GeometryObject::GetDimensions() const {
    return fDimensions;
}

const G4RotationMatrix& GeometryObject::GetRotation() const {
    return fRotation;
}

// --- Setters ---
void GeometryObject::SetPosition(const G4ThreeVector& pos) {
    fPosition = pos;
}

void GeometryObject::SetDimensions(const G4ThreeVector& dim) {
    fDimensions = dim;
}

void GeometryObject::SetRotation(const G4RotationMatrix& rot) {
    fRotation = rot;
}

// --- Translation ---
void GeometryObject::Translate(const G4ThreeVector& delta) {
    fPosition += delta;
}

void GeometryObject::Translate(double dx, double dy, double dz) {
    fPosition += G4ThreeVector(dx, dy, dz);
}

// --- Scaling ---
void GeometryObject::Scale(double factor) {
    fDimensions *= factor;
}

void GeometryObject::Scale(double sx, double sy, double sz) {
    fDimensions.setX(fDimensions.x() * sx);
    fDimensions.setY(fDimensions.y() * sy);
    fDimensions.setZ(fDimensions.z() * sz);
}

// --- Rotation (local axes) ---
void GeometryObject::RotateX(double angle) {
    fRotation.rotateX(angle);
}

void GeometryObject::RotateY(double angle) {
    fRotation.rotateY(angle);
}

void GeometryObject::RotateZ(double angle) {
    fRotation.rotateZ(angle);
}

// --- Rotation around arbitrary axis ---
void GeometryObject::Rotate(const G4ThreeVector& axis, double angle) {
    fRotation.rotate(angle, axis);
}

// --- Rotation around a point in space ---
void GeometryObject::RotateAroundPoint(const G4ThreeVector& point,
                                       const G4ThreeVector& axis,
                                       double angle) {
    // Translate to origin
    G4ThreeVector relative = fPosition - point;

    // Rotate position
    relative.rotate(angle, axis);

    // Translate back
    fPosition = point + relative;

    // Rotate orientation
    fRotation.rotate(angle, axis);
}

// --- Reset ---
void GeometryObject::ResetTransform() {
    fPosition = G4ThreeVector(0,0,0);
    fRotation = G4RotationMatrix();
}

// --- Debug print ---
void GeometryObject::Print() const {
    std::cout << "Position: " << fPosition << std::endl;
    std::cout << "Dimensions: " << fDimensions << std::endl;
    std::cout << "Rotation matrix: " << fRotation << std::endl;
}

void GeometryObject::CenterX(){
    fPosition.setX(0);
}
void GeometryObject::CenterY(){
    fPosition.setY(0);
}
void GeometryObject::CenterZ(){
    fPosition.setZ(0);
}
void GeometryObject::Center(){
    fPosition = G4ThreeVector(0,0,0);
}