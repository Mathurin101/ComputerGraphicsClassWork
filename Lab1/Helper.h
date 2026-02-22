#pragma once
#include "MiniClasses.h"
#include "tiles_12.h"
#include "Shaders.h"
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
	float CurrentX;
	float CurrentY;
	float StartX = CurrentX = Spots.x1;
	float StartY = CurrentY = Spots.y1;//A
	float EndY = Spots.y2; //B
	float EndX = Spots.x2;
	float Steps;//bottom half of ratio
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
	}
}

float ImplicitLineEquation(Position point, Points Line) {
	float Answer;

	Answer = (((Line.y1 - Line.y2) * (point.x)) + ((Line.x2 - Line.x1) * (point.y)) + ((Line.x1 * Line.y2) - (Line.y1 * Line.x2)));

	return Answer;
}

Points NDCtoScreen(Vertex NDC, float Width, float Height) {
	float SceenX1 = ((NDC.x + 1) * (Width / 2));
	float SceenY1 = ((1 - NDC.y) * (Height / 2));

	return Points(SceenX1, SceenY1);
}

Matrix4x4 IdentityMatrix() {

	Matrix4x4 Ident
	   (1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	return Ident;
}

Matrix4x4 TranslationMatrix(float x, float y, float z) {

	Matrix4x4 Translation 
	(1, 0, 0, 0,            
	 0, 1, 0, 0,	        
	 0, 0, 1, 0,	        
	 x, y, z, 1 );   

	return Translation;
}

float DegreesToRadians(float Degrees) {
	return   (Degrees * (3.14f / 180.0f));
}

Matrix4x4 RotateY(float x) {
	float y = DegreesToRadians(x);


	Matrix4x4 Rotate
  (cosf(y),  0, sinf(y), 0,
		0,   1,       0, 0,
  -sinf(y),  0, cosf(y), 0,
		0,   0,       0, 1);
			  
	return Rotate;
}

Matrix4x4 RotateX(float x) {
	float y = DegreesToRadians(x);


	Matrix4x4 Rotate
	   (1, 0, 0, 0,
		0, cosf(y), -sinf(y), 0,
		0, sinf(y), cosf(y), 0,
		0, 0, 0, 1);

	return Rotate;
}

Matrix4x4 RotateZ(float x) {
	float y = DegreesToRadians(x);


	Matrix4x4 Rotate
		(cosf(y), -sinf(y), 0, 0,
		 sinf(y),  cosf(y), 0, 0,
		      0,       0, 1, 0,
		      0,       0, 0, 1);

	return Rotate;
}

Matrix4x4 Transpose(const Matrix4x4& mIn) {
	Matrix4x4 Trans;
	Trans.xx = mIn.xx; Trans.xy = mIn.yx; Trans.xz = mIn.zx; Trans.xw = mIn.wx;
	Trans.yx = mIn.xy; Trans.yy = mIn.yy; Trans.yz = mIn.zy; Trans.yw = mIn.wy;
	Trans.zx = mIn.xz; Trans.zy = mIn.yz; Trans.zz = mIn.zz; Trans.zw = mIn.wz;
	Trans.wx = mIn.xw; Trans.wy = mIn.yw; Trans.wz = mIn.zw; Trans.ww = mIn.ww;
	return Trans;
}

Matrix4x4 OrthonormalInverse(const Matrix4x4& mIn)
{
	Matrix4x4 Inverse;

	Inverse = Transpose(mIn);
	//set default 0.0s and 1.0s for the.w components of m;
	
	// calculate new position:
	Inverse.AxisW.x = -DotProduct(mIn.AxisX, mIn.AxisW);
	Inverse.AxisW.y = -DotProduct(mIn.AxisY, mIn.AxisW);
	Inverse.AxisW.z = -DotProduct(mIn.AxisZ, mIn.AxisW);

	return Inverse;
}

Matrix4x4 PerspectiveProjection(float FOV, float Ratio, float Near, float Far) {
	float FOVRad = DegreesToRadians(FOV);
	float Yscale = 1 / tanf(FOVRad / 2);
	float Xscale = Yscale * Ratio;
	float Zdifference = -(Far * Near) / (Far - Near);
	float Zdif = Far / (Far - Near);



	Matrix4x4 DoubleP(
   Xscale,      0,           0, 0,
		0, Yscale,           0, 0,
		0,      0,        Zdif, 1,
		0,      0, Zdifference, 0
	);

	return DoubleP;
}