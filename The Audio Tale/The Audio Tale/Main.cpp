
#include <string>
#include <iostream>

// Graphics Includes
/*#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h> 
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>*/

// Audio Includes
#include "audioParser.h"
#include "FMODLoader.h"

#include "GraphicsManager.h"

int main()
{
	GraphicsManager graph(std::string("The Audio Tale"), 1500, 750);
	graph.init();
	graph.loop();
	return (0);
}