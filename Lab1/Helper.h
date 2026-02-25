#pragma once
#include "MiniClasses.h"
#include "tiles_12.h"
#include "Shaders.h"
#include <iostream>

static const unsigned int _Red = 0xFFFF0000;
static const unsigned int _Green = 0xFF00FF00;
static const unsigned int _Blue = 0xFF0000FF;

const unsigned int PixelWidth = 600;
const unsigned int PixelHeight = 500;

const unsigned int MaxPixels = PixelWidth * PixelHeight;

unsigned int TotalPixels[MaxPixels];
float DepthBuffer[MaxPixels];

void Print(std::string words) {
	std::cout << words << std::endl;
}
// A function to clear the color buffer to a solid color of your choice.
void CCBuffer(PColor color, unsigned int* PixelArry, int ArrySize, float* ZBuffer) {

	//changing each slot in the array to one color value
	for (int i = 0; i < ArrySize; i++) {
		PixelArry[i] = color.color;
		ZBuffer[i] = 1.0f;
	}
}

// A function to convert 2 dimensional coordinates to a 1 dimensional coordinate.
int Convert2Dto1D(int nX, int nY, int nWidth)
{

	//return a 1d coordinate using the 2D->1D formula
	return ((nY * nWidth) + nX);
}

// A function to draw a pixel (fill a certain pixel with a specific color)
void DrawPixel(int ArrySpot, PColor color, unsigned int* PixelArry, int ArrySize, float* ZBuffer, float Depth) {
	//changing each slot in the array to one color value

		//If the new value is lower (nearer to the camera)
	if (ZBuffer[ArrySpot] >= Depth) {
		//you draw the pixel and write that new depth into the buffer.
		PixelArry[ArrySpot] = color.color;
		ZBuffer[ArrySpot] = Depth;
	}
	//If the new value is farther than what's already in the buffer, you just don't draw it.
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

void ParametricLineFunction(Points Spots, PColor _color, unsigned int* PixelArry, int ArrySize, int RasterWidth, float* ZBuffer) {
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
		DrawPixel(Convert2Dto1D(CurrentX, CurrentY, RasterWidth), _color, PixelArry, ArrySize, ZBuffer, 1);

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

Position NDCtoScreen(Vertex NDC, float Width = PixelWidth, float Height = PixelHeight) {
	float SceenX1 = ((NDC.cord.x + 1) * (Width / 2));
	float SceenY1 = ((1 - NDC.cord.y) * (Height / 2));
	return Position(SceenX1, SceenY1);
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
		x, y, z, 1);

	return Translation;
}

float DegreesToRadians(float Degrees) {
	return   (Degrees * (3.14f / 180.0f));
}

Matrix4x4 RotateY(float x) {
	float y = DegreesToRadians(x);

	Matrix4x4 Rotate
	(cosf(y), 0, sinf(y), 0,
		0, 1, 0, 0,
		-sinf(y), 0, cosf(y), 0,
		0, 0, 0, 1);

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
		sinf(y), cosf(y), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

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
	float Yscale = 1 / tanf((FOVRad / 2));
	float Xscale = Yscale * Ratio;
	float Zdifference = -((Far * Near) / (Far - Near));
	float Zdif = (Far / (Far - Near));
	//std::cout << "Ratio: " << Ratio << std::endl;
	//std::cout << "FOVRad: " << FOVRad << std::endl;
	//std::cout << "Yscale: " << Yscale << std::endl;
	//std::cout << "Xscale: " << Xscale << std::endl;
	//std::cout << "Zdifference: " << Zdifference << std::endl;
	//std::cout << "Zdif: " << Zdif << std::endl;

	Matrix4x4 DoubleP(
		Xscale, 0, 0, 0,
		0, Yscale, 0, 0,
		0, 0, Zdif, 1,
		0, 0, Zdifference, 0);

	return DoubleP;
}

float MinOut3(float one, float two, float three) {
	float A = one;
	float B = two;
	float C = three;
	float MinFloat;

	if (A <= C && B >= A) {
		MinFloat = A;
	}
	else if (B <= C && A >= B) {
		MinFloat = B;
	}
	else {
		MinFloat = C;
	}

	return MinFloat;
}

float MaxOut3(float one, float two, float three) {
	float A = one;
	float B = two;
	float C = three;
	float MaxFloat;

	if (A >= C && B <= A) {
		MaxFloat = A;
	}
	else if (B >= C && A <= B) {
		MaxFloat = B;
	}
	else {
		MaxFloat = C;
	}

	return MaxFloat;
}

BarycentricCoord Barycentric(Position pointA, Position pointB, Position pointC, Position pointP) {
	BarycentricCoord Three; //NDCtoScreen(Vertex NDC, float Width, float Height)

	Three.Beta = ImplicitLineEquation(pointB, Points(pointA.x, pointA.y, pointC.x, pointC.y));
	Three.Gamma = ImplicitLineEquation(pointC, Points(pointB.x, pointB.y, pointA.x, pointA.y));
	Three.Alpha = ImplicitLineEquation(pointA, Points(pointC.x, pointC.y, pointB.x, pointB.y));

	float b = ImplicitLineEquation(pointP, Points(pointA.x, pointA.y, pointC.x, pointC.y));
	float y = ImplicitLineEquation(pointP, Points(pointB.x, pointB.y, pointA.x, pointA.y));
	float a = ImplicitLineEquation(pointP, Points(pointC.x, pointC.y, pointB.x, pointB.y));

	//TODO: Could cut some frames by not doing the last division and by seeing if the rest gives zero then making (a / Alpha) = 1
		//Pβγα = ( b / β ,   y / γ   ,  a / α )   ​
	return BarycentricCoord(a / Three.Alpha, b / Three.Beta, y / Three.Gamma);
}

void BruteTriangle(Triangle _Tri, unsigned int* PixelArry, int ArrySize, float* ZBuffer) {
	BarycentricCoord byA;
	float StartX = 0;
	float StartY = 0;
	float EndX = PixelWidth;
	float EndY = PixelHeight;
	float BaryInterpo = 0;


	for (int CurrY = StartY; CurrY < EndY; CurrY++) {

		for (int CurrX = StartX; CurrX < EndX; CurrX++) {

			//byA = FindBarycentric (CurrX, CurrY )​
			byA = Barycentric(NDCtoScreen(_Tri.A), NDCtoScreen(_Tri.B), NDCtoScreen(_Tri.C), Position(CurrX, CurrY));

			//IF b >=0 && b <= 1 && ​y >= 0 && y <= 1 &&​ a >= 0 && a <= 1​
			if ((byA.Beta >= 0 && byA.Beta <= 1) && (byA.Gamma >= 0 && byA.Gamma <= 1) && (byA.Alpha >= 0 && 1 >= byA.Alpha)) {

				//Barycentric Interpolation: X = A * α + B * β + C * γ 
				BaryInterpo = (_Tri.A.cord.z * byA.Alpha) + (_Tri.B.cord.z * byA.Beta) + (_Tri.C.cord.z * byA.Gamma);

				//THEN - ​PlotPixel ( CurrX, CurrY )​
				DrawPixel(Convert2Dto1D(CurrX, CurrY, PixelWidth), PColor(0xFFADD8E6), PixelArry, ArrySize, ZBuffer, BaryInterpo);
			}

		}
	}
}

void BetterBruteTriangle(Triangle _Tri, unsigned int* PixelArry, int ArrySize, float* ZBuffer, PColor Color) {
	BarycentricCoord byA;
	PixelShader = VS_PixelShadder;
	float StartX = MinOut3(NDCtoScreen(_Tri.A).x, NDCtoScreen(_Tri.B).x, NDCtoScreen(_Tri.C).x);
	float StartY = MinOut3(NDCtoScreen(_Tri.A).y, NDCtoScreen(_Tri.B).y, NDCtoScreen(_Tri.C).y);
	float EndX = MaxOut3(NDCtoScreen(_Tri.A).x, NDCtoScreen(_Tri.B).x, NDCtoScreen(_Tri.C).x);
	float EndY = MaxOut3(NDCtoScreen(_Tri.A).y, NDCtoScreen(_Tri.B).y, NDCtoScreen(_Tri.C).y);
	float BaryInterpo = 0;

	for (int CurrY = StartY; CurrY < EndY; CurrY++) {

		for (int CurrX = StartX; CurrX < EndX; CurrX++) {

			//byA = FindBarycentric (CurrX, CurrY )​
			byA = Barycentric(NDCtoScreen(_Tri.A), NDCtoScreen(_Tri.B), NDCtoScreen(_Tri.C), Position(CurrX, CurrY)); //FindBarycentric(_Tri, Position(CurrX, CurrY))​;

			//IF b >=0 && b <= 1 && ​y >= 0 && y <= 1 &&​ a >= 0 && a <= 1​
			if ((byA.Beta >= 0 && byA.Beta <= 1) && (byA.Gamma >= 0 && byA.Gamma <= 1) && (byA.Alpha >= 0 && 1 >= byA.Alpha)) {

				//Barycentric Interpolation: X = A * α + B * β + C * γ 
				//get A, B, and C's z value and multiply it with alpha, beta, and gamma
				BaryInterpo = (_Tri.A.cord.z * byA.Alpha) + (_Tri.B.cord.z * byA.Beta) + (_Tri.C.cord.z * byA.Gamma);

				//THEN - ​PlotPixel ( CurrX, CurrY )​
				DrawPixel(Convert2Dto1D(CurrX, CurrY, PixelWidth), PixelShader(_Tri, byA), PixelArry, ArrySize, ZBuffer, BaryInterpo);
			}

		}
	}
}



float DotProductVerts(Vertex Vert1, Vertex Vert2) {
	float Answer;

	Answer = (Vert1.cord.x * Vert2.cord.x) + (Vert1.cord.y * Vert2.cord.y) * (Vert1.cord.z * Vert2.cord.z);
	return Answer;
}

Vertex Cross_ProductVerts(Vertex Vert1, Vertex Vert2) {
	//1xA   Bx1
	//2yA   By2
	//3zA   Bz3
	return Vertex((Vert1.cord.y * Vert2.cord.z) - (Vert1.cord.z * Vert2.cord.y), //x
		(Vert1.cord.z * Vert2.cord.x) - (Vert1.cord.x * Vert2.cord.z), //y
		(Vert1.cord.x * Vert2.cord.y) - (Vert1.cord.y * Vert2.cord.x)); //z
}

//vec3_normalize -- normalize a vector with x,y,z components

//Combine_colors -- additively combine two colors
PColor Combine_colors(PColor color1, PColor color2) {
	PColor CombineColor;

	if ((color1.A + color2.A) > 255)
	{
		CombineColor.A = 255;
	}
	else {
		CombineColor.A = (color1.A + color2.A);
	}

	if ((color1.R + color2.R) > 255) {
		CombineColor.R = 255;
	}
	else {
		CombineColor.R = (color1.R + color2.R);
	}

	if ((color1.G + color2.G) > 255) {
		CombineColor.G = 255;
	}
	else {
		CombineColor.G = (color1.G + color2.G);
	}

	if ((color1.B + color2.B) > 255) {
		CombineColor.B = 255;
	}
	else {
		CombineColor.B = (color1.B + color2.B);
	}

	return CombineColor;
}

//Modulate_Colors -- multiplicatively combine two colors
PColor Modulate_Colors(PColor color1, PColor color2) {
	PColor ModulateColor;

	//TODO: add if statements like in "Combine_colors"'s body
	//Alpha
	ModulateColor.A = ((color1.A * color2.A) / 255);

	//red
	ModulateColor.R = ((color1.R * color2.R) / 255);

	//green
	ModulateColor.G = ((color1.G * color2.G) / 255);

	//blue
	ModulateColor.B = ((color1.B * color2.B) / 255);

	return ModulateColor;
}

//saturate -- clamps a value between 0 and 1

//vec3_length -- computes the vectors length