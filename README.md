# Authors

 - Christophe Hugon - CPPM (CNRS)
 - Vladimir Kulikovskiy - INFN Genova

# Containers and modules

 Docker and singularity containers are build in in the gitlab and stored in sftp (singularity). On Lyon `omgsim` modules are available

```
module load omgsim
OMGsim
```

# Documentation

The doxygen documentation is provided in the folder `documentation` (including installation guides).
It is easier to use it at your local machine.

## Compile with:

    cd [your checkout path]/trunk
    doxygen documentation/config.dox
    cd documentation/latex
    make

## Open (on linux):

    firefox documentation/html/index.html
    gnome-open latex/refman.pdf

## Open (on Mac):

    open documentation/html/index.html 
    open latex/refman.pdf

Note that the Doxygen needs to be installed. On MacOS type:

    brew install doxygen  
