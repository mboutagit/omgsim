#pragma once

#include <vtkSmartPointer.h>
#include <vtkCubeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

class Cube {
public:
    Cube(double x, double y, double z,
         double dx, double dy, double dz);

    vtkActor* GetActor();

private:
    vtkSmartPointer<vtkCubeSource> cube;
    vtkSmartPointer<vtkPolyDataMapper> mapper;
    vtkSmartPointer<vtkActor> actor;
};