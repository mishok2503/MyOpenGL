#include "tgaimage.h"

int main()
{
	TGAImage image(200, 200, TGAImage::RGB);

	image.write_tga_file("out/test.tga");

	return 0;
}