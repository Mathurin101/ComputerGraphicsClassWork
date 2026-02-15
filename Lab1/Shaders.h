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
	
	Vertex NewVertex;

		//might not work
		//xx 	yx 	zx	wx  x = (xx * x) + (yx * y) + (zx * z) + (wx * w)
		//xy 	yy 	zy	wy  y = (xy * x) + (yy * y) + (zy * z) + (wy * w)
		//xz 	yz 	zz	wz  z = (xz * x) + (yz * y) + (zz * z) + (wz * w)
		//xw 	yw 	zw	ww  w = (xw * x) + (yw * y) + (zw * z) + (ww * w)

		
		//xx 	xy 	xz	xw  x = (xx * x) + (xy * y) + (xz * z) + (xw * w)
		//yx 	yy 	yz	yw  y = (yx * x) + (yy * y) + (yz * z) + (yw * w)
		//zx 	zy 	zz	zw  z = (zx * x) + (zy * y) + (zz * z) + (zw * w)
		//wx 	wy 	wz	ww  w = (wx * x) + (wy * y) + (wz * z) + (ww * w)


	//this doesn't work 
	std::cout << "Before-x: " << NewVertex.x << std::endl;
	NewVertex.x = (matrix4.xx * vertex4.x) + (matrix4.xy * vertex4.y) + (matrix4.xz * vertex4.z) + (matrix4.xw * vertex4.w);
	std::cout << "After-x: " << NewVertex.x << std::endl;
	
	std::cout << "Before-y: " << NewVertex.y << std::endl;
	NewVertex.y = (matrix4.yx * vertex4.x) + (matrix4.yy * vertex4.y) + (matrix4.yz * vertex4.z) + (matrix4.yw * vertex4.w);
	std::cout << "After-y: " << NewVertex.y << std::endl;
	
	std::cout << "Before-z: " << NewVertex.y << std::endl;
	NewVertex.z = (matrix4.zx * vertex4.x) + (matrix4.zy * vertex4.y) + (matrix4.zz * vertex4.z) + (matrix4.zw * vertex4.w);
	std::cout << "After-z: " << NewVertex.z << std::endl;
	
	std::cout << "Before-w: " << NewVertex.w << std::endl;
	NewVertex.w = (matrix4.wx * vertex4.x) + (matrix4.wy * vertex4.y) + (matrix4.wz * vertex4.z) + (matrix4.ww * vertex4.w);
	std::cout << "After-w: " << NewVertex.w << std::endl;

	vertex4 = NewVertex;

}

void VS_WVP(Vertex& vert) {

	MultiplyMatrixByVertex(VS_World, vert);
}


