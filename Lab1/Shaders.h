#pragma once
#include "MiniClasses.h"
#include <iostream>


//made a pointer function 
void (*VertexShader) (Vertex&) = 0;

Matrix4x4 VS_World;

Matrix4x4 VS_View;

Matrix4x4 VS_Projection;

float DotProduct(Vertex v1, Vertex v2)
{	
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z) + (v1.w * v2.w);
}

Matrix4x4 MultiplyMatrixByMatrix(const Matrix4x4 &matrix1, const Matrix4x4& matrix2)
{
	Matrix4x4 mOutput;

	Vertex Column1(matrix2.xx, matrix2.yx, matrix2.zx, matrix2.wx);
	Vertex Column2(matrix2.xy, matrix2.yy, matrix2.zy, matrix2.wy);
	Vertex Column3(matrix2.xz, matrix2.yz, matrix2.zz, matrix2.wz);
	Vertex Column4(matrix2.xw, matrix2.yw, matrix2.zw, matrix2.ww);

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
	
	Vertex NewVertex;

	//xx 	yx 	zx	wx  x = (xx * x) + (yx * y) + (zx * z) + (wx * w)
	//xy 	yy 	zy	wy  y = (xy * x) + (yy * y) + (zy * z) + (wy * w)
	//xz 	yz 	zz	wz  z = (xz * x) + (yz * y) + (zz * z) + (wz * w)
	//xw 	yw 	zw	ww  w = (xw * x) + (yw * y) + (zw * z) + (ww * w)

	
	NewVertex.x = (matrix4.xx * vertex4.x) + (matrix4.yx * vertex4.y) + (matrix4.zx * vertex4.z) + (matrix4.wx * vertex4.w);
	NewVertex.y = (matrix4.xy * vertex4.x) + (matrix4.yy * vertex4.y) + (matrix4.zy * vertex4.z) + (matrix4.wy * vertex4.w);
	NewVertex.z = (matrix4.xz * vertex4.x) + (matrix4.yz * vertex4.y) + (matrix4.zz * vertex4.z) + (matrix4.wz * vertex4.w);
	NewVertex.w = (matrix4.xw * vertex4.x) + (matrix4.yw * vertex4.y) + (matrix4.zw * vertex4.z) + (matrix4.ww * vertex4.w);


	vertex4 = NewVertex;

	return NewVertex;
}

void VS_WVP(Vertex& vert) {

	vert = MultiplyMatrixByVertex(VS_World, vert);
	vert = MultiplyMatrixByVertex(VS_View, vert);
	vert = MultiplyMatrixByVertex(VS_Projection, vert);
	vert.x /= vert.w;
	vert.y /= vert.w;
	vert.z /= vert.w;
}
