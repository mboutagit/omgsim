#include "Cube.hh"

// Cube::Cube(double x, double y, double z,
//            double dx, double dy, double dz)
// {
//     // Geometry
//     cube = vtkSmartPointer<vtkCubeSource>::New();
//     cube->SetXLength(dx);
//     cube->SetYLength(dy);
//     cube->SetZLength(dz);

//     // Mapper
//     mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//     mapper->SetInputConnection(cube->GetOutputPort());

//     // Actor
//     actor = vtkSmartPointer<vtkActor>::New();
//     actor->SetMapper(mapper);

//     // Position
//     actor->SetPosition(x, y, z);
// }

// vtkActor* Cube::GetActor()
// {
//     return actor;
// }