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


	
	PColor ColorRED(0xFFFF0000);//red
				//		x,   y,   x,   y
	Points OuterLine1(180, 100, 260, 100);//   -
	 
	Points OuterLine2(100, 160, 100, 240);// |
	
	Points OuterLine3(345, 240, 345, 160);//     |     

	Points OuterLine4(180, 300, 260, 300);//   _
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
	PColor ColorLightBlue(0xFFADD8E6);//LightBlue
	Points InnerLine1(180, 100, 260, 300);
	
	PColor ColorMarigold(0xFFEAA221);//Marigold
	Points InnerLine2(180, 300, 260, 100);
	//
	PColor ColorLilac(0xFFE6DAF0);//Lilac
	Points InnerLine3(100, 160, 340, 240);
	//
	PColor ColorPastelYellow(0xFFFDFD96);//Pastel Yellow
	Points InnerLine4(100, 240, 340, 160);
	
	//will print on the screen
	do {
		
		//ParametricLineFunction(Line, ColorOG, TotalPixels, MaxPixels, RasterPos.width);


		//border angles
		ParametricLineFunction(OuterLine5, ColorRED, TotalPixels, MaxPixels, RasterPos.width);
		ParametricLineFunction(OuterLine6, ColorRED, TotalPixels, MaxPixels, RasterPos.width);
		ParametricLineFunction(OuterLine7, ColorRED, TotalPixels, MaxPixels, RasterPos.width);
		ParametricLineFunction(OuterLine8, ColorRED, TotalPixels, MaxPixels, RasterPos.width);
		
		//straight lines - left to right
		ParametricLineFunction(OuterLine1, ColorRED, TotalPixels, MaxPixels, RasterPos.width);
		ParametricLineFunction(OuterLine4, ColorRED, TotalPixels, MaxPixels, RasterPos.width);
		
		ParametricLineFunction(OuterLine2, ColorRED, TotalPixels, MaxPixels, RasterPos.width);
		//up and down lines -- doesn't work  
		ParametricLineFunction(OuterLine3, ColorRED, TotalPixels, MaxPixels, RasterPos.width);
		
		
		
		//lines crossing
		ParametricLineFunction(InnerLine1, ColorLightBlue, TotalPixels, MaxPixels, RasterPos.width);
		ParametricLineFunction(InnerLine2, ColorMarigold, TotalPixels, MaxPixels, RasterPos.width);
		ParametricLineFunction(InnerLine3, ColorLilac, TotalPixels, MaxPixels, RasterPos.width);
		ParametricLineFunction(InnerLine4, ColorPastelYellow, TotalPixels, MaxPixels, RasterPos.width);



	} while (RS_Update(TotalPixels, MaxPixels));

	RS_Shutdown();

	return 0;
}

