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
Writen by: D.Sousa-Pontes, A.Dakri

In order to use the program to generate the particale detected mesh on the image Img.png:
```bash
./launsher.sh Img.png
```
## Solvers
Written by: M.Ettaouchi

A C++ Eigen implementation of different linear sparse matrix solvers for use in a finite element composite linear elsticity problem.
The algorithms implemented include: Gradient conjugate, Minimal residual, Optimal step Gradient, FOM and GMRES as well as an SDP-modified GMRes variant.
