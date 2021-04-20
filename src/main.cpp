#include "tgaimage.h"

#include "model.h"

#include <iostream> //TODO: remove

int main() {
  TGAImage image(800, 800, TGAImage::RGB);

  Model nigga("res/models/Nigger/nigger.obj");
  nigga.load_texture("res/models/Nigger/nigger_texture.tga", "texture");
  nigga.load_texture("res/models/Nigger/nigger_normals.tga", "normals");

  nigga.render(image);

  image.flip_vertically();
  image.write_tga_file("out/test.tga");

  return 0;
}