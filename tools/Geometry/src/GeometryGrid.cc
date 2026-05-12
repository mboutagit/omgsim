#include "GeometryGrid.hh"
#include <iostream>
#include <stdexcept>

// --- Constructors ---
// template<typename T>
// GeometryGrid<T>::GeometryGrid()
//     : fNx(1), fNy(1), fNz(1),
//       fSpacing(1,1,1),
//       fOrigin(0,0,0) {}

// template<typename T>
// GeometryGrid<T>::GeometryGrid(int nx, int ny, int nz,
//                            const G4ThreeVector& spacing)
//     : fNx(nx), fNy(ny), fNz(nz),
//       fSpacing(spacing),
//       fOrigin(0,0,0) {}

// template<typename T>
// GeometryGrid<T>::~GeometryGrid() {}

// // --- Configuration ---
// template<typename T>
// void GeometryGrid<T>::SetCounts(int nx, int ny, int nz) {
//     fNx = nx; fNy = ny; fNz = nz;
// }

// template<typename T>
// void GeometryGrid<T>::SetSpacing(const G4ThreeVector& spacing) {
//     fSpacing = spacing;
// }

// template<typename T>
// void GeometryGrid<T>::SetOrigin(const G4ThreeVector& origin) {
//     fOrigin = origin;
// }

// template<typename T>
// int GeometryGrid<T>::GetNx() const { return fNx; }
// template<typename T>
// int GeometryGrid<T>::GetNy() const { return fNy; }
// template<typename T>
// int GeometryGrid<T>::GetNz() const { return fNz; }

// // --- Indexing ---
// template<typename T>
// int GeometryGrid<T>::Index(int ix, int iy, int iz) const {
//     return ix + fNx * (iy + fNy * iz);
// }

// // --- Build grid ---
// template<typename T>
// void GeometryGrid<T>::BuildGrid(const T& prototype) {
//     fObjects.clear();
//     fObjects.reserve(fNx * fNy * fNz);

//     for (int iz = 0; iz < fNz; ++iz) {
//         for (int iy = 0; iy < fNy; ++iy) {
//             for (int ix = 0; ix < fNx; ++ix) {

//                 T obj = prototype;

//                 G4ThreeVector pos(
//                     ix * fSpacing.x(),
//                     iy * fSpacing.y(),
//                     iz * fSpacing.z()
//                 );

//                 obj.SetPosition(fOrigin + pos);

//                 fObjects.push_back(obj);
//             }
//         }
//     }
// }

// // --- Access ---
// template<typename T>
// T& GeometryGrid<T>::Get(int ix, int iy, int iz) {
//     return fObjects[Index(ix, iy, iz)];
// }

// template<typename T>
// const T& GeometryGrid<T>::Get(int ix, int iy, int iz) const {
//     return fObjects[Index(ix, iy, iz)];
// }

// template<typename T>
// size_t GeometryGrid<T>::GetTotal() const {
//     return fObjects.size();
// }

// // --- Global transformations ---
// template<typename T>
// void GeometryGrid<T>::Translate(const G4ThreeVector& delta) {
//     for (auto& obj : fObjects) {
//         obj.Translate(delta);
//     }
// }

// template<typename T>
// void GeometryGrid<T>::RotateX(double angle) {
//     G4ThreeVector center = GetCenter();
//     for (auto& obj : fObjects) {
//         obj.RotateAroundPoint(center, G4ThreeVector(1,0,0), angle);
//     }
// }

// template<typename T>
// void GeometryGrid<T>::RotateY(double angle) {
//     G4ThreeVector center = GetCenter();
//     for (auto& obj : fObjects) {
//         obj.RotateAroundPoint(center, G4ThreeVector(0,1,0), angle);
//     }
// }

// template<typename T>
// void GeometryGrid<T>::RotateZ(double angle) {
//     G4ThreeVector center = GetCenter();
//     for (auto& obj : fObjects) {
//         obj.RotateAroundPoint(center, G4ThreeVector(0,0,1), angle);
//     }
// }

// template<typename T>
// void GeometryGrid<T>::Rotate(const G4ThreeVector& axis, double angle) {
//     G4ThreeVector center = GetCenter();
//     for (auto& obj : fObjects) {
//         obj.RotateAroundPoint(center, axis, angle);
//     }
// }

// // --- Centering ---
// template<typename T>
// G4ThreeVector GeometryGrid<T>::GetCenter() const {
//     if (fObjects.empty()) return G4ThreeVector(0,0,0);

//     G4ThreeVector sum(0,0,0);
//     for (const auto& obj : fObjects) {
//         sum += obj.GetPosition();
//     }
//     return sum / fObjects.size();
// }

// template<typename T>
// void GeometryGrid<T>::CenterGrid() {
//     G4ThreeVector center = GetCenter();
//     Translate(-center);
// }

// // --- Utilities ---

// template<typename T>
// void GeometryGrid<T>::Clear() {
//     fObjects.clear();
// }

// template<typename T>
// void GeometryGrid<T>::PrintSummary() const {
//     std::cout << "Grid: "
//               << fNx << " x " << fNy << " x " << fNz
//               << " = " << fObjects.size() << " objects\n";
// }

// template<typename T>
// std::vector<T> GeometryGrid<T>::GetObjects(){
//     return fObjects;
// }