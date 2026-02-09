#include "RasterSurface.h"
#include "Helper.h"
#include "XTime.h"
#include "tiles_12.h"
#include "teleporter_hit.h"
#include <random> //random number class "std::rand()"
#include <iostream>

const char* Name = "MathurinGenty_ProgrammingAssignment 1";
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


	PColor ColorOG(0xFF8e6acc);//purple

	PColor ColorCH(0xFF123524);//green

	RS_Initialize(Name, PixelWidth, PixelHeight);
	srand(time(0));


	int CurrentX = 0;//x = 130
	int CurrentY = 0;//y = 155
	int WidthX = 128; //x = 130
	int HeightY = 128;// y = 117

	double PersonTimer = 0;
	XTime Timer;

	int RandomNumberX[11];
	int RandomNumberY[11];
	for (int i = 0; i < 10; i++) {
		RandomNumberX[i] = int((rand() % PixelWidth));
	}
	for (int i = 0; i < 10; i++) {
		RandomNumberY[i] = int((rand() % PixelWidth));
	}

	//will print on the screen
	do {
		Timer.Signal();
		Timer.Delta();
		
		/*
		CurrY = StartY
		Slope = SlopeY / SlopeX

		Error = 0

		For StartX to EndX
			PlotPixel(CurrX, CurrY)
			Error += Slope 
				If Error > 0.5
					CurrY += 1, Error -= 1

			*/










	} while (RS_Update(TotalPixels, MaxPixels));

	RS_Shutdown();

	return 0;
}

