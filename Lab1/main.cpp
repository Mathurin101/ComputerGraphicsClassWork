#include "RasterSurface.h"
#include "Helper.h"
#include "XTime.h"
#include "tiles_12.h"
#include "teleporter_hit.h"
#include "Shaders.h"

#include <random> //random number class "std::rand()"
#include <iostream>

const char* Name = "MathurinGenty_Line Drawing";








enum FrontCubeE {
	topLeftF,	 //0
	topRightF,   //1
	bottomRightF,//2
	bottomLeftF, //3

	topLeftB,    //4
	topRightB,   //5
	bottomRightB,//6 
	bottomLeftB  //7
};

enum CubeE {
	//front
	TopL2TopRFront,
	TopR2BottomRFront,
	TopL2BottomLFront,
	BottomL2BottomRFront,

	//back
	TopL2TopRBack,
	TopR2BottomRBack,
	TopL2BottomLBack,
	BottomL2BottomRBack,

	//Connecting Corners
	TopLeftcorner,    //Lside top    --- TopL Front to TopL back
	TopRightcorner,   //Rside top    --- TopR Front to TopR back
	BottomLeftcorner, //Lside bottom --- BottomL Front to bottomL back
	BottomRightcorner //Rside bottom --- BottomR Front to bottomR back
};

Vertex FrontCube[8] = {
	// -x = left, +x = right
	// -y = down, +y = up
	// -z = near screen, +z = far screen

	//front square
			//  x,      y,     z 
	Vertex(-0.25f,  0.25f, -0.25f),//   top left  = 0
	Vertex(0.25f,  0.25f, -0.25f),//   top right  = 1
	Vertex(0.25f, -0.25f, -0.25f),//bottom right  = 2
	Vertex(-0.25f, -0.25f, -0.25f),//bottom left  = 3

	//back square
	Vertex(-0.25f,  0.25f, 0.25f),//   top left  = 4
	Vertex(0.25f,  0.25f, 0.25f),//   top right  = 5
	Vertex(0.25f, -0.25f, 0.25f),//bottom right  = 6
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
	Matrix4x4 view = MultiplyMatrixByMatrix(TranslationMatrix(0, 0, -1), RotateX(-18));//
	Matrix4x4 Projection = PerspectiveProjection(90, (float)PixelHeight / PixelWidth, 0.1f, 10);
	view = OrthonormalInverse(view);
	VS_View = view;
	VS_Projection = Projection;
	

	
	
	//will print on the screen
	do {

		CCBuffer(0xFF000000, TotalPixels, MaxPixels, DepthBuffer);

		VS_World = GridWorld;
		DrawGrid();

		Time.Signal();
		CubeWorld = MultiplyMatrixByMatrix(CubeWorld, RotateY(45.0f * Time.Delta()));

		VS_World = CubeWorld;
		DrawCube();




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

	//bottom left triangle F
	Vertex A(NewVert[topLeftF]);      //top left to bottom Left
	Vertex B(NewVert[bottomLeftF]);   //bottom left to bottom right
	Vertex C(NewVert[bottomRightF]);  //bottom right to top left

	Triangle TriOnFront(A, B, C);

	//Top Right triangle F
	//Vertex A2(NewVert[]);   //top left to Top right
	Vertex B2(NewVert[topRightF]);   //Top right to bottom right
	//Vertex C2(NewVert[]);   //bottom right to top left

	Triangle TriOnFront2(A, B2, C);//NewVert[].z



	//bottom left triangle B  
	Vertex A3(NewVert[topLeftB]);      //top left to bottom Left     B
	Vertex B3(NewVert[bottomLeftB]);   //bottom left to bottom right B
	Vertex C3(NewVert[bottomRightB]);  //bottom right to top left    B

	Triangle TriOnBack3(A3, B3, C3);

	//Top Right triangle B
	//Vertex A4(NewVert[]);       //top left to Top right     B
	Vertex B4(NewVert[topRightB]);//Top right to bottom right B
	//Vertex C4(NewVert[]);       //bottom right to top left  B

	Triangle TriOnBack4(A3, B4, C3);




	//bottom left triangle Lside
	Vertex A5(NewVert[topLeftB]);         //top leftB to bottom LeftB         back cube
	Vertex B5(NewVert[bottomLeftB]);      //bottom leftB to bottom LeftF      Back cube to front 
	Vertex C5(NewVert[bottomLeftF]);      //bottom LeftF to top leftB   

	Triangle TriOnBack5(A5, B5, C5);


	//Top Right triangle Lside
	Vertex A6(NewVert[topLeftF]);//TopL to BottomL front
	//Vertex B6(NewVert[]);      //BottomLeftF to TopLeftB 
	//Vertex C6(NewVert[]);      //TopLeftB to TopLeftF

	Triangle TriOnBack6(A6, C5, A5);


	//bottom left triangle Rside
	Vertex A7(NewVert[topRightF]);     //TopRightF to BottomRightF 
	Vertex B7(NewVert[bottomRightF]);  //BottomRightF to BottomRightB
	Vertex C7(NewVert[bottomRightB]);  //BottomRightB to TopRightF

	Triangle TriOnBack7(A7, B7, C7);

	//Top Right triangle Rside
	//Vertex A8(NewVert[topRightF]);   //TopRightF to TopRightB 
	Vertex B8(NewVert[topRightB]);   //TopRightB to BottomRightB
	//Vertex C8(NewVert[bottomRightB]);   //BottomRightB to TopRightF

	Triangle TriOnBack8(A7, B8, C7);




	//bottom left triangle Topside
	Vertex A9(NewVert[topLeftF]);     //TopLeftF to TopRightF
	Vertex B9(NewVert[topRightF]);    //TopRightF to TopRightB
	Vertex C9(NewVert[topRightB]);	  //TopRightB to TopRightF

	Triangle TriOnBack9(A9, B9, C9);

	//Top Right triangle Topside
	//Vertex A10(NewVert[]);           //TopLeftF to TopRightB
	Vertex B10(NewVert[topLeftB]);     //TopLeftB to TopRightB 
	//Vertex C10(NewVert[]);	       //TopRightB to TopLeftF

	Triangle TriOnBack10(A9, B10, C9);



	//bottom left triangle BottomSide
	Vertex A11(NewVert[bottomLeftF]);	     //BottomLeftF to BottomRightF
	Vertex B11(NewVert[bottomRightF]);	     //BottomRightF to BottomRightB
	Vertex C11(NewVert[bottomRightB]);	     //BottomRightB to BottomLeftF

	Triangle TriOnBack11(A11, B11, C11);


	//Top Right triangle bottomSide
	//Vertex A12(NewVert[]);       //BottomLeftF to BottomLeftB
	Vertex B12(NewVert[bottomLeftB]);	 //BottomLeftB to BottomRightB
	//Vertex C12(NewVert[]);	     //BottomRightB to BottomLeftF

	Triangle TriOnBack12(A11, B12, C11);


	Triangle AllTriangles[12] = {
	TriOnFront, //0
	TriOnFront2,//1

	TriOnBack3, //2 
	TriOnBack4, //3 
	
	TriOnBack5, //4
	TriOnBack6, //5 
	
	TriOnBack7, //6
	TriOnBack8, //7
	
	TriOnBack9, //8
	TriOnBack10,//9
	
	TriOnBack11,//10
	TriOnBack12 //11
	};

	PColor Color;

	for (int i = 0; i < 12; i++) {
		
		if (i == 0 || i == 1) {//
			Color = 0xFF5b3a80; //purple dizanezodifnawe
		}
		else if (i == 2 || i == 3) {
			Color = 0xFFADD8E6; //Light Blue
		}
		else if (i == 4 || i == 5) {
			Color = 0x33FFD3D6; //0xFFFFD3D6; //light pink
		}
		else if (i == 6 || i == 7) {
			Color = 0xFF88E788; //light green
		}
		else if (i == 8 || i == 9) {
			Color = 0xFFEAA221; //Marigold
		}
		else if (i == 10 || i == 11) {
			Color = 0xFF123524; //Phthalo
		}

		BetterBruteTriangle(AllTriangles[i], TotalPixels, MaxPixels, DepthBuffer);
	}

	//outline
	//for (int i = 0; i < 12; i++) {
	//	ParametricLineFunction(ArrayPoints[i], LightBlue, TotalPixels, MaxPixels, PixelWidth);
	//}

	//used to debug the triangles
	//ParametricLineFunction(AllTriangles[2].LineABC[0], LightBlue, TotalPixels, MaxPixels, PixelWidth);
	//ParametricLineFunction(AllTriangles[2].LineABC[1], LightBlue, TotalPixels, MaxPixels, PixelWidth);
	//ParametricLineFunction(AllTriangles[2].LineABC[2], LightBlue, TotalPixels, MaxPixels, PixelWidth);
	//   BetterBruteTriangle(AllTriangles[2], TotalPixels, MaxPixels, PixelWidth, PColor(0xFF5b3a80));
	//
	//ParametricLineFunction(AllTriangles[3].LineABC[0], LightBlue, TotalPixels, MaxPixels, PixelWidth);
	//ParametricLineFunction(AllTriangles[3].LineABC[1], LightBlue, TotalPixels, MaxPixels, PixelWidth);
	//ParametricLineFunction(AllTriangles[3].LineABC[2], LightBlue, TotalPixels, MaxPixels, PixelWidth);
	//   BetterBruteTriangle(AllTriangles[3], TotalPixels, MaxPixels, PixelWidth, PColor(0xFF5b3a80));
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
		Points(NDCtoScreen(NewVertGrid[0], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[0], PixelWidth, PixelWidth).y,
			NDCtoScreen(NewVertGrid[1], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[1], PixelWidth, PixelWidth).y),

			Points(NDCtoScreen(NewVertGrid[2], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[2], PixelWidth, PixelWidth).y,
			NDCtoScreen(NewVertGrid[3], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[3], PixelWidth, PixelWidth).y),

			//4 5
			Points(NDCtoScreen(NewVertGrid[4], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[4], PixelWidth, PixelWidth).y,
			NDCtoScreen(NewVertGrid[5], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[5], PixelWidth, PixelWidth).y),

			//6 7
			Points(NDCtoScreen(NewVertGrid[6], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[6], PixelWidth, PixelWidth).y,
			NDCtoScreen(NewVertGrid[7], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[7], PixelWidth, PixelWidth).y),

			//8 9
			Points(NDCtoScreen(NewVertGrid[8], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[8], PixelWidth, PixelWidth).y,
			NDCtoScreen(NewVertGrid[9], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[9], PixelWidth, PixelWidth).y),

			//10 11
			Points(NDCtoScreen(NewVertGrid[10], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[10], PixelWidth, PixelWidth).y,
			NDCtoScreen(NewVertGrid[11], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[11], PixelWidth, PixelWidth).y),

			//12 13
			Points(NDCtoScreen(NewVertGrid[12], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[12], PixelWidth, PixelWidth).y,
			NDCtoScreen(NewVertGrid[13], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[13], PixelWidth, PixelWidth).y),

			//14 15
			Points(NDCtoScreen(NewVertGrid[14], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[14], PixelWidth, PixelWidth).y,
			NDCtoScreen(NewVertGrid[15], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[15], PixelWidth, PixelWidth).y),

			//16 17
			Points(NDCtoScreen(NewVertGrid[16], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[16], PixelWidth, PixelWidth).y,
			NDCtoScreen(NewVertGrid[17], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[17], PixelWidth, PixelWidth).y),

			//18 19
			Points(NDCtoScreen(NewVertGrid[18], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[18], PixelWidth, PixelWidth).y,
			NDCtoScreen(NewVertGrid[19], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[19], PixelWidth, PixelWidth).y),

			//20 21
			Points(NDCtoScreen(NewVertGrid[20], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[20], PixelWidth, PixelWidth).y,
			NDCtoScreen(NewVertGrid[21], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[21], PixelWidth, PixelWidth).y),

			//22 23
			Points(NDCtoScreen(NewVertGrid[22], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[22], PixelWidth, PixelWidth).y,
			NDCtoScreen(NewVertGrid[23], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[23], PixelWidth, PixelWidth).y),

			//	24 25
			Points(NDCtoScreen(NewVertGrid[24], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[24], PixelWidth, PixelWidth).y,
			NDCtoScreen(NewVertGrid[25], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[25], PixelWidth, PixelWidth).y),

			//26 27
			Points(NDCtoScreen(NewVertGrid[26], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[26], PixelWidth, PixelWidth).y,
			NDCtoScreen(NewVertGrid[27], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[27], PixelWidth, PixelWidth).y),

			//28 29
			Points(NDCtoScreen(NewVertGrid[28], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[28], PixelWidth, PixelWidth).y,
			NDCtoScreen(NewVertGrid[29], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[29], PixelWidth, PixelWidth).y),

			//30 31
			Points(NDCtoScreen(NewVertGrid[30], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[30], PixelWidth, PixelWidth).y,
			NDCtoScreen(NewVertGrid[31], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[31], PixelWidth, PixelWidth).y),

			//32 33
			Points(NDCtoScreen(NewVertGrid[32], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[32], PixelWidth, PixelWidth).y,
			NDCtoScreen(NewVertGrid[33], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[33], PixelWidth, PixelWidth).y),

			//34 35
			Points(NDCtoScreen(NewVertGrid[34], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[34], PixelWidth, PixelWidth).y,
			NDCtoScreen(NewVertGrid[35], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[35], PixelWidth, PixelWidth).y),

			//36 37
			Points(NDCtoScreen(NewVertGrid[36], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[36], PixelWidth, PixelWidth).y,
			NDCtoScreen(NewVertGrid[37], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[37], PixelWidth, PixelWidth).y),

			//38 39
			Points(NDCtoScreen(NewVertGrid[38], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[38], PixelWidth, PixelWidth).y,
			NDCtoScreen(NewVertGrid[39], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[39], PixelWidth, PixelWidth).y),

			//40 41
			Points(NDCtoScreen(NewVertGrid[40], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[40], PixelWidth, PixelWidth).y,
			NDCtoScreen(NewVertGrid[41], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[41], PixelWidth, PixelWidth).y),

			//42 43
			Points(NDCtoScreen(NewVertGrid[42], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[42], PixelWidth, PixelWidth).y,
			NDCtoScreen(NewVertGrid[43], PixelWidth, PixelWidth).x, NDCtoScreen(NewVertGrid[43], PixelWidth, PixelWidth).y)
	};


	for (int i = 0; i < 22; i++) {
		
		ParametricLineFunction(ArrayPointGrids[i], ColorWhite, TotalPixels, MaxPixels, PixelWidth, DepthBuffer);
	}
}

