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


    PColor Color(0xFF8e6acc);//purple

    PColor ColorG(0xFF123542);//green

    RS_Initialize(Name, PixelWidth, PixelHeight);

    //will print on the screen
    do {
        
        //CCBuffer(Color.color, TotalPixels, MaxPixels);

        //DrawPixel(72, ColorG.color, TotalPixels, MaxPixels);

        BLIT(scrRect, RasterPos, tiles_12_pixels, TotalPixels);



    } while (RS_Update(TotalPixels, MaxPixels));

    RS_Shutdown();

    return 0;
}
