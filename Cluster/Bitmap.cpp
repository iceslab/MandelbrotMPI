#include "Bitmap.h"

void saveBitmap(int * _image, int _x, int _y, String _name)
{
	for (int x = 0; x < _x; x++)
    {
    	for (int y = 0; y < _y; y++)
        {
        	double &color = image[y * _x + x];
            r = (int)round((sin( 2*M_PI*color - M_PI/2 + M_PI / 3) + 1) * 255.0);
            g = (int)round((sin( 2*M_PI*color - M_PI/2           ) + 1) * 255.0);
            b = (int)round((sin( 2*M_PI*color - M_PI/2 - M_PI / 3) + 1) * 255.0);
            image.set_pixel(x, y, r, g, b);
        }
    }
    image.save_image(_name);
}