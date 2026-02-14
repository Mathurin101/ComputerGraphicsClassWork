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

	PColor ColorGreen(0xFF123524);//green

	RS_Initialize(Name, PixelWidth, PixelHeight);
	srand(time(0));

	//checking if this line print on the screen
	Points Line(32, 54, 63, 79);
	PColor ColorOG(0xFF8e6acc);//purple

	//1) Before your while-loop, initialize the verts of the cube and store them in an array.
	// -x = left, +x = right
	// -y = down, +y = up
	// -z = near, +z = far

	Vertex FrontCube[8] = {
		//front square
				//  x,      y,     z 
		Vertex(-0.25f,  0.25f, -0.25f),//   top left  = 0
		Vertex( 0.25f,  0.25f, -0.25f),//   top right = 1
		Vertex( 0.25f, -0.25f, -0.25f),//bottom right = 2
		Vertex(-0.25f, -0.25f, -0.25f),//bottom left  = 3

		//back square
		Vertex(-0.25f,  0.25f, 0.25f),//   top left  = 4
		Vertex( 0.25f,  0.25f, 0.25f),//   top right = 5
		Vertex( 0.25f, -0.25f, 0.25f),//bottom right = 6
		Vertex(-0.25f, -0.25f, 0.25f),//bottom left  = 7
	};

	Points CubeLines[12] = {
		Points(FrontCube[0].x,FrontCube[0].y, FrontCube[1].x,FrontCube[1].y),//top left to top right
		Points(FrontCube[1].x,FrontCube[1].y, FrontCube[2].x,FrontCube[2].y),//top right to bottom right
		Points(FrontCube[3].x,FrontCube[3].y, FrontCube[2].x,FrontCube[2].y),//bottom left to bottom right 
		Points(FrontCube[0].x,FrontCube[0].y, FrontCube[3].x,FrontCube[3].y),//top left to bottom left

	};

	//will print on the screen
	do {
		
		
	//2) Inside your while-loop, convert all the verts of the cube.....
	//.....from NDC coordinates to pixel-coordinate, using a function you write (NDCtoScreen()).
	
		//---The cube will have a total width of 0.5 and height of 0.5 and lastly a depth of 0.5---

	//NDCtoScreen(float x, float y, float Width, float Height)



	//3) Also inside the while-loop, pass the converted screen-space pixel coordinates to.....
	//.....your DrawLine() function to connect them.
		
		//ParametricLineFunction(Points Spots, PColor _color, unsigned int* PixelArry, int ArrySize, int RasterWidth)
		
		//DrawPixel(int ArrySpot, PColor color, unsigned int* PixelArry, int ArrySize)

		//NDCtoScreen(float x, float y, float Width, float Height)

		for (int i = 0; i < 4; i++) {

			//make a line of vertexs with the NDC function
			//get those lines ut in the ParametricLineFunction

			// make a line() using NDC for start and end
			// aka Line(NDCTToScreen(start), NDCToScreen(end)) or aka Line(NDCTToScreen(A), NDCToScreen(B))
			// then use that line in drawline


			//ParametricLineFunction(NDCtoScreen(CubeLines[i], PixelWidth, PixelHeight), ColorGreen, TotalPixels, MaxPixels, PixelWidth);
		}
				


	} while (RS_Update(TotalPixels, MaxPixels));

	RS_Shutdown();

	return 0;
}

