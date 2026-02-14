#include "RasterSurface.h"
#include "Helper.h"
#include "XTime.h"
#include "tiles_12.h"
#include "teleporter_hit.h"
#include <random> //random number class "std::rand()"
#include <iostream>

const char* Name = "MathurinGenty_Line Drawing";
const unsigned int PixelWidth = 500;
const unsigned int PixelHeight = 500;

const unsigned int MaxPixels = PixelWidth * PixelHeight;

unsigned int TotalPixels[MaxPixels];


int main()
{
	Position scrRect(0, 0, tiles_12_width, tiles_12_height);
	const Position RasterPos(0, 0, PixelWidth, PixelHeight);

	PColor ColorCH(0xFF123524);//green

	RS_Initialize(Name, PixelWidth, PixelHeight);
	srand(time(0));

	//checking if this line print on the screen
	Points Line( 32, 54, 63, 79);
	PColor ColorOG(0xFF8e6acc);//purple

	//1) Before your while-loop, initialize the verts of the cube and store them in an array.
	// -x = left, +x = right
	// -y = down, +y = up
	// -z = near, +z = far

	Vertex FrontCube[8] = {
		//front square
			    //  x,      y,     z 
		Vertex(-0.25f,  0.25f, -0.25f),//   top left
		Vertex( 0.25f,  0.25f, -0.25f),//   top right
		Vertex(-0.25f, -0.25f, -0.25f),//bottom left
		Vertex( 0.25f, -0.25f, -0.25f),//bottom right

		//back square
		Vertex(-0.25f,  0.25f, 0.25f),//   top left
		Vertex( 0.25f,  0.25f, 0.25f),//   top right
		Vertex(-0.25f, -0.25f, 0.25f),//bottom left
		Vertex( 0.25f, -0.25f, 0.25f),//bottom right
	};




	//2) Inside your while-loop, convert all the verts of the cube .....
	//.....from NDC coordinates to pixel-coordinate, using a function you write (NDCtoScreen()).

	//3) Also inside the while-loop, pass the converted screen-space pixel coordinates to.....
	//.....your DrawLine() function to connect them.

	//will print on the screen
	do {
		


	} while (RS_Update(TotalPixels, MaxPixels));

	RS_Shutdown();

	return 0;
}

