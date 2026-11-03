#ifndef GEOMETRYOBJECT_HH
#define GEOMETRYOBJECT_HH

#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

#include "TROOT.h"

class GeometryObject {
public:
    // Constructors
    GeometryObject();
    GeometryObject(const G4ThreeVector& position,
                   const G4ThreeVector& dimensions);

    // Destructor
    virtual ~GeometryObject();

    // --- Getters ---
    const G4ThreeVector& GetPosition() const;
    const G4ThreeVector& GetDimensions() const;
    const G4RotationMatrix& GetRotation() const;

    // --- Setters ---
    void SetPosition(const G4ThreeVector& pos);
    void SetDimensions(const G4ThreeVector& dim);
    void SetRotation(const G4RotationMatrix& rot);

    // --- Translation ---
    void Translate(const G4ThreeVector& delta);
    void Translate(double dx, double dy, double dz);

    // --- Scaling ---
    void Scale(double factor);
    void Scale(double sx, double sy, double sz);

    // --- Rotation (about object center) ---
    void RotateX(double angle);
    void RotateY(double angle);
    void RotateZ(double angle);

    void CenterX();
    void CenterY();
    void CenterZ();
    void Center();

    // --- Rotation around arbitrary axis ---
    void Rotate(const G4ThreeVector& axis, double angle);

    // --- Rotation around a point in space ---
    void RotateAroundPoint(const G4ThreeVector& point,
                           const G4ThreeVector& axis,
                           double angle);

    // --- Utility ---
    void ResetTransform();
    void Print() const;

protected:
    G4ThreeVector fPosition;
    G4ThreeVector fDimensions;
    G4RotationMatrix fRotation;

    ClassDef(GeometryObject, 0);
};

#endif