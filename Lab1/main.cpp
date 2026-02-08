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

int main()
{
	//Position scrRect[4] = { 149,44, 190, 95 };
	Position scrRect(0, 0, tiles_12_width, tiles_12_height);
	const Position RasterPos(0, 0, PixelWidth, PixelHeight);

	Position GrassRect(288, 128, 32, 32);

	Position GrassWFlower(64, 192, 16, 16);

	Position GrassHouse(208, 32, 96, 65);

	Position Teleport(208, 32, 96, 65);//---------------------------

	PColor ColorOG(0xFF8e6acc);//purple

	PColor ColorCH(0xFF123524);//green

	RS_Initialize(Name, PixelWidth, PixelHeight);
	srand(time(0));
	int randomNumber = (rand() % PixelWidth);
	int randomNumber2 = (rand() % PixelHeight);
	int randomNumber3 = (rand() % PixelWidth);
	int randomNumber4 = (rand() % PixelHeight);

	//will print on the screen
	do {

		//CCBuffer(ColorOG.color, TotalPixels, MaxPixels);
		//CCBuffer(BGRAtoARGB(ColorCH.color).color, TotalPixels, MaxPixels);

		//DrawPixel(72, ColorG.color, TotalPixels, MaxPixels);

		//BLIT(Position(0,0,100,100), RasterPos, tiles_12_pixels, TotalPixels, tiles_12_width);
		//BLIT(GrassRect, RasterPos, tiles_12_pixels, TotalPixels, tiles_12_width);

		//grass
		//DrawTile(GrassRect, RasterPos, tiles_12_pixels, TotalPixels, tiles_12_width);

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

		
		//BLIT(GrassHouse, RasterPos, teleporter_hit_pixels, TotalPixels, teleporter_hit_width);


	} while (RS_Update(TotalPixels, MaxPixels));

	RS_Shutdown();

	return 0;
}

