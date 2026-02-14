#pragma once
#include "MiniClasses.h"
#include "tiles_12.h"
#include <iostream>

static const unsigned int _Red   = 0xFFFF0000;
static const unsigned int _Green = 0xFF00FF00;
static const unsigned int _Blue  = 0xFF0000FF;


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

	//ColorConverted.color = (ColorConverted.A | ColorConverted.R | ColorConverted.G | ColorConverted.B);

	return ColorConverted.color = (ColorConverted.A | ColorConverted.R | ColorConverted.G | ColorConverted.B);
}

unsigned int AlphaBlend(unsigned int DestinationColor, unsigned int SourceColor)
// Destination is the raster, source is the texture
{
	//break DestinationColor and SourceColor down into 4 channels each,... 
	//...using bitwise operators to mask and right - shift into the lowest - order 8 bits
	PColor BgDestination(DestinationColor);
	PColor SourceImage(BGRAtoARGB(SourceColor));
	float fAlphaRatio;
	PColor FinalImage;

	//Calculate fAlphaRatio as SourceAlpha(texture alpha channel) / 255.0f
	fAlphaRatio = (SourceImage.A / 255.0f);

	//Interpolate all 4 channels based on fAlphaRatio to get FinalRed,... 
	//...FinalGreen, FinalBlue, and FinalAlpha(1.0 alpha = fullSource,... 
	//...0.0 alpha = full destination color)
	FinalImage.A = (fAlphaRatio * SourceImage.A) + ((1.0f - fAlphaRatio) * BgDestination.A);
	FinalImage.R = (fAlphaRatio * SourceImage.R) + ((1.0f - fAlphaRatio) * BgDestination.R);
	FinalImage.G = (fAlphaRatio * SourceImage.G) + ((1.0f - fAlphaRatio) * BgDestination.G);
	FinalImage.B = (fAlphaRatio * SourceImage.B) + ((1.0f - fAlphaRatio) * BgDestination.B);

	//use bitwise operators(left - shift and bitwise - or )... 
	//...to reassemble the 4 color channels into a single unsigned int, and return it

	//FinalImage.color = (FinalImage.A << 24) | (FinalImage.R << 16) | (FinalImage.G << 8) | FinalImage.B;

	return FinalImage.color = (FinalImage.A << 24) | (FinalImage.R << 16) | (FinalImage.G << 8) | FinalImage.B;//SourceImage.CombineColor().color
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

			TileP = AlphaBlend(ArryScreen[Convert2Dto1D(RasterPos.x + x - SourceRect.x, RasterPos.y + y - SourceRect.y, RasterPos.width)], TileP.color);

			// copy a pixel from pSourceTextureArray to the Raster
			ArryScreen[Convert2Dto1D(RasterPos.x + x - SourceRect.x, RasterPos.y + y - SourceRect.y, RasterPos.width)] = TileP.color;
		}
	}
}

void ParametricLineFunction(Points Spots, PColor _color, unsigned int* PixelArry, int ArrySize, int RasterWidth) {
	double CurrentX;
	double CurrentY;
	double StartX = CurrentX = Spots.x1;
	double StartY = CurrentY = Spots.y1;//A
	double EndY = Spots.y2; //B
	double EndX = Spots.x2;
	double Steps;//bottom half of ratio
	float IncrementX;
	float IncrementY;

	//Parametric Line Algorithm​

	//Take the largest difference
	if (abs(Spots.deltaX) > abs(Spots.deltaY)) {
		Steps = abs(Spots.deltaX);
	}
	else {
		Steps = abs(Spots.deltaY);
	}

	//get change of x and change of y per step​
	IncrementX = Spots.deltaX / Steps;
	IncrementY = Spots.deltaY / Steps;


	//FOR i to steps​
	for (int i = 0; i < Steps; i++) {

		//PlotPixel(CurrX, Floor(CurrY + 0.5))​ //Convert2Dto1D(CurrentX, CurrentY + 0.5, RasterWidth)
		DrawPixel(Convert2Dto1D(CurrentX, CurrentY, RasterWidth), _color, PixelArry, ArrySize);

		// Increment the current x  
		CurrentX = CurrentX + IncrementX;

		// Increment the current y 
		CurrentY = CurrentY + IncrementY;

		std::cout << "CurrentX: " << CurrentX << "\nCurrentY: " << CurrentY << std::endl;
	}
}

Matrix4x4 MultiplyMatrixByMatrix(Vertex& matrix1, Vertex& matrix2)
{
	Matrix4x4 mOutput;

	mOutput.xx = matrix1.x * matrix2.x;
	mOutput.xy = matrix1.x * matrix2.y;
	mOutput.xz = matrix1.x * matrix2.z;
	mOutput.xw = matrix1.x * matrix2.w;
	
	mOutput.yx = matrix1.y * matrix2.x;
	mOutput.yy = matrix1.y * matrix2.y;
	mOutput.yz = matrix1.y * matrix2.z;
	mOutput.yw = matrix1.y * matrix2.w;
	
	mOutput.zx = matrix1.z * matrix2.x;
	mOutput.zy = matrix1.z * matrix2.y;
	mOutput.zz = matrix1.z * matrix2.z;
	mOutput.zw = matrix1.z * matrix2.w;
	
	mOutput.wx = matrix1.w * matrix2.x;
	mOutput.wy = matrix1.w * matrix2.y;
	mOutput.wz = matrix1.w * matrix2.z;
	mOutput.ww = matrix1.w * matrix2.w;
	
	return mOutput;
}

Points NDCtoScreen(Vertex NDC, float Width, float Height) {
	float SceenX1 = ((NDC.x + 1) / 2) * Width;	
	float SceenY1 = ((1 - NDC.y) / 2) * Height;	

	return Points(SceenX1, SceenY1, SceenX2, SceenY2);
}