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
