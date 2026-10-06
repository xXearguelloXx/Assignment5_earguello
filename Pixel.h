#ifndef PIXEL_H
#define PIXEL_H

//One pixel from the image.
//r, g, b are color values from 0.0 to 1.0
//x, y are the pixel's location on the screen
struct _pixel
{
  float r;
  float g;
  float b;
  int x;
  int y;
};
typedef struct _pixel Pixel;

#endif
