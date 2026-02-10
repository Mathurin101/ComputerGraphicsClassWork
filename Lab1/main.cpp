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

	//Position scrRect[4] = { 149,44, 190, 95 };
	Position scrRect(0, 0, tiles_12_width, tiles_12_height);
	const Position RasterPos(0, 0, PixelWidth, PixelHeight);

	Position GrassRect(288, 128, 32, 32);

	Position GrassWFlower(64, 192, 16, 16);

	Position GrassHouse(208, 32, 96, 65);

	Position TeleportFile(1034, 1000, teleporter_hit_width, teleporter_hit_height); //teleporter_hit.h
	Position TeleportEffect(515, 10, teleporter_hit_width, teleporter_hit_height);
	Position SingleTeleportEffect(132, 155, 120, 120);



	PColor ColorCH(0xFF123524);//green

	RS_Initialize(Name, PixelWidth, PixelHeight);
	srand(time(0));


	int CurrentX = 0;//x = 130
	int CurrentY = 0;//y = 155
	int WidthX = 128; //x = 130
	int HeightY = 128;// y = 117

	double PersonTimer = 0;
	XTime Timer;

	Points Line( 32, 54, 63, 79);

	PColor ColorOG(0xFF8e6acc);//purple

	
	PColor ColorRED(0xFFFF0000);//red
				//		x,   y,   x,   y
	//Points OuterLine1(132, 100, 133, 500);//   -
	//Points OuterLine2(132, 154, 163, 179);// |
	//Points OuterLine3(132, 154, 163, 179);//     |
	//Points OuterLine4(132, 154, 163, 179);//   _
	  Points OuterLine5(100, 160, 180, 100);// /
	  Points OuterLine6(260, 100, 340, 160);//     \        //
	  Points OuterLine7(100, 240, 180, 300);//  \           //
	  Points OuterLine8(260, 300, 340, 240);//     /
	//	  _______
	//	 /	   	 \
	//  /         \
 // // |           |
    // |           |
	//  \         /
	//	 \_______/
	//
	PColor ColorDioxazine(0xFF320960);//Dioxazine purple
	//Points InnerLine1( 132, 154, 163, 179);
	//
	PColor ColorMarigold(0xFFEAA221);//Marigold
	//Points InnerLine2( 132, 154, 163, 179);
	//
	PColor ColorLilac(0xFFE6DAF0);//Lilac
	//Points InnerLine3( 132, 154, 163, 179);
	//
	PColor ColorPastelYellow(0xFFFDFD96);//Pastel Yellow
	//Points InnerLine4( 132, 154, 163, 179);
	
	//will print on the screen
	do {
		Timer.Signal();
		Timer.Delta();
		
		//ParametricLineFunction(Line, ColorOG, TotalPixels, MaxPixels, RasterPos.width);


		//border
		ParametricLineFunction(OuterLine5, ColorRED, TotalPixels, MaxPixels, RasterPos.width);
		ParametricLineFunction(OuterLine6, ColorRED, TotalPixels, MaxPixels, RasterPos.width);
		ParametricLineFunction(OuterLine7, ColorRED, TotalPixels, MaxPixels, RasterPos.width);
		ParametricLineFunction(OuterLine8, ColorRED, TotalPixels, MaxPixels, RasterPos.width);




	} while (RS_Update(TotalPixels, MaxPixels));

	RS_Shutdown();

	return 0;
}

