Cobalt Engine
============
Welcome to the Cobalt wiki.
Cobalt is a 3D graphics engine developed by students for fun and learning. It is currently in early
development. The current version is v0.3.
Dependencies
============
The engine relies on the following libraries:

 - <A href="http://www.assimp.org/">assimp</A>
 - <A href="http://glew.sourceforge.net/">glew</A>
 - <A href="http://www.glfw.org">glfw</A>
 - <A href="http://glm.g-truc.net/">glm</A>
 - <A href="http://www.lonesock.net/soil.html">soil</A>

You will need to install them on your machine before compiling the code. To compile the scene editor
you need QT version 5 or above.
We also used JSON for modern C++ by Niels Lohmann (https://github.com/nlohmann/json) which
is fully included within the source.
The engine was developed under Ubuntu 16.04 64 bit with OpenGL 3.3/GLSL 1.30 and has not been
tested in other configurations.
Building
========
To compile  the source code of the engine and the main example on Linux
with [CMake](https://cmake.org) go to the root folder of the repository and run the command

	cmake CMakeLists.txt && make
You can the run the example using

	./bin/CobaltDemo
To compile and run the scene editor go to the editor folder and run

	qmake && make
	./SceneEditor
The editor links against the Cobalt library, so make sure you build the engine first.
If you want to include a custom include path for QT add it to SceneEditor.pro
after "INCLUDEPATH += ".

To generate a local copy of the wiki with [doxygen](http://doxygen.org) go to the main folder and run

	doxygen doxy_config
The html version will be available in wiki/html/index.html.

If you are too lazy to build everything you can execute build_all. Just navigate to the root
directory and use one of the following commands:

	./build_all
	sh build_all
	bash build_all

