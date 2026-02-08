#include "RasterSurface.h"
#include "Helper.h"
#include "XTime.h"
#include "tiles_12.h"

const char* Name = "MathurinGenty_ProgrammingAssignment 1";
const unsigned int PixelWidth = 500;
const unsigned int PixelHeight = 500;

const unsigned int MaxPixels = PixelWidth * PixelHeight;

unsigned int TotalPixels[MaxPixels];

int main()
{
    //Position scrRect[4] = { 149,44, 190, 95 };
    Position scrRect(0, 0, tiles_12_width, tiles_12_height);
	Position RasterPos(0, 0, PixelWidth, PixelHeight);

    Position GrassRect(288, 128, 32, 32);

    PColor ColorOG (0xFF8e6acc);//purple

    PColor ColorCH (0xFF123524);//green
                   
    RS_Initialize(Name, PixelWidth, PixelHeight);

    //will print on the screen
    do {
        
        //CCBuffer(ColorOG.color, TotalPixels, MaxPixels);
        //CCBuffer(BGRAtoARGB(ColorCH.color).color, TotalPixels, MaxPixels);

        //DrawPixel(72, ColorG.color, TotalPixels, MaxPixels);

        //BLIT(scrRect, RasterPos, tiles_12_pixels, TotalPixels, tiles_12_width);
        //BLIT(Position(0,0,100,100), RasterPos, tiles_12_pixels, TotalPixels, tiles_12_width);
        //BLIT(GrassRect, RasterPos, tiles_12_pixels, TotalPixels, tiles_12_width);

        DrawTile(GrassRect, RasterPos, tiles_12_pixels, TotalPixels, tiles_12_width);

        
    } while (RS_Update(TotalPixels, MaxPixels));

    RS_Shutdown();

    return 0;
}

