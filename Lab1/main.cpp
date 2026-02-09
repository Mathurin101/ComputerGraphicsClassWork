#include "RasterSurface.h"
#include "Helper.h"
#include "XTime.h"
#include "tiles_12.h"
#include "teleporter_hit.h"
#include <random> //random number class "std::rand()"

const char* Name = "MathurinGenty_ProgrammingAssignment 1";
const unsigned int PixelWidth = 500;
const unsigned int PixelHeight = 500;

const unsigned int MaxPixels = PixelWidth * PixelHeight;

unsigned int TotalPixels[MaxPixels];
XTime Timer;

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
	int randomNumber = (rand() % PixelWidth);
	int randomNumber2 = (rand() % PixelHeight);
	int randomNumber3 = (rand() % PixelWidth);
	int randomNumber4 = (rand() % PixelHeight);

	int CurrentX = 0;//x = 130
	int CurrentY = 0;//y = 155
	int WidthX = 128; //x = 130
	int HeightY = 128;// y = 117
		
	//will print on the screen
	do {
		Timer.Signal();
		Timer.Delta();
		
		//background
		for (int width = 0; width <= PixelWidth/GrassRect.width; width++) {

			for (int height = 0; height <= PixelHeight/GrassRect.height; height++) {

				Position GrassPos(width * GrassRect.width, height * GrassRect.height, PixelWidth, PixelHeight);

				BLIT(GrassRect, GrassPos, tiles_12_pixels, TotalPixels, tiles_12_width);

			}
		}
		
		//BLIT(scrRect, RasterPos, tiles_12_pixels, TotalPixels, tiles_12_width);//OG

		Position RandomRasterPos(randomNumber, randomNumber2, PixelWidth, PixelHeight);
		BLIT(GrassWFlower, RandomRasterPos, tiles_12_pixels, TotalPixels, tiles_12_width);

		Position RandomRasterPos2(randomNumber3, randomNumber4, PixelWidth, PixelHeight);
		BLIT(GrassHouse, RandomRasterPos2, tiles_12_pixels, TotalPixels, tiles_12_width);


		//BLIT(TeleportFile, RasterPos, teleporter_hit_pixels, TotalPixels, teleporter_hit_width);
		//BLIT(TeleportEffect, RasterPos, teleporter_hit_pixels, TotalPixels, teleporter_hit_width);
		//BLIT(SingleTeleportEffect, RasterPos, teleporter_hit_pixels, TotalPixels, teleporter_hit_width);

		/*
		for (int i = 0; i < teleporter_hit_width; i++) {

			//AlphaBlend(0x00000000, teleporter_hit_pixels[i]);
			//BLIT(SingleTeleportEffect, RasterPos, teleporter_hit_pixels, TotalPixels, teleporter_hit_width);
		
			CCBuffer(AlphaBlend(0x0000FF00, 0x00006700), TotalPixels, MaxPixels);
		}
		*/

		//SingleTeleportEffect -- Animation Update 
		BLIT(Position(CurrentX, CurrentY, WidthX, HeightY), RasterPos, teleporter_hit_pixels, TotalPixels, teleporter_hit_width);
		
		CurrentX += WidthX;
		if (CurrentX >= teleporter_hit_width - WidthX) //is off the edge of the page
		{
			CurrentX = 0;
			
			// perform additional check here to see if CurrentY has gone off the bottom of the page
			if (CurrentY >= teleporter_hit_height - HeightY) {
				CurrentY = 0;
			}
			else {
				CurrentY += HeightY; // += PixelHeight;
			}
		}

	} while (RS_Update(TotalPixels, MaxPixels));

	RS_Shutdown();

	return 0;
}

