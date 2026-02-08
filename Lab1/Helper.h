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
	return ((nX * nWidth) + nY);
}


// A function to draw a pixel (fill a certain pixel with a specific color)
void DrawPixel(int ArrySpot, PColor color, unsigned int PixelArry[]) {
	//changing each slot in the array to one color value
	for (int i = 0; i < *PixelArry; i++) {
		if (ArrySpot == i) {
			PixelArry[i] = color.color;
		}
	}
}


// A function to BLIT (Block Image Transfer)
/*
void BLIT(4 ints for SourceRect, 2 ints for RasterPos, 1 int for SourceTextureWidth,
	unsigned int* pSourceTextureArray)
{
	for (int y = 0; y < SourceRectHeight; y++)
	{
		for (int x = 0; x < SourceRectWidth; x++)
		{
			// copy a pixel from pSourceTextureArray to the Raster
		}
	}
}*/


// Color convertion BGRAtoARGB
PColor BGRAtoARGB(unsigned C)
{
	PColor ColorConverted;

	//1) use the & operator (bitwise - and) with a bitmask to isolate each of the 4 color channels contained in C

						   // BBGGRRAA
						  //0x000000FF = AA
	ColorConverted.A = (C & 0x000000FF);
	
						  //0x0000FF00 = RR
	ColorConverted.R = (C & 0x0000FF00);

						  //0x00FF0000 = GG
	ColorConverted.G = (C & 0x00FF0000);

						  //0xFF000000 = BB
	ColorConverted.B = (C & 0xFF000000);


	//2) use the << and >> operators(bitwise left - shift and right - shift) to shift each channel to its new position

	//3) use the | operator (bitwise - or ) to re - combine the 4 color channels into a single unsigned int, and return it
	ColorConverted.CombineColor();

	return ColorConverted;
}