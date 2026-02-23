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

	//Connecting Cornners
	TopLeftCornners,    //Lside top    --- TopL Front to TopL back
	TopRightCornners,   //Rside top    --- TopR Front to TopR back
	BottomLeftCornners, //Lside bottom --- BottomL Front to bottomL back
	BottomRightCornners //Rside bottom --- BottomR Front to bottomR back
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
	Matrix4x4 view = MultiplyMatrixByMatrix(TranslationMatrix(0, 0, -1), RotateX(-18));
	Matrix4x4 Projection = PerspectiveProjection(90, (float)PixelHeight / PixelWidth, 0.1f, 10);
	view = OrthonormalInverse(view);
	VS_View = view;
	VS_Projection = Projection;
	//will print on the screen

	Points A(100, 300, 100, 450);
	Points B(100, 450, 300, 200);
	Points C(300, 200, 100, 300);


	Triangle Green(A, B, C);

	//put in do while loop to see a triangle
	//ParametricLineFunction(Green.LineABC[0], LightBlue, TotalPixels, MaxPixels, PixelWidth);
	//ParametricLineFunction(Green.LineABC[1], LightBlue, TotalPixels, MaxPixels, PixelWidth);
	//ParametricLineFunction(Green.LineABC[2], LightBlue, TotalPixels, MaxPixels, PixelWidth);
	//BruteTriangle(Green, TotalPixels, MaxPixels, PixelWidth, PixelHeight);
	//BetterBruteTriangle(Green, TotalPixels, MaxPixels, PixelWidth);

	do {

		CCBuffer(0xFF000000, TotalPixels, MaxPixels);

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

	Points ArrayPoints[12] = {

		//front top                   left     to         right
		Points(NDCtoScreen(NewVert[topLeftF], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[topLeftF], PixelWidth, PixelWidth).y1,
			NDCtoScreen(NewVert[topRightF], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[topRightF], PixelWidth, PixelWidth).y1),

		//top                   right    to         bottom right
		Points(NDCtoScreen(NewVert[topRightF], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[topRightF], PixelWidth, PixelWidth).y1,
			NDCtoScreen(NewVert[bottomRightF], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[bottomRightF], PixelWidth, PixelWidth).y1),

		//top                   left    to         bottom left
		Points(NDCtoScreen(NewVert[topLeftF], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[topLeftF], PixelWidth, PixelWidth).y1,
			NDCtoScreen(NewVert[bottomLeftF], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[bottomLeftF], PixelWidth, PixelWidth).y1),

		//bottom                   left    to         right
		Points(NDCtoScreen(NewVert[bottomLeftF], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[bottomLeftF], PixelWidth, PixelWidth).y1,
			NDCtoScreen(NewVert[bottomRightF], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[bottomRightF], PixelWidth, PixelWidth).y1),


		//back top                   left     to         right
		Points(NDCtoScreen(NewVert[topLeftB], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[topLeftB], PixelWidth, PixelWidth).y1,
			NDCtoScreen(NewVert[topRightB], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[topRightB], PixelWidth, PixelWidth).y1),

		//top                   right    to         bottom right
		Points(NDCtoScreen(NewVert[topRightB], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[topRightB], PixelWidth, PixelWidth).y1,
			NDCtoScreen(NewVert[bottomRightB], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[bottomRightB], PixelWidth, PixelWidth).y1),

		//top                   left    to         bottom left
		Points(NDCtoScreen(NewVert[topLeftB], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[topLeftB], PixelWidth, PixelWidth).y1,
				NDCtoScreen(NewVert[bottomLeftB], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[bottomLeftB], PixelWidth, PixelWidth).y1),

		//bottom                   left    to         right
			Points(NDCtoScreen(NewVert[bottomLeftB], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[bottomLeftB], PixelWidth, PixelWidth).y1,
				NDCtoScreen(NewVert[bottomRightB], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[bottomRightB], PixelWidth, PixelWidth).y1),


					//connecting top left cornners
					Points(NDCtoScreen(NewVert[topLeftF], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[topLeftF], PixelWidth, PixelWidth).y1,
						NDCtoScreen(NewVert[topLeftB], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[topLeftB], PixelWidth, PixelWidth).y1),

					//connecting top right cornners
					Points(NDCtoScreen(NewVert[topRightF], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[topRightF], PixelWidth, PixelWidth).y1,
						NDCtoScreen(NewVert[topRightB], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[topRightB], PixelWidth, PixelWidth).y1),

					//connecting bottom left cornners
					Points(NDCtoScreen(NewVert[bottomLeftF], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[bottomLeftF], PixelWidth, PixelWidth).y1,
						NDCtoScreen(NewVert[bottomLeftB], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[bottomLeftB], PixelWidth, PixelWidth).y1),

					//connecting bottom right cornners
							Points(NDCtoScreen(NewVert[bottomRightF], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[bottomRightF], PixelWidth, PixelWidth).y1,
											NDCtoScreen(NewVert[bottomRightB], PixelWidth, PixelWidth).x1, NDCtoScreen(NewVert[bottomRightB], PixelWidth, PixelWidth).y1),
	};

	//bottom left triangle F
	Points A(ArrayPoints[TopL2BottomLFront]);   //top left to bottom Left
	Points B(ArrayPoints[BottomL2BottomRFront]);//bottom left to bottom right
	Points C(B.x2, B.y2, A.x1, A.y1);           //bottom right to top left

	Triangle TriOnFront(A, B, C);

	//Top Right triangle F
	Points A2(ArrayPoints[TopL2TopRFront]);   //top left to Top right
	Points B2(ArrayPoints[TopR2BottomRFront]);//Top right to bottom right
	Points C2(C);                             //bottom right to top left

	Triangle TriOnFront2(A2, B2, C2);



	//bottom left triangle B  
	Points A3(ArrayPoints[TopL2BottomLBack]);   //top left to bottom Left     B
	Points B3(ArrayPoints[BottomL2BottomRBack]);//bottom left to bottom right B
	Points C3(B3.x1, B3.y1, A3.x1, A3.y1);      //bottom right to top left    B

	Triangle TriOnBack3(A3, B3, C3);

	//Top Right triangle B
	Points A4(ArrayPoints[TopL2TopRBack]);//top left to Top right B
	Points B4(ArrayPoints[TopR2BottomRBack]);//Top right to bottom right B
	Points C4(A4.x1, A4.y1, B4.x1, B4.y1);//bottom right to top left   B

	Triangle TriOnBack4(A4, B4, C4);




	//bottom left triangle Lside
	Points A5(ArrayPoints[TopL2BottomLBack]);   //top leftB to bottom LeftB         back cube
	Points B5(ArrayPoints[BottomLeftCornners]); //bottom leftB to bottom LeftF      Back cube to front 
	Points C5(B5.x2, B5.y2, A5.x1, A5.y1);      //bottom LeftF to top leftB   

	Triangle TriOnBack5(A5, B5, C5);


	//Top Right triangle Lside
	Points A6(ArrayPoints[TopL2BottomLFront]);  //TopL to BottomL front
	Points B6(ArrayPoints[TopLeftCornners]);    //TopLeftF to TopLeftB 
	Points C6(B6.x2, B6.y2, A6.x2, A6.y2);      //TopLeftB to BottomLeftF

	Triangle TriOnBack6(A6, B6, C6);


	//bottom left triangle Rside
	Points A7(ArrayPoints[TopR2BottomRFront]);    //TopRightF to BottomRightF 
	Points B7(ArrayPoints[BottomRightCornners]);  //BottomRightF to BottomRightB
	Points C7(B7.x2, B7.y2, A7.x2, A7.y2);        //BottomRightB to TopRightF

	Triangle TriOnBack7(A7, B7, C7);

	//Top Right triangle Rside
	Points A8(ArrayPoints[TopRightCornners]);   //TopRightF to TopRightB 
	Points B8(ArrayPoints[TopR2BottomRBack]);   //TopRightB to BottomRightB
	Points C8(B8.x2, B8.y2, A8.x1, A8.y1);      //BottomRightB to TopRightF

	Triangle TriOnBack8(A8, B8, C8);



	//bottom left triangle Topside
	Points A9(ArrayPoints[TopL2TopRFront]);   //TopLeftF to TopRightF
	Points B9(ArrayPoints[TopRightCornners]); //TopRightF to TopRightB
	Points C9(B9.x2, B9.y2, B9.x1, B9.y1);	  //TopRightB to TopRightF

	Triangle TriOnBack9(A9, B9, C9);

	//Top Right triangle Topside
	Points A10(A9.x1, A9.y1, B9.x2, B9.y2);     //TopLeftF to TopRightB
	Points B10(ArrayPoints[TopL2TopRBack]);     //TopLeftB to TopRightB 
	Points C10(A10.x2, A10.y2, A10.x1, A10.y1);	//TopRightB to TopLeftF

	Triangle TriOnBack10(A10, B10, C10);



	//bottom left triangle Bottomside
	Points A11(ArrayPoints[BottomL2BottomRFront]);	 //BottomLeftF to BottomRightF
	Points B11(ArrayPoints[BottomRightCornners]);	 //BottomRightF to BottomRightB
	Points C11(B11.x2, B11.y2, A11.x1, A11.y1);	     //BottomRightB to BottomLeftF

	Triangle TriOnBack11(A11, B11, C11);

	//---------------------------------------------------------------------------------------------------
	//Top Right triangle bottomside
	Points A12(ArrayPoints[BottomLeftCornners]);     //BottomLeftF to BottomLeftB
	Points B12(ArrayPoints[BottomL2BottomRBack]);	 //BottomLeftB to BottomRightB
	Points C12(B12.x2, B12.y2, A12.x1, A12.y1);	     //BottomRightB to BottomLeftF

	Triangle TriOnBack12(A12, B12, C12);



	Triangle AllTriangles[12] = {
	TriOnFront,
	TriOnFront2,
	TriOnBack3,
	TriOnBack4,
	TriOnBack5,
	TriOnBack6,
	TriOnBack7,
	TriOnBack8,
	TriOnBack9,
	TriOnBack10,
	TriOnBack11,
	TriOnBack12
	};

	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 3; j++) {
			ParametricLineFunction(AllTriangles[i].LineABC[j], LightBlue, TotalPixels, MaxPixels, PixelWidth);
		}

		BetterBruteTriangle(AllTriangles[i], TotalPixels, MaxPixels, PixelWidth, PColor(0xFF5b3a80));
	}

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

