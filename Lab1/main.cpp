#include "RasterSurface.h"
#include "Helper.h"
#include "XTime.h"
#include "tiles_12.h"
#include "teleporter_hit.h"
#include "Shaders.h"

#include <random> //random number class "std::rand()"
#include <iostream>

const char* Name = "MathurinGenty_Line Drawing";
const unsigned int PixelWidth = 600;
const unsigned int PixelHeight = 500;

const unsigned int MaxPixels = PixelWidth * PixelHeight;

unsigned int TotalPixels[MaxPixels];

enum FrontCubeE {
	topLeftF,
	topRightF,
	bottomRightF,
	bottomLeftF,
	topLeftB,
	topRightB,
	bottomRightB,
	bottomLeftB
};


Vertex FrontCube[8] = {
	// -x = left, +x = right
	// -y = down, +y = up
	// -z = near screen, +z = far screen

	//front square
			//  x,      y,     z 
	Vertex(-0.25f,  0.25f, -0.25f),//   top left  = 0
	Vertex(0.25f,  0.25f, -0.25f),//   top right = 1
	Vertex(0.25f, -0.25f, -0.25f),//bottom right = 2
	Vertex(-0.25f, -0.25f, -0.25f),//bottom left  = 3

	//back square
	Vertex(-0.25f,  0.25f, 0.25f),//   top left  = 4
	Vertex(0.25f,  0.25f, 0.25f),//   top right = 5
	Vertex(0.25f, -0.25f, 0.25f),//bottom right = 6
	Vertex(-0.25f, -0.25f, 0.25f),//bottom left  = 7
};

Vertex Grid[44] = {

	//top to bottom grid
			//  x, y,     z 
	Vertex(-0.50f, 0,  0.50f),//upBorderL   = 0
	Vertex(-0.50f, 0, -0.50f),//downBorderL = 1

	Vertex(-0.40f, 0,  0.50f),//upLL        = 2
	Vertex(-0.40f, 0, -0.50f),//downLL      = 3

	Vertex(-0.30f, 0,  0.50f),//upL         = 4
	Vertex(-0.30f, 0, -0.50f),//downL       = 5

	Vertex(-0.20f, 0,  0.50f),//upLM        = 6
	Vertex(-0.20f, 0, -0.50f),//downLM      = 7

	Vertex(-0.10f, 0,  0.50f),//upLM        = 8
	Vertex(-0.10f, 0, -0.50f),//downLM      = 9


	Vertex(0.10f, 0,  0.50f),//upLM		    = 10
	Vertex(0.10f, 0, -0.50f),//downLM	    = 11

	Vertex(0.20f, 0,  0.50f),//upRM         = 12
	Vertex(0.20f, 0, -0.50f),//downRM       = 13

	Vertex(0.30f, 0,  0.50f),//upR			= 14
	Vertex(0.30f, 0, -0.50f),//downR        = 15

	Vertex(0.40f, 0,  0.50f),//upRR         = 16
	Vertex(0.40f, 0, -0.50f),//downRR       = 17

	Vertex(0.50f, 0,  0.50f),//upBorderR	= 18
	Vertex(0.50f, 0, -0.50f),//downBorderR  = 19




	//side to side grind
			  //x, y,     z
	Vertex(-0.50f, 0, 0.50f),//leftTop     = 20
	Vertex(0.50f, 0, 0.50f),//rightTop     = 21

	Vertex(-0.50f, 0, 0.40f),//leftTT	   = 22
	Vertex(0.50f, 0, 0.40f),//rightTT      = 23

	Vertex(-0.50f, 0, 0.30f),//leftT       = 24
	Vertex(0.50f, 0, 0.30f),//rightT       = 25

	Vertex(-0.50f, 0, 0.20f),//leftTM      = 26
	Vertex(0.50f, 0, 0.20f),//rightTM	   = 27

	Vertex(-0.50f, 0, 0.10f),//leftTMM     = 28 
	Vertex(0.50f, 0, 0.10f),//rightTMM     = 29

	Vertex(-0.50f, 0, 0.0),//leftM		   = 30
	Vertex(0.50f, 0, 0.0),//rightM         = 31

	Vertex(-0.50f, 0, -0.10f),//leftBMM    = 32
	Vertex(0.50f, 0, -0.10f),//rightBMM    = 33

	Vertex(-0.50f, 0, -0.20f),//leftBM	   = 34
	Vertex(0.50f, 0, -0.20f),//rightBM     = 35

	Vertex(-0.50f, 0, -0.30f),//leftB	   = 36
	Vertex(0.50f, 0, -0.30f),//rightB	   = 37

	Vertex(-0.50f, 0, -0.40f),//leftBB	   = 38
	Vertex(0.50f, 0, -0.40f),//rightBB     = 39

	Vertex(-0.50f, 0, -0.50f),//leftBottom = 40
	Vertex(0.50f, 0, -0.50f),//rightBottom = 41	

	//middle up and down lines
	Vertex(0, 0,  0.50f),//upLM        = 42
	Vertex(0, 0, -0.50f),//downLM      = 43
};


void DrawCube();
void DrawGrid();


PColor ColorGreen(0xFF123524);//green
PColor ColorWhite(0xFFFFFFFF);//white



PColor LightBlue(0xFFADD8E6);

int main()
{
	XTime Time;
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
	Matrix4x4 view = MultiplyMatrixByMatrix(TranslationMatrix(0, 0, -1), RotateX(-18));
	Matrix4x4 Projection = PerspectiveProjection(90, (float)PixelHeight  / PixelWidth, 0.1f, 10);
	view = OrthonormalInverse(view);
	VS_View = view;
	VS_Projection = Projection;
	//will print on the screen
	
	Points A(100, 300, 100, 450);
	Points B(100, 450, 300, 200);
	Points C(300, 200, 100, 300);


	Triangle Green(A,B,C);
	
	
	do {
		/*
		CCBuffer(0xFF000000, TotalPixels, MaxPixels);

		VS_World = GridWorld;
		DrawGrid();

		Time.Signal();
		CubeWorld = MultiplyMatrixByMatrix(CubeWorld, RotateY(45.0f * Time.Delta()));

		VS_World = CubeWorld;
		DrawCube();
*/
		

		ParametricLineFunction(Green.LineABC[0], LightBlue, TotalPixels, MaxPixels, PixelWidth);
		ParametricLineFunction(Green.LineABC[1], LightBlue, TotalPixels, MaxPixels, PixelWidth);
		ParametricLineFunction(Green.LineABC[2], LightBlue, TotalPixels, MaxPixels, PixelWidth);

		//BruteTriangle(Green, TotalPixels, MaxPixels, PixelWidth, PixelHeight);
		BetterBruteTriangle(Green, TotalPixels, MaxPixels, PixelWidth);


	} while (RS_Update(TotalPixels, MaxPixels));

	RS_Shutdown();

	return 0;
}

void DrawCube() {

	//made a copy so the cube doesn't fly high
	Vertex NewVert[12];

	for (int i = 0; i < 8; i++) {
		NewVert[i] = FrontCube[i];
		VS_WVP(NewVert[i]);
	}

	/**/
	Points ArrayPoints[12] = {

		//front top                   left     to         right

		Points(NDCtoScreen(NewVert[0], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[0], PixelWidth, PixelWidth).y1,
			NDCtoScreen(NewVert[1], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[1], PixelWidth, PixelWidth).y1),

			//top                   right    to         bottom right
			Points(NDCtoScreen(NewVert[1], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[1], PixelWidth, PixelWidth).y1,
				NDCtoScreen(NewVert[2], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[2], PixelWidth, PixelWidth).y1),

				//top                   left    to         bottom left
				Points(NDCtoScreen(NewVert[0], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[0], PixelWidth, PixelWidth).y1,
					NDCtoScreen(NewVert[3], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[3], PixelWidth, PixelWidth).y1),

					//bottom                   left    to         right
					Points(NDCtoScreen(NewVert[3], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[3], PixelWidth, PixelWidth).y1,
						NDCtoScreen(NewVert[2], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[2], PixelWidth, PixelWidth).y1),


						//back top                   left     to         right
						Points(NDCtoScreen(NewVert[4], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[4], PixelWidth, PixelWidth).y1,
							NDCtoScreen(NewVert[5], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[5], PixelWidth, PixelWidth).y1),

							//top                   right    to         bottom right
							Points(NDCtoScreen(NewVert[5], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[5], PixelWidth, PixelWidth).y1,
								NDCtoScreen(NewVert[6], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[6], PixelWidth, PixelWidth).y1),

								//top                   left    to         bottom left
								Points(NDCtoScreen(NewVert[4], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[4], PixelWidth, PixelWidth).y1,
									NDCtoScreen(NewVert[7], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[7], PixelWidth, PixelWidth).y1),

									//bottom                   left    to         right
									Points(NDCtoScreen(NewVert[7], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[7], PixelWidth, PixelWidth).y1,
										NDCtoScreen(NewVert[6], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[6], PixelWidth, PixelWidth).y1),


										//connecting top left cornners
										Points(NDCtoScreen(NewVert[0], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[0], PixelWidth, PixelWidth).y1,
											NDCtoScreen(NewVert[4], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[4], PixelWidth, PixelWidth).y1),

											//connecting top right cornners
											Points(NDCtoScreen(NewVert[1], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[1], PixelWidth, PixelWidth).y1,
												NDCtoScreen(NewVert[5], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[5], PixelWidth, PixelWidth).y1),

												//connecting bottom left cornners
												Points(NDCtoScreen(NewVert[3], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[3], PixelWidth, PixelWidth).y1,
													NDCtoScreen(NewVert[7], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[7], PixelWidth, PixelWidth).y1),

													//connecting bottom right cornners
													Points(NDCtoScreen(NewVert[2], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[2], PixelWidth, PixelWidth).y1,
														NDCtoScreen(NewVert[6], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[6], PixelWidth, PixelWidth).y1),
	};


	for (int i = 0; i < 12; i++) {
		ParametricLineFunction(ArrayPoints[i], LightBlue, TotalPixels, MaxPixels, PixelWidth);
	}

}
void DrawGrid() {
	//made a copy
	Vertex NewVertGrid[44];

	for (int i = 0; i < 44; i++) {
		NewVertGrid[i] = Grid[i];
		VS_WVP(NewVertGrid[i]);
	}


	//43 all together
	Points ArrayPointGrids[22] = {
		Points(NDCtoScreen(NewVertGrid[0], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[0], PixelWidth, PixelWidth).y1,
			NDCtoScreen(NewVertGrid[1], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[1], PixelWidth, PixelWidth).y1),

			Points(NDCtoScreen(NewVertGrid[2], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[2], PixelWidth, PixelWidth).y1,
			NDCtoScreen(NewVertGrid[3], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[3], PixelWidth, PixelWidth).y1),

			//4 5
			Points(NDCtoScreen(NewVertGrid[4], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[4], PixelWidth, PixelWidth).y1,
			NDCtoScreen(NewVertGrid[5], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[5], PixelWidth, PixelWidth).y1),

			//6 7
			Points(NDCtoScreen(NewVertGrid[6], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[6], PixelWidth, PixelWidth).y1,
			NDCtoScreen(NewVertGrid[7], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[7], PixelWidth, PixelWidth).y1),

			//8 9
			Points(NDCtoScreen(NewVertGrid[8], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[8], PixelWidth, PixelWidth).y1,
			NDCtoScreen(NewVertGrid[9], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[9], PixelWidth, PixelWidth).y1),

			//10 11
			Points(NDCtoScreen(NewVertGrid[10], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[10], PixelWidth, PixelWidth).y1,
			NDCtoScreen(NewVertGrid[11], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[11], PixelWidth, PixelWidth).y1),

			//12 13
			Points(NDCtoScreen(NewVertGrid[12], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[12], PixelWidth, PixelWidth).y1,
			NDCtoScreen(NewVertGrid[13], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[13], PixelWidth, PixelWidth).y1),

			//14 15
			Points(NDCtoScreen(NewVertGrid[14], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[14], PixelWidth, PixelWidth).y1,
			NDCtoScreen(NewVertGrid[15], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[15], PixelWidth, PixelWidth).y1),

			//16 17
			Points(NDCtoScreen(NewVertGrid[16], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[16], PixelWidth, PixelWidth).y1,
			NDCtoScreen(NewVertGrid[17], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[17], PixelWidth, PixelWidth).y1),

			//18 19
			Points(NDCtoScreen(NewVertGrid[18], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[18], PixelWidth, PixelWidth).y1,
			NDCtoScreen(NewVertGrid[19], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[19], PixelWidth, PixelWidth).y1),

			//20 21
			Points(NDCtoScreen(NewVertGrid[20], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[20], PixelWidth, PixelWidth).y1,
			NDCtoScreen(NewVertGrid[21], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[21], PixelWidth, PixelWidth).y1),

			//22 23
			Points(NDCtoScreen(NewVertGrid[22], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[22], PixelWidth, PixelWidth).y1,
			NDCtoScreen(NewVertGrid[23], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[23], PixelWidth, PixelWidth).y1),

			//	24 25
			Points(NDCtoScreen(NewVertGrid[24], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[24], PixelWidth, PixelWidth).y1,
			NDCtoScreen(NewVertGrid[25], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[25], PixelWidth, PixelWidth).y1),

			//26 27
			Points(NDCtoScreen(NewVertGrid[26], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[26], PixelWidth, PixelWidth).y1,
			NDCtoScreen(NewVertGrid[27], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[27], PixelWidth, PixelWidth).y1),

			//28 29
			Points(NDCtoScreen(NewVertGrid[28], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[28], PixelWidth, PixelWidth).y1,
			NDCtoScreen(NewVertGrid[29], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[29], PixelWidth, PixelWidth).y1),

			//30 31
			Points(NDCtoScreen(NewVertGrid[30], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[30], PixelWidth, PixelWidth).y1,
			NDCtoScreen(NewVertGrid[31], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[31], PixelWidth, PixelWidth).y1),

			//32 33
			Points(NDCtoScreen(NewVertGrid[32], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[32], PixelWidth, PixelWidth).y1,
			NDCtoScreen(NewVertGrid[33], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[33], PixelWidth, PixelWidth).y1),

			//34 35
			Points(NDCtoScreen(NewVertGrid[34], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[34], PixelWidth, PixelWidth).y1,
			NDCtoScreen(NewVertGrid[35], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[35], PixelWidth, PixelWidth).y1),

			//36 37
			Points(NDCtoScreen(NewVertGrid[36], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[36], PixelWidth, PixelWidth).y1,
			NDCtoScreen(NewVertGrid[37], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[37], PixelWidth, PixelWidth).y1),

			//38 39
			Points(NDCtoScreen(NewVertGrid[38], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[38], PixelWidth, PixelWidth).y1,
			NDCtoScreen(NewVertGrid[39], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[39], PixelWidth, PixelWidth).y1),

			//40 41
			Points(NDCtoScreen(NewVertGrid[40], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[40], PixelWidth, PixelWidth).y1,
			NDCtoScreen(NewVertGrid[41], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[41], PixelWidth, PixelWidth).y1),

			//42 43
			Points(NDCtoScreen(NewVertGrid[42], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[42], PixelWidth, PixelWidth).y1,
			NDCtoScreen(NewVertGrid[43], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVertGrid[43], PixelWidth, PixelWidth).y1)
	};


	for (int i = 0; i < 22; i++) {

		ParametricLineFunction(ArrayPointGrids[i], ColorWhite, TotalPixels, MaxPixels, PixelWidth);
	}
}

