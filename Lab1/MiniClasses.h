#pragma once

struct Position 
{
	int x;
	int y;

	int width;
	int height;

	Position(int _x, int _y, int _width, int _height) {
		x = _x; width = _width;
		y = _y; height = _height;
	}


};

struct PColor {
	unsigned color;	//0xAARRBBGG
	unsigned A;//0xFF000000 = AA
	unsigned R;//0x00FF0000 = RR
	unsigned B;//0x0000FF00 = BB
	unsigned G;//0x000000FF = GG	

	PColor();

	PColor(unsigned _color) {
		color = _color;
		A = (color & 0xFF000000);//0xFF000000 = AA
		R = (color & 0x00FF0000);//0x00FF0000 = RR
		B = (color & 0x0000FF00);//0x0000FF00 = BB
		G = (color & 0x000000FF);//0x000000FF = GG
	}

	void CombineColor() {
		color = (A | R | B | G);
	}

	SetColor(unsigned ) {

	}
	
};