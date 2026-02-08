#pragma once
#include "MiniClasses.h"
#include "tiles_12.h"

// A function to clear the color buffer to a solid color of your choice.
void CCBuffer(PColor color, unsigned int* PixelArry, int ArrySize) {

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

// Color conversion BGRAtoARGB
PColor BGRAtoARGB(unsigned int C)
{
	PColor ColorConverted;

	// BBGGRRAA
   //0x000000FF = AA   //0xAARRGGBB
	ColorConverted.A = (C & 0x000000FF) << 24;//0xFF000000 = AA

	//0x0000FF00 = RR
	ColorConverted.R = (C & 0x0000FF00) << 8; //0x00FF0000 = RR

	//0x00FF0000 = GG
	ColorConverted.G = (C & 0x00FF0000) >> 8; //0x0000FF00 = GG

	//0xFF000000 = BB
	ColorConverted.B = (C & 0xFF000000) >> 24;//0x000000FF = BB

	return ColorConverted.CombineColor();
}

// A function to BLIT (Block Image Transfer)
void BLIT(Position SourceRect, Position RasterPos, const unsigned int* pSourceTextureArray, unsigned int* ArryScreen, unsigned int SourceWidth)
{
	for (int y = SourceRect.y; y < SourceRect.height + SourceRect.y; y++)
	{
		for (int x = SourceRect.x; x < SourceRect.width + SourceRect.x; x++)
		{
			if (((RasterPos.x + x - SourceRect.x) >= RasterPos.width) || (RasterPos.y + y - SourceRect.y) >= RasterPos.height) {
				continue;
			}
			PColor TileP = pSourceTextureArray[Convert2Dto1D(x, y, SourceWidth)];
			if (TileP.A != 0x00) {//black

				// copy a pixel from pSourceTextureArray to the Raster
				ArryScreen[Convert2Dto1D(RasterPos.x + x - SourceRect.x, RasterPos.y + y - SourceRect.y, RasterPos.width)] = BGRAtoARGB(TileP.color).color;
			}
		}
	}
}

/*
void DrawTile(Position &SourceRect, Position &RasterPos, const unsigned int* pSourceTextureArray,
	unsigned int* ArryScreen, unsigned int SourceWidth) {

	for (int width = 0; width < RasterPos.width; width += SourceRect.width) {

		for (int height = 0; height < RasterPos.height; height += SourceRect.height) {

			BLIT(SourceRect, RasterPos, pSourceTextureArray, ArryScreen, SourceWidth);

			RasterPos.x = width;
			RasterPos.y = height;
		}
	}
}
*/

unsigned int AlphaBlend(unsigned int DestinationColor, unsigned int SourceColor) 
						 // Destination is the raster, source is the texture
{
	//break DestinationColor and SourceColor down into 4 channels each, 
	//   using bitwise operators to mask and right - shift into the lowest - order 8 bits
	PColor BgDestination(DestinationColor);
	PColor SourceImage(BGRAtoARGB(SourceColor));
	PColor fAlphaRatio;

	//Calculate fAlphaRatio as SourceAlpha(texture alpha channel) / 255.0f
	fAlphaRatio.A = (SourceImage.A / 255.0f);

	//Interpolate all 4 channels based on fAlphaRatio to get FinalRed,... 
	//...FinalGreen, FinalBlue, and FinalAlpha(1.0 alpha = fullSource,... 
	//...0.0 alpha = full destination color)
	SourceImage.A = (fAlphaRatio.A * SourceImage.A) + ((1.0f - fAlphaRatio.A) * BgDestination.A);
	SourceImage.R = (fAlphaRatio.A * SourceImage.R) + ((1.0f - fAlphaRatio.A) * BgDestination.R);
	SourceImage.G = (fAlphaRatio.A * SourceImage.G) + ((1.0f - fAlphaRatio.A) * BgDestination.G);
	SourceImage.B = (fAlphaRatio.B * SourceImage.B) + ((1.0f - fAlphaRatio.A) * BgDestination.B);

	//use bitwise operators(left - shift and bitwise - or ) 
	// to reassemble the 4 color channels into a single unsigned int, and return it

	return SourceImage.CombineColor().color;
}

void AnimationUpdate() {
	/*
	CurrentX += CellWidth;
	if (CurrentX is off the edge of the page)
	{
		CurrentX = 0;
		CurrentY += CellHeight;
		// perform additional check here to see if CurrentY has gone off the bottom of the page
	}*/
}