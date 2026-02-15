#pragma once
#include "MiniClasses.h"
#include <iostream>

Matrix4x4 VS_World;


//made a pointer function 
void (*VertexShader) (Vertex&) = 0;

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

void MultiplyMatrixByVertex(Matrix4x4& matrix4, Vertex& vertex4) {

		//xx 	yx 	zx	wx  x = (xx * x) + (yx * x) + (zx * x) + (wx * x)
		//xy 	yy 	zy	wy  y = (xy * y) + (yy * y) + (zy * y) + (wy * y)
		//xz 	yz 	zz	wz  z = (xz * z) + (yz * z) + (zz * z) + (wz * z)
		//xw 	yw 	zw	ww  w = (xw * w) + (yw * w) + (zw * w) + (ww * w)

	vertex4.x = (matrix4.xx * vertex4.x) + (matrix4.yx * vertex4.x) + (matrix4.zx * vertex4.x) + (matrix4.wx * vertex4.x);
	vertex4.y = (matrix4.xy * vertex4.y) + (matrix4.yy * vertex4.y) + (matrix4.zy * vertex4.y) + (matrix4.wy * vertex4.y);
	vertex4.z = (matrix4.xz * vertex4.z) + (matrix4.yz * vertex4.z) + (matrix4.zz * vertex4.z) + (matrix4.wz * vertex4.z);
	vertex4.w = (matrix4.xw * vertex4.w) + (matrix4.yw * vertex4.w) + (matrix4.zw * vertex4.w) + (matrix4.ww * vertex4.w);
}

void VS_WVP(Vertex& vert) {

	MultiplyMatrixByVertex(VS_World, vert);
}