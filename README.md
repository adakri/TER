# Display_image
An automated OpenCV tool to detect circular patterns in gray-scale images subject to over-segmentation and generate unstructured triangular meshes for use in finite element analysis.

This is still a work in progress.

## Installation
Follow the instruction in these links in order to intall [OpenCv](https://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html) and [FreeFem++](https://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html) as well as [Cmake](https://cmake.org/download/). 
Launch the Cmake build process once the program is installed :
```bash
Cmake .
```
## Image_processing
Written by: D.Sousa-Pontes, A.Dakri

In order to use the program to generate the particale detected mesh on the image Img.png:
```bash
./launsher.sh Img.png
```

**Example of use**

The geometry detection tool in action:

<img src="https://github.com/adakri/TER/blob/master/Images/test1111_best_10px.jpg?raw=true" width="350" height="250">

Topology detection:

<img src="https://github.com/adakri/TER/blob/master/Images/test3_result125.jpg?raw=true" width="350" height="250">

The result mesh:

<img src="https://github.com/adakri/TER/blob/master/Images/Screenshot from 2021-04-27 16-55-35.png?raw=true" width="350" height="250">


## Solvers
Written by: M.Ettaouchi

A C++ Eigen implementation of different linear sparse matrix solvers for use in a finite element composite linear elsticity problem.
The algorithms implemented include: Gradient conjugate, Minimal residual, Optimal step Gradient, FOM and GMRES as well as an SDP-modified GMRes variant.

# Application to finite element elasticity
Edited by: A.Dakri
The automatic geometry and meshing tool was used in a linear elasticity simulation of Carbon/Carbon composite using the finite element method, source code in the **/FEM** directory.

This is mostly a verified work, at least numerically. The user interaction is reduced to a _datafile.txt_ where all the simulation parameters are defined. To change the studied problem to another field of study (thermoelastics, magnetics, rupture mecanics, CFD ? ..) the main file to modify is _Model.cpp_, the boundary consitions are mainly constructed in _InitialCondition.cpp_. For more advanced users, the liberty is all yours to discover/modify/erase or develop any part you seem fit.

**Example of Results**

Applied efforts on a fiber (model to a circle in a background of a carbon matrix):

<img src="https://github.com/adakri/TER/blob/master/Images/0_0.5_2.png?raw=true" width="300" height="200">

Example of post processing using the appended _.vtk_ paraview tool:

<img src="https://github.com/adakri/TER/blob/master/Images/0.5_20_-40_diag.png?raw=true" width="300" height="200">

