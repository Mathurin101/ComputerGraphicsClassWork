#pragma once
#include "MiniClasses.h"
#include "flower.h"
#include "StoneHenge.h"
#include "StoneHenge_Texture.h"
#include <iostream>


//made a pointer function 
void (*VertexShader) (Vertex&) = 0;
PColor(*PixelShader) (Triangle&, const BarycentricCoord&) = 0;

//shadder
Vertex ColorShade(-0.577, -0.577, 0.577);
PColor ShadeAm(0xFFC0C0F0);

Matrix4x4 VS_World;

Matrix4x4 VS_View;

Matrix4x4 VS_Projection;

float DotProduct(Coord v1, Coord v2)
{	
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z) + (v1.w * v2.w);
}

Matrix4x4 MultiplyMatrixByMatrix(const Matrix4x4 &matrix1, const Matrix4x4& matrix2)
{
	Matrix4x4 mOutput;

	Coord Column1(matrix2.xx, matrix2.yx, matrix2.zx, matrix2.wx);
	Coord Column2(matrix2.xy, matrix2.yy, matrix2.zy, matrix2.wy);
	Coord Column3(matrix2.xz, matrix2.yz, matrix2.zz, matrix2.wz);
	Coord Column4(matrix2.xw, matrix2.yw, matrix2.zw, matrix2.ww);

	mOutput.xx = DotProduct(matrix1.AxisX, Column1);
    mOutput.xy = DotProduct(matrix1.AxisX, Column2);
	mOutput.xz = DotProduct(matrix1.AxisX, Column3);
	mOutput.xw = DotProduct(matrix1.AxisX, Column4);

	mOutput.yx = DotProduct(matrix1.AxisY, Column1);
	mOutput.yy = DotProduct(matrix1.AxisY, Column2);
	mOutput.yz = DotProduct(matrix1.AxisY, Column3);
	mOutput.yw = DotProduct(matrix1.AxisY, Column4);

	mOutput.zx = DotProduct(matrix1.AxisZ, Column1);
	mOutput.zy = DotProduct(matrix1.AxisZ, Column2);
	mOutput.zz = DotProduct(matrix1.AxisZ, Column3);
	mOutput.zw = DotProduct(matrix1.AxisZ, Column4);

	mOutput.wx = DotProduct(matrix1.AxisW, Column1);
	mOutput.wy = DotProduct(matrix1.AxisW, Column2);
	mOutput.wz = DotProduct(matrix1.AxisW, Column3);
	mOutput.ww = DotProduct(matrix1.AxisW, Column4);

	return mOutput;
}

Vertex MultiplyMatrixByVertex(Matrix4x4& matrix4, Vertex& vertex4) {
	
	Vertex NewVertex = vertex4;

	//xx 	yx 	zx	wx  x = (xx * x) + (yx * y) + (zx * z) + (wx * w)
	//xy 	yy 	zy	wy  y = (xy * x) + (yy * y) + (zy * z) + (wy * w)
	//xz 	yz 	zz	wz  z = (xz * x) + (yz * y) + (zz * z) + (wz * w)
	//xw 	yw 	zw	ww  w = (xw * x) + (yw * y) + (zw * z) + (ww * w)

	
	NewVertex.cord.x = (matrix4.xx * vertex4.cord.x) + (matrix4.yx * vertex4.cord.y) + (matrix4.zx * vertex4.cord.z) + (matrix4.wx * vertex4.cord.w);
	NewVertex.cord.y = (matrix4.xy * vertex4.cord.x) + (matrix4.yy * vertex4.cord.y) + (matrix4.zy * vertex4.cord.z) + (matrix4.wy * vertex4.cord.w);
	NewVertex.cord.z = (matrix4.xz * vertex4.cord.x) + (matrix4.yz * vertex4.cord.y) + (matrix4.zz * vertex4.cord.z) + (matrix4.wz * vertex4.cord.w);
	NewVertex.cord.w = (matrix4.xw * vertex4.cord.x) + (matrix4.yw * vertex4.cord.y) + (matrix4.zw * vertex4.cord.z) + (matrix4.ww * vertex4.cord.w);


	vertex4 = NewVertex;

	return NewVertex;
}

void VS_WVP(Vertex& vert) {

	vert = MultiplyMatrixByVertex(VS_World, vert);
	vert = MultiplyMatrixByVertex(VS_View, vert);
	vert = MultiplyMatrixByVertex(VS_Projection, vert);
	vert.cord.x /= vert.cord.w;
	vert.cord.y /= vert.cord.w;
	vert.cord.z /= vert.cord.w;
}

PColor BGRA2ARGB(unsigned int C)
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
int Convert2DTWO1D(int nX, int nY, int nWidth)
{

	//return a 1d coordinate using the 2D->1D formula
	return ((nY * nWidth) + nX);
}
PColor VS_PixelShadder(Triangle& Tri, const BarycentricCoord& Bary) {
	float u;
	float v;
	int Position;
	PColor _Color;
	
	u = (Tri.A.u * Bary.Alpha) + (Tri.B.u * Bary.Beta) + (Tri.C.u * Bary.Gamma);
	v = (Tri.A.v * Bary.Alpha) + (Tri.B.v * Bary.Beta) + (Tri.C.v * Bary.Gamma);
	
	u *= flower_width;
	v *= flower_height;
	
	Position = Convert2DTWO1D(u, v, flower_width);
	
	_Color.color = flower_pixels[Position];
	
	_Color = BGRA2ARGB(_Color.color);

	return _Color;// _Color;
}

PColor VS_PixelShadderH(Triangle& Tri, const BarycentricCoord& Bary) {
	float u;
	float v;
	int Position;
	PColor _Color;

	u = (Tri.A.u * Bary.Alpha) + (Tri.B.u * Bary.Beta) + (Tri.C.u * Bary.Gamma);
	v = (Tri.A.v * Bary.Alpha) + (Tri.B.v * Bary.Beta) + (Tri.C.v * Bary.Gamma);

	u *= StoneHenge_width;
	v *= StoneHenge_height;

	Position = Convert2DTWO1D(u, v, StoneHenge_width);

	_Color.color = StoneHenge_pixels[Position];

	_Color = BGRA2ARGB(_Color.color);
	_Color = Combine_colors(_Color, ShadeAm);

	return _Color;// _Color;
}

PColor SoildColor(Triangle& Tri, const BarycentricCoord& Bary) {
	return 0xFFFFFFFF;
}