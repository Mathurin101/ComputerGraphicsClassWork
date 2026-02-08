#pragma once

struct Position 
{
	int x;
	int y;

	int width;
	int height;

	Position(int _x, int _y, int _width = 0, int _height = 0) {
		x = _x; width = _width;
		y = _y; height = _height;
	}
};

struct PColor {
	unsigned int color;	//0xAARRBBGG
	unsigned int A;//0xFF000000 = AA
	unsigned int R;//0x00FF0000 = RR
	unsigned int G;//0x000000FF = GG	
	unsigned int B;//0x0000FF00 = BB

	PColor() {
		color = 0;
		A = 0;
		R = 0;
		G = 0;
		B = 0;
	}

	PColor(unsigned int _color) {
		color = _color;
		A = (color >> 24) ;//0xFF000000 = AA
		R = (color & 0x00FF0000) >> 16;//0x00FF0000 = RR
		G = (color & 0x000000FF);//0x000000FF = GG
		B = (color & 0x0000FF00) >> 8;//0x0000FF00 = BB
	}

	PColor CombineColor() {
		return color = (A | R | G | B );
	}	
};