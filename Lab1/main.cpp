#include "RasterSurface.h"
#include "Helper.h"
#include "XTime.h"
#include "tiles_12.h"
#include "teleporter_hit.h"
#include "Shaders.h"

#include <random> //random number class "std::rand()"
#include <iostream>

const char* Name = "MathurinGenty_Line Drawing";
const unsigned int PixelWidth = 500;
const unsigned int PixelHeight = 500;

const unsigned int MaxPixels = PixelWidth * PixelHeight;

unsigned int TotalPixels[MaxPixels];

Vertex FrontCube[8] = {
	// -x = left, +x = right
	// -y = down, +y = up
	// -z = near screen, +z = far screen
	
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

Vertex Grid[42] = {

	//top to bottom grid
			//  x, y,     z 
	Vertex(-0.50f, 0,  0.50f),//upBorderL
	Vertex(-0.50f, 0, -0.50f),//downBorderL

	Vertex(-0.40f, 0,  0.50f),//upLL
	Vertex(-0.40f, 0, -0.50f),//downLL

	Vertex(-0.30f, 0,  0.50f),//upL
	Vertex(-0.30f, 0, -0.50f),//downL

	Vertex(-0.20f, 0,  0.50f),//upLM
	Vertex(-0.20f, 0, -0.50f),//downLM

	Vertex(-0.10f, 0,  0.50f),//upLM
	Vertex(-0.10f, 0, -0.50f),//downLM


	Vertex(0.10f, 0,  0.50f),//upLM
	Vertex(0.10f, 0, -0.50f),//downLM

	Vertex(0.20f, 0,  0.50f),//upRM
	Vertex(0.20f, 0, -0.50f),//downRM

	Vertex(0.30f, 0,  0.50f),//upR
	Vertex(0.30f, 0, -0.50f),//downR

	Vertex(0.40f, 0,  0.50f),//upRR
	Vertex(0.40f, 0, -0.50f),//downRR

	Vertex(0.50f, 0,  0.50f),//upBorderR
	Vertex(0.50f, 0, -0.50f),//downBorderR




	//side to side grind
			  //x, y,     z
	Vertex(-0.50f, 0, 0.50f),//leftTop 
	Vertex(0.50f, 0, 0.50f),//rightTop

	Vertex(-0.50f, 0, 0.40f),//leftTT
	Vertex(0.50f, 0, 0.40f),//rightTT

	Vertex(-0.50f, 0, 0.30f),//leftT 
	Vertex(0.50f, 0, 0.30f),//rightT

	Vertex(-0.50f, 0, 0.20f),//leftTM 
	Vertex(0.50f, 0, 0.20f),//rightTM

	Vertex(-0.50f, 0, 0.10f),//leftTMM 
	Vertex(0.50f, 0, 0.10f),//rightTMM

	Vertex(-0.50f, 0, 0.0),//leftM 
	Vertex(0.50f, 0, 0.0),//rightM

	Vertex(-0.50f, 0, -0.10f),//leftBMM 
	Vertex(0.50f, 0, -0.10f),//rightBMM

	Vertex(-0.50f, 0, -0.20f),//leftBM 
	Vertex(0.50f, 0, -0.20f),//rightBM

	Vertex(-0.50f, 0, -0.30f),//leftB 
	Vertex(0.50f, 0, -0.30f),//rightB

	Vertex(-0.50f, 0, -0.40f),//leftBB 
	Vertex(0.50f, 0, -0.40f),//rightBB

	Vertex(-0.50f, 0, -0.50f),//leftBottom 
	Vertex(0.50f, 0, -0.50f),//rightBottom	
};


void DrawCube();
void DrawGrid();


PColor ColorGreen(0xFF123524);//green
PColor ColorWhite(0xFFFFFFFF);//white

int main()
{
	Position scrRect(0, 0, tiles_12_width, tiles_12_height);
	const Position RasterPos(0, 0, PixelWidth, PixelHeight);

	RS_Initialize(Name, PixelWidth, PixelHeight);
	srand(time(0));

	//checking if this line print on the screen
	Points Line(32, 54, 63, 79);
	PColor ColorOG(0xFF8e6acc);//purple


	VertexShader = VS_WVP;

	Matrix4x4 GridWorld = IdentityMatrix();
	Matrix4x4 CubeWorld = TranslationMatrix(0, 0.25f, 0);

	//will print on the screen
	do {
		VS_World = GridWorld;
		DrawGrid();

		VS_World = CubeWorld;
		DrawCube();

	} while (RS_Update(TotalPixels, MaxPixels));

	RS_Shutdown();

	return 0;
}

void DrawCube() {
	for (int i = 0; i < 7; i++) {

		//make a line of vertexs with the NDC function
		//get those lines ut in the ParametricLineFunction

		// make a line() using NDC for start and end
		// aka Line(NDCTToScreen(start), NDCToScreen(end)) or aka Line(NDCTToScreen(A), NDCToScreen(B))


		// then use that line in drawline

		VS_WVP(*FrontCube);

		ParametricLineFunction(
	 Points(NDCtoScreen(FrontCube[i], PixelWidth, PixelHeight).x1,
			NDCtoScreen(FrontCube[i], PixelWidth, PixelHeight).y1,
			NDCtoScreen(FrontCube[i + 1], PixelWidth, PixelHeight).x1,
			NDCtoScreen(FrontCube[i + 1], PixelWidth, PixelHeight).y1),
			ColorGreen, TotalPixels, MaxPixels, PixelWidth);

	}
}
void DrawGrid() {
	for (int i = 0; i < 18; i++) {

		//make a line of vertexs with the NDC function
		//get those lines ut in the ParametricLineFunction

		// make a line() using NDC for start and end
		// aka Line(NDCTToScreen(start), NDCToScreen(end)) or aka Line(NDCTToScreen(A), NDCToScreen(B))


		// then use that line in drawline

		VS_WVP(*Grid);

		ParametricLineFunction(Points(NDCtoScreen(Grid[i], PixelWidth, PixelHeight).x1, NDCtoScreen(Grid[i], PixelWidth, PixelHeight).y1,
			NDCtoScreen(Grid[i + 1], PixelWidth, PixelHeight).x1, NDCtoScreen(Grid[i + 1], PixelWidth, PixelHeight).y1),
			ColorWhite, TotalPixels, MaxPixels, PixelWidth);

	}
}

