#include "tgaimage.h"

#include "primitives.h"

int main()
{
	TGAImage image(200, 200, TGAImage::RGB);

	line(image, {100, 100}, {200, 200}, {255, 255, 255, 0});

	image.write_tga_file("out/test.tga");

	return 0;
}