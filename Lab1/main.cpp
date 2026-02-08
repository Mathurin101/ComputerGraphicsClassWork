#include "RasterSurface.h"
#include "Helper.h"

const char* Name = "MathurinGenty_ProgrammingAssignment 1";
const unsigned int PixelWidth = 500;
const unsigned int PixelHeight = 500;

const unsigned int MaxPixels = PixelWidth * PixelHeight;

unsigned int TotalPixels[MaxPixels];

int WinMain()
{
    //Position scrRect[4] = { 149,44, 190, 95 };
    Position scrRect(149, 44, 190, 95);

    PColor Color(0xFF8e6acc);//purple

    RS_Initialize(Name, PixelWidth, PixelHeight);

    //will print on the screen
    do {
        
        CCBuffer(Color.color, TotalPixels, MaxPixels);



    } while (RS_Update(TotalPixels, MaxPixels));

    RS_Shutdown();

    return 0;
}
