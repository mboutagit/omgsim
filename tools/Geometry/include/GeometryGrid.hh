#ifndef GEOMETRYGRID_HH
#define GEOMETRYGRID_HH

#include "GeometryObject.hh"
#include <vector>
#include "TROOT.h"

#include "BLogger.hh"

using namespace utils;

template <typename T>
class GeometryGrid {
public:
    // GeometryGrid();
    // GeometryGrid(int nx, int ny, int nz,
    //              const G4ThreeVector& spacing);

    // virtual ~GeometryGrid();

    // // --- Configuration ---
    // void SetCounts(int nx, int ny, int nz);
    // void SetSpacing(const G4ThreeVector& spacing);
    // void SetOrigin(const G4ThreeVector& origin);

    // int GetNx() const;
    // int GetNy() const;
    // int GetNz() const;

    // // --- Build grid ---
    // void BuildGrid(const T& prototype);

    // // --- Access ---
    // T& Get(int ix, int iy, int iz);
    // const T& Get(int ix, int iy, int iz) const;

    // size_t GetTotal() const;

    // // --- Global transformations ---
    // void Translate(const G4ThreeVector& delta);
    // void RotateX(double angle);
    // void RotateY(double angle);
    // void RotateZ(double angle);

    // void Rotate(const G4ThreeVector& axis, double angle);

    // // --- Centering / alignment ---
    // void CenterGrid();
    // G4ThreeVector GetCenter() const;

    // // --- Utilities ---
    // void Clear();
    // void PrintSummary() const;

    // std::vector<T> GetObjects();

    GeometryGrid()
        : fNx(1), fNy(1), fNz(1),
        fSpacing(1,1,1),
        fOrigin(0,0,0) {}

    GeometryGrid(int nx, int ny, int nz,
                            const G4ThreeVector& spacing)
        : fNx(nx), fNy(ny), fNz(nz),
        fSpacing(spacing),
        fOrigin(0,0,0) {}

    ~GeometryGrid() {}

    // --- Configuration ---
    void SetCounts(int nx, int ny, int nz) {
        fNx = nx; fNy = ny; fNz = nz;
    }

    void SetSpacing(const G4ThreeVector& spacing) {
        fSpacing = spacing;
    }

    void SetOrigin(const G4ThreeVector& origin) {
        fOrigin = origin;
    }

    int GetNx() const { return fNx; }
    int GetNy() const { return fNy; }
    int GetNz() const { return fNz; }

    // --- Indexing ---
    int Index(int ix, int iy, int iz) const {
        return ix + fNx * (iy + fNy * iz);
    }

    
    void addObject(T obj){
        fObjects.push_back(obj);
    }  
    
    // void addObject(T& obj){
    //     fObjects.push_back(obj);
    // }  
    
    // void addObjects(std::vector<T&> objs){
    //     for (auto& obj : objs)
    //         fObjects.push_back(obj);
    // } 

    void addObjects(std::vector<T> objs){
        for (auto& obj : objs)
            fObjects.push_back(obj);
    } 

    void BuildGrid(int nx, int ny, int nz) {
        if(nx*ny*nz > fObjects.size()){
            BLOG_WARNING("required grid size is higher than the current objects number. Building anyway!");
        }
        
        if(nx*ny*nz < fObjects.size()){
            BLOG_WARNING("required grid size is lower than the current objects number. Building anyway!");
        }

        int ii = 0;
        for (int iz = 0; iz < nz; ++iz) {
            for (int iy = 0; iy < ny; ++iy) {
                for (int ix = 0; ix < nx; ++ix) {

                    G4ThreeVector pos(
                        ix * fSpacing.x(),
                        iy * fSpacing.y(),
                        iz * fSpacing.z()
                    );

                    fObjects[ii].SetPosition(fOrigin + pos);
                    ii++;
                }
            }
        }
    }


    // --- Build grid ---
    void BuildGrid(const T& prototype) {
        fObjects.clear();
        fObjects.reserve(fNx * fNy * fNz);

        for (int iz = 0; iz < fNz; ++iz) {
            for (int iy = 0; iy < fNy; ++iy) {
                for (int ix = 0; ix < fNx; ++ix) {

                    T obj = prototype;

                    G4ThreeVector pos(
                        ix * fSpacing.x(),
                        iy * fSpacing.y(),
                        iz * fSpacing.z()
                    );

                    obj.SetPosition(fOrigin + pos);
                    fObjects.push_back(obj);
                }
            }
        }
    }

    // --- Access ---
    T& Get(int ix, int iy, int iz) {
        return fObjects[Index(ix, iy, iz)];
    }

    const T& Get(int ix, int iy, int iz) const {
        return fObjects[Index(ix, iy, iz)];
    }

    size_t GetTotal() const {
        return fObjects.size();
    }

    // --- Global transformations ---
    void Translate(const G4ThreeVector& delta) {
        for (auto& obj : fObjects) {
            obj.Translate(delta);
        }
    }

    void RotateX(double angle) {
        G4ThreeVector center = GetCenter();
        for (auto& obj : fObjects) {
            obj.RotateAroundPoint(center, G4ThreeVector(1,0,0), angle);
        }
    }

    void RotateY(double angle) {
        G4ThreeVector center = GetCenter();
        for (auto& obj : fObjects) {
            obj.RotateAroundPoint(center, G4ThreeVector(0,1,0), angle);
        }
    }

    void RotateZ(double angle) {
        G4ThreeVector center = GetCenter();
        for (auto& obj : fObjects) {
            obj.RotateAroundPoint(center, G4ThreeVector(0,0,1), angle);
        }
    }

    void Rotate(const G4ThreeVector& axis, double angle) {
        G4ThreeVector center = GetCenter();
        for (auto& obj : fObjects) {
            obj.RotateAroundPoint(center, axis, angle);
        }
    }

    // --- Centering ---
    G4ThreeVector GetCenter() const {
        if (fObjects.empty()) return G4ThreeVector(0,0,0);

        G4ThreeVector sum(0,0,0);
        for (const auto& obj : fObjects) {
            sum += obj.GetPosition();
        }
        return sum / fObjects.size();
    }

    void CenterGrid() {
        G4ThreeVector center = GetCenter();
        Translate(-center);
    }

    // --- Utilities ---

    void Clear() {
        fObjects.clear();
    }

    void PrintSummary() const {
        std::cout << "Grid: "
                << fNx << " x " << fNy << " x " << fNz
                << " = " << fObjects.size() << " objects\n";
    }

    std::vector<T>& GetObjects(){
        return fObjects;
    }

    const std::vector<T>& GetObjects() const{
        return fObjects;
    }

    const G4ThreeVector GetGridCenter() const{
        G4ThreeVector pos;
            for(auto& obj : fObjects)
                pos+=obj.GetPosition();
        return pos/fObjects.size();
    }

    const G4ThreeVector GetGridDimensions() const{
        if(! GetObjects().size())
            return G4ThreeVector();

        G4ThreeVector   minDim = fObjects[0].GetPosition() - fObjects[0].GetDimensions(), 
                        maxDim = fObjects[0].GetPosition() + fObjects[0].GetDimensions();

        for(auto& obj : fObjects){
            minDim = std::min(minDim, obj.GetPosition() - obj.GetDimensions());
            maxDim = std::max(maxDim, obj.GetPosition() + obj.GetDimensions());
        }
        return maxDim - minDim;
    }

protected:
    int fNx, fNy, fNz;
    G4ThreeVector fSpacing;
    G4ThreeVector fOrigin;

    std::vector<T> fObjects;

    // int Index(int ix, int iy, int iz) const;

    ClassDef(GeometryGrid, 0);
};

#endif