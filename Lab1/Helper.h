#pragma once
#include "MiniClasses.h"


// A function to clear the color buffer to a solid color of your choice.
void CCBuffer(PColor color, unsigned int *PixelArry, int ArrySize) {

	//changing each slot in the array to one color value
	for (int i = 0; i < ArrySize; i++) {
		PixelArry[i] = color.color;
	}
}

// A function to convert 2 dimensional coordinates to a 1 dimensional coordinate.
int Convert2Dto1D(int nX, int nY, int nWidth)
{
	//return a 1d coordinate using the 2D->1D formula
	return ((nY * nWidth) + nX);
}

// A function to draw a pixel (fill a certain pixel with a specific color)
void DrawPixel(int ArrySpot, PColor color, unsigned int* PixelArry, int ArrySize) {
	//changing each slot in the array to one color value
	for (int i = 0; i < ArrySize; i++) {
		if (ArrySpot == i) {
			PixelArry[i] = color.color;
		}
	}
}

// A function to BLIT (Block Image Transfer)
void BLIT(Position SourceRect, Position RasterPos, const unsigned int* pSourceTextureArray, unsigned int* ArryScreen)
{
	for (int y = 0; y < SourceRect.height; y++)
	{
		for (int x = 0; x < SourceRect.width; x++)
		{
			// copy a pixel from pSourceTextureArray to the Raster
			ArryScreen[Convert2Dto1D(x, y, RasterPos.width)] = pSourceTextureArray[Convert2Dto1D(x, y, SourceRect.width)];
		}
	}
}

// Color conversion BGRAtoARGB
PColor BGRAtoARGB(unsigned int C)
{
	PColor ColorConverted;

	//1) use the & operator (bitwise - and) with a bitmask to isolate each of the 4 color channels contained in C

						   // BBGGRRAA
						  //0x000000FF = AA   //0xAARRGGBB
	ColorConverted.A = (C & 0x000000FF) << 24;//0xFF000000 = AA
	
						  //0x0000FF00 = RR
	ColorConverted.R = (C & 0x0000FF00) << 8; //0x00FF0000 = RR

						  //0x00FF0000 = GG
	ColorConverted.G = (C & 0x00FF0000) >> 8; //0x0000FF00 = GG

						  //0xFF000000 = BB
	ColorConverted.B = (C & 0xFF000000) >> 24;//0x000000FF= BB


	//2) use the << and >> operators(bitwise left - shift and right - shift) to shift each channel to its new position

	//3) use the | operator (bitwise - or ) to re - combine the 4 color channels into a single unsigned int, and return it

	return ColorConverted.CombineColor();
}