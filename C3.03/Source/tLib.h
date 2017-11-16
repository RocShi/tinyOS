#ifndef TLIB_H
#define TLIB_H

#include <stdint.h>

typedef struct		//Bitmap data structure
{
	uint32_t bitmap;
}tBitmap;

void tBitmapInit(tBitmap *bitmap);									//Bitmap initiate
void tBitmapSet(tBitmap *bitmap, uint32_t pos);			//Set one bit of a bitmap 
void tBitmapClear(tBitmap *bitmap, uint32_t pos);		//Clear one bit of a bitmap
uint32_t tBitmapGetFirstSet(tBitmap *bitmap);				//Get the first setted bit of a bitmap
uint32_t tBitmapPosCount(void);											//Total setted bits of a bitmap

#endif
