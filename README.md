# GMLtoGL 
GMLtoGL allows you to easily convert your .gml files to .gl files !

### Installation

GMLtoGL requires a few dependencies to work.
To allow you to swiftly convert your .gml files into .gl, here are few steps you should do :

First of all, install the following dependencies :

* cmake
* libxerces-c-dev
* libxercesX.X (3.1 or higher)
* libgdal-dev

Here are the commands to help you install these dependencies : 

```sh
$ sudo apt-get install cmake
$ sudo apt-get install libxerces-c-dev
$ sudo apt-cache search -libxerces
$ sudo apt-get install -libxerces-cx.x (with x the version numbers)
$ sudo apt-get install -libgdal-dev
```

You also need to install gkit2light : 
    http://perso.univ-lyon1.fr/jean-claude.iehl/Public/educ/M1IMAGE/html/group__installation.html
Then copy the GMLtoGL folder in your gkit2light folder.


You also need to change some variables in the file libcitygml-master->CMakeCache.txt :
> line #2 : #For build in directory: /REPLACE_WITH_YOUR_OWN_PATH/libcitygml-master
> line #434 : libcitygml_BINARY_DIR:STATIC=/REPLACE_WITH_YOUR_OWN_PATH/libcitygml-master
> line #437 : libcitygml_SOURCE_DIR:STATIC=/REPLACE_WITH_YOUR_OWN_PATH/libcitygml-master
> line #447 : CMAKE_CACHEFILE_DIR:INTERNAL=/REPLACE_WITH_YOUR_OWN_PATH/libcitygml-master
> line #516 : CMAKE_HOME_DIRECTORY:INTERNAL=/REPLACE_WITH_YOUR_OWN_PATH/libcitygml-master/

Once all the above steps are done, you can enter the following commands :
(You need to be at your libcitygml-master directory) :
```sh
$ sudo cmake -G "Unix Makefiles"
$ sudo make install
```
Then go in your gkit2light folder, and edit your premake4.lua file :
> replace your line #28 with this line : links { "GLEW", "SDL2", "SDL2_image", "GL", "citygml" } 

>and add those lines at the end of your premake4.lua file : 

project("GMLtoGL")
    language "C++"
	kind "ConsoleApp"
	targetdir "bin"
	files ( gkit_files )
	files { gkit_dir .. "/GMLtoGL/*.cpp" }
	files { gkit_dir .. "/GMLtoGL/*.h" }

Now you can start compiling your files :
	
```sh
$ premake4 gmake
$ make GMLtoGL
```

### Running

To run the program :

```sh
$ ./bin/GMLtoGL
```
	
### Todos

 -Prettty much evereything

License
----

