#pragma once
#include "Helper.h"

struct Position
{
	int x;
	int y;

	int width;
	int height;



	Position(int _x, int _y, int _width = 0, int _height = 0) {
		x = _x;  width = _width;
		y = _y; height = _height;
	}
};

struct Points {
	int x1;
	int y1;
	int x2;
	int y2;
	int slope;
	int deltaX;
	int deltaY;

	Points() {
		x1 = 0;
		y1 = 0;
		x2 = 0;
		y2 = 0;
		slope = 0;
		deltaX = 0;
		deltaY = 0;
	}


	Points(int _x1 = 0, int _y1 = 0, int _x2 = 0, int _y2 = 0) {
		x1 = _x1;	 x2 = _x2;
		y1 = _y1;	 y2 = _y2;
		deltaY = (y2 - y1);
		deltaX = (x2 - x1);
		//slope = deltaY / deltaX;
	}
	Points(Position start, Position end) {
		x1 = start.x;
		y1 = start.y;

		x2 = end.x;
		y2 = end.y;
	}

};

struct PColor {
	unsigned int color;//0xAARRBBGG
	unsigned int A;    //0xFF000000 = AA
	unsigned int R;    //0x00FF0000 = RR
	unsigned int G;    //0x0000FF00 = GG	
	unsigned int B;    //0x000000FF = BB

	PColor() {
		color = 0;
		A = 0;
		R = 0;
		G = 0;
		B = 0;
	}

	PColor(unsigned int _color) {
		color = _color;
		A = (color >> 24);			   //0xFF000000 = AA
		R = (color & 0x00FF0000) >> 16;//0x00FF0000 = RR
		G = (color & 0x0000FF00) >> 8; //0x0000FF00 = GG
		B = (color & 0x000000FF);	   //0x000000FF = BB
	}

	unsigned int CombineColor() {
		color |= (A << 24);
		color |= (R << 16);
		color |= (G << 8);
		color |= B;

		return color;
	}
};
struct Coord
{
	union
	{
		struct
		{
			float xyzw[4];
		};
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};

	};

	Coord(float _x = 0, float _y = 0, float _z = 0, float _w = 1.0f) {
		x = _x; y = _y; z = _z; w = _w;
	}
};
struct Vertex
{
	
	Coord cord;

	float u;
	float v;

	Vertex() {
		for (int i = 0; i < 4; i++) {
			cord.xyzw[i] = 0;
		}
	}
	Vertex(float _x, float _y, float _z, float _w = 1.0f, float _u = 0, float _v = 0) {
		cord.x = _x; cord.y = _y; cord.z = _z; cord.w = _w;
		u = _u; v = _v;
	}

	Vertex(Vertex _A, float _u, float _v) {
		cord.x = _A.cord.x; cord.y = _A.cord.y; cord.z = _A.cord.z; cord.w = _A.cord.w;
		u = _u; v = _v;
	}
};

struct Matrix4x4
{
	union
	{
		struct
		{
			float Varray[16];
		};
		struct
		{
			float  xx;
			float  xy;
			float  xz;
			float  xw;

			float  yx;
			float  yy;
			float  yz;
			float  yw;

			float  zx;
			float  zy;
			float  zz;
			float  zw;

			float  wx;
			float  wy;
			float  wz;
			float  ww;
		};
		struct
		{
			Coord AxisX;
			Coord AxisY;
			Coord AxisZ;
			Coord AxisW;
		};

	};
	Matrix4x4() {
		for (int i = 0; i < 16; i++) {
			Varray[i] = 0;
		}

	};

	Matrix4x4(float _xx, float _xy, float _xz, float _xw,
		float _yx, float _yy, float _yz, float _yw,
		float _zx, float _zy, float _zz, float _zw,
		float _wx, float _wy, float _wz, float _ww) {

		xx = _xx; xy = _xy; xz = _xz; xw = _xw;
		yx = _yx; yy = _yy; yz = _yz; yw = _yw;
		zx = _zx; zy = _zy; zz = _zz; zw = _zw;
		wx = _wx; wy = _wy; wz = _wz; ww = _ww;

	};



};

struct Triangle {
	union {
		struct {
			Vertex LineABC[3];
		};
		struct {
			Vertex A;
			Vertex B;
			Vertex C;
		};
	};

	Triangle() {
		A = Vertex(0,0,0);
		B = Vertex(0,0,0);
		C = Vertex(0,0,0);
	}

	Triangle(Vertex _A, Vertex _B, Vertex _C) {
		A = _A; B = _B; C = _C;
	}

};

struct BarycentricCoord {
	union {
		struct {
			float bya[3];
		};
		struct {
			float Beta;
			float Gamma;//y
			float Alpha;
		};
	};

	BarycentricCoord(float _Alpha = 0, float _Beta = 0, float _Gamma = 0) {
		Alpha = _Alpha; Beta = _Beta; Gamma = _Gamma;
	}

};
