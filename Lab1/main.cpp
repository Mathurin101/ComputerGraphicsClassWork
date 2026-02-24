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
	Vertex A(NewVert[topLeftF],    0,0);      //top left to bottom Left
	Vertex B(NewVert[bottomLeftF], 0,1);   //bottom left to bottom right
	Vertex C(NewVert[bottomRightF], 1,1);  //bottom right to top left

	Triangle TriOnFront(A, B, C);

	//Top Right triangle F
	//Vertex A2(NewVert[]);   //top left to Top right
	Vertex B2(NewVert[topRightF], 1,0);   //Top right to bottom right
	//Vertex C2(NewVert[]);   //bottom right to top left

	Triangle TriOnFront2(A, B2, C);//NewVert[].z



	//bottom left triangle B  
	Vertex A3(NewVert[topLeftB], 0,0);      //top left to bottom Left     B
	Vertex B3(NewVert[bottomLeftB], 0,1);   //bottom left to bottom right B
	Vertex C3(NewVert[bottomRightB], 1,1);  //bottom right to top left    B

	Triangle TriOnBack3(A3, B3, C3);

	//Top Right triangle B
	//Vertex A4(NewVert[]);       //top left to Top right     B
	Vertex B4(NewVert[topRightB], 1,0);//Top right to bottom right B
	//Vertex C4(NewVert[]);       //bottom right to top left  B

	Triangle TriOnBack4(A3, B4, C3);




	//bottom left triangle Lside
	Vertex A5(NewVert[topLeftB], 0,0);         //top leftB to bottom LeftB         back cube
	Vertex B5(NewVert[bottomLeftB], 0,1);      //bottom leftB to bottom LeftF      Back cube to front 
	Vertex C5(NewVert[bottomLeftF], 1,1);      //bottom LeftF to top leftB   

	Triangle TriOnBack5(A5, B5, C5);


	//Top Right triangle Lside
	Vertex A6(NewVert[topLeftF], 1,0);//TopL to BottomL front
	//Vertex B6(NewVert[]);      //BottomLeftF to TopLeftB 
	//Vertex C6(NewVert[]);      //TopLeftB to TopLeftF

	Triangle TriOnBack6(A6, C5, A5);


	//bottom left triangle Rside
	Vertex A7(NewVert[topRightF], 0,0);     //TopRightF to BottomRightF 
	Vertex B7(NewVert[bottomRightF], 0,1);  //BottomRightF to BottomRightB
	Vertex C7(NewVert[bottomRightB], 1,1);  //BottomRightB to TopRightF

	Triangle TriOnBack7(A7, B7, C7);

	//Top Right triangle Rside
	//Vertex A8(NewVert[topRightF]);   //TopRightF to TopRightB 
	Vertex B8(NewVert[topRightB], 1,0);   //TopRightB to BottomRightB
	//Vertex C8(NewVert[bottomRightB]);   //BottomRightB to TopRightF

	Triangle TriOnBack8(A7, B8, C7);




	//bottom left triangle Topside
	Vertex A9(NewVert[topLeftF], 0,1);     //TopLeftF to TopRightF
	Vertex B9(NewVert[topRightF], 1,1);    //TopRightF to TopRightB
	Vertex C9(NewVert[topRightB], 1,0);	  //TopRightB to TopRightF

	Triangle TriOnBack9(A9, B9, C9);

	//Top Right triangle Topside
	//Vertex A10(NewVert[]);           //TopLeftF to TopRightB
	Vertex B10(NewVert[topLeftB], 0,0);     //TopLeftB to TopRightB 
	//Vertex C10(NewVert[]);	       //TopRightB to TopLeftF

	Triangle TriOnBack10(A9, B10, C9);



	//bottom left triangle BottomSide
	Vertex A11(NewVert[bottomLeftF], 0,1);	     //BottomLeftF to BottomRightF
	Vertex B11(NewVert[bottomRightF], 1,1);	     //BottomRightF to BottomRightB
	Vertex C11(NewVert[bottomRightB] , 1,0);	     //BottomRightB to BottomLeftF

	Triangle TriOnBack11(A11, B11, C11);


	//Top Right triangle bottomSide
	//Vertex A12(NewVert[]);       //BottomLeftF to BottomLeftB
	Vertex B12(NewVert[bottomLeftB], 0,0);	 //BottomLeftB to BottomRightB
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
		
		if (i == 0 || i == 1) {
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

		BetterBruteTriangle(AllTriangles[i], TotalPixels, MaxPixels, DepthBuffer, Color);
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
		Points(NDCtoScreen(NewVertGrid[0] ).x, NDCtoScreen(NewVertGrid[0] ).y,
			NDCtoScreen(NewVertGrid[1] ).x, NDCtoScreen(NewVertGrid[1] ).y),

			Points(NDCtoScreen(NewVertGrid[2] ).x, NDCtoScreen(NewVertGrid[2] ).y,
			NDCtoScreen(NewVertGrid[3] ).x, NDCtoScreen(NewVertGrid[3] ).y),

			//4 5
			Points(NDCtoScreen(NewVertGrid[4] ).x, NDCtoScreen(NewVertGrid[4] ).y,
			NDCtoScreen(NewVertGrid[5] ).x, NDCtoScreen(NewVertGrid[5] ).y),

			//6 7
			Points(NDCtoScreen(NewVertGrid[6] ).x, NDCtoScreen(NewVertGrid[6] ).y,
			NDCtoScreen(NewVertGrid[7] ).x, NDCtoScreen(NewVertGrid[7] ).y),

			//8 9
			Points(NDCtoScreen(NewVertGrid[8] ).x, NDCtoScreen(NewVertGrid[8] ).y,
			NDCtoScreen(NewVertGrid[9] ).x, NDCtoScreen(NewVertGrid[9] ).y),

			//10 11
			Points(NDCtoScreen(NewVertGrid[10] ).x, NDCtoScreen(NewVertGrid[10] ).y,
			NDCtoScreen(NewVertGrid[11] ).x, NDCtoScreen(NewVertGrid[11] ).y),

			//12 13
			Points(NDCtoScreen(NewVertGrid[12] ).x, NDCtoScreen(NewVertGrid[12] ).y,
			NDCtoScreen(NewVertGrid[13] ).x, NDCtoScreen(NewVertGrid[13] ).y),

			//14 15
			Points(NDCtoScreen(NewVertGrid[14] ).x, NDCtoScreen(NewVertGrid[14] ).y,
			NDCtoScreen(NewVertGrid[15] ).x, NDCtoScreen(NewVertGrid[15] ).y),

			//16 17
			Points(NDCtoScreen(NewVertGrid[16] ).x, NDCtoScreen(NewVertGrid[16] ).y,
			NDCtoScreen(NewVertGrid[17] ).x, NDCtoScreen(NewVertGrid[17] ).y),

			//18 19
			Points(NDCtoScreen(NewVertGrid[18] ).x, NDCtoScreen(NewVertGrid[18] ).y,
			NDCtoScreen(NewVertGrid[19] ).x, NDCtoScreen(NewVertGrid[19] ).y),

			//20 21
			Points(NDCtoScreen(NewVertGrid[20] ).x, NDCtoScreen(NewVertGrid[20] ).y,
			NDCtoScreen(NewVertGrid[21] ).x, NDCtoScreen(NewVertGrid[21] ).y),

			//22 23
			Points(NDCtoScreen(NewVertGrid[22] ).x, NDCtoScreen(NewVertGrid[22] ).y,
			NDCtoScreen(NewVertGrid[23] ).x, NDCtoScreen(NewVertGrid[23] ).y),

			//	24 25
			Points(NDCtoScreen(NewVertGrid[24] ).x, NDCtoScreen(NewVertGrid[24] ).y,
			NDCtoScreen(NewVertGrid[25] ).x, NDCtoScreen(NewVertGrid[25] ).y),

			//26 27
			Points(NDCtoScreen(NewVertGrid[26] ).x, NDCtoScreen(NewVertGrid[26] ).y,
			NDCtoScreen(NewVertGrid[27] ).x, NDCtoScreen(NewVertGrid[27] ).y),

			//28 29
			Points(NDCtoScreen(NewVertGrid[28] ).x, NDCtoScreen(NewVertGrid[28] ).y,
			NDCtoScreen(NewVertGrid[29] ).x, NDCtoScreen(NewVertGrid[29] ).y),

			//30 31
			Points(NDCtoScreen(NewVertGrid[30] ).x, NDCtoScreen(NewVertGrid[30] ).y,
			NDCtoScreen(NewVertGrid[31] ).x, NDCtoScreen(NewVertGrid[31] ).y),

			//32 33
			Points(NDCtoScreen(NewVertGrid[32] ).x, NDCtoScreen(NewVertGrid[32] ).y,
			NDCtoScreen(NewVertGrid[33] ).x, NDCtoScreen(NewVertGrid[33] ).y),

			//34 35
			Points(NDCtoScreen(NewVertGrid[34] ).x, NDCtoScreen(NewVertGrid[34] ).y,
			NDCtoScreen(NewVertGrid[35] ).x, NDCtoScreen(NewVertGrid[35] ).y),

			//36 37
			Points(NDCtoScreen(NewVertGrid[36] ).x, NDCtoScreen(NewVertGrid[36] ).y,
			NDCtoScreen(NewVertGrid[37] ).x, NDCtoScreen(NewVertGrid[37] ).y),

			//38 39
			Points(NDCtoScreen(NewVertGrid[38] ).x, NDCtoScreen(NewVertGrid[38] ).y,
			NDCtoScreen(NewVertGrid[39] ).x, NDCtoScreen(NewVertGrid[39] ).y),

			//40 41
			Points(NDCtoScreen(NewVertGrid[40] ).x, NDCtoScreen(NewVertGrid[40] ).y,
			NDCtoScreen(NewVertGrid[41] ).x, NDCtoScreen(NewVertGrid[41] ).y),

			//42 43
			Points(NDCtoScreen(NewVertGrid[42] ).x, NDCtoScreen(NewVertGrid[42] ).y,
			NDCtoScreen(NewVertGrid[43] ).x, NDCtoScreen(NewVertGrid[43] ).y)
	};


	for (int i = 0; i < 22; i++) {
		
		ParametricLineFunction(ArrayPointGrids[i], ColorWhite, TotalPixels, MaxPixels, PixelWidth, DepthBuffer);
	}
}

