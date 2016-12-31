# 3D-ConvexHull

# What is this repository for?
* Final Version : Christmas-Red
.
├── as2 : The Project Container
│   ├── ConvexHull
│   │   ├── ConvexHullBuilder: Here is my work
│   │   ├── GUI
│   │   │   ├── managers
│   │   │   └── objects
│   │   └── lib
│   │       ├── common
│   │       ├── dcel
│   │       └── dcel_doc
│   │           └── it
│   │               └── search
│   └── libraries_toInstall_IfNeeded
├── models : FOR TESTING
└── Specification : Conception and technical specifications


### How do I get set up? ###
How to compile the Project in Ubuntu :

Installare QtCreator e Qt5.
sudo apt-get install qtcreator qt5-default

Installare le seguenti librerie:
sudo apt-get install libboost-all-dev libcgal-dev libgmp-dev libqglviewer-dev

If "libqglviewer-dev" is not able to be installed, you will found a.deb of the library to install

Documentation of the project:

Install Doxygen:
sudo apt-get install doxygen

Activate the doc in /lib:
doxygen dcel_doxygen_config_it
a "dcel_doc" in <HTML> will be generated
### Not to do ###
Don't try to work with Dcel::Vertex* instead Pointd, because random_shuffling don't return the exact coordinates(x,y,z).
### Contribution guidelines ###
This Contribution was a final course's project that we have to develop an algorithm to calculate the Convex Hull of a given
3D shape.
The base project (FrameWork) is contributed by : muntoni.alessandro@gmail.com and cordafab@gmail.com
### Run in /ConvexHull ###
In terminal :
	qmake
	make
	65073_chaoua_ilyas.pro
In Qt5:
	Open project
