#include "tgaimage.h"

#include "model.h"
#include <cmath>//TODO: remove
#include <string>

int main() {

  Model nigga("res/models/Nigger/nigger.obj");
  nigga.load_texture("res/models/Nigger/nigger_texture.tga", "texture");
  nigga.load_texture("res/models/Nigger/nigger_normals.tga", "normals");

int i = 0;
  for (double a = 0; a < 2 * 3.1415; a += 0.1, ++i)
  {
  	TGAImage image(800, 800, TGAImage::RGB);
  	nigga.set_light({std::sin(a), std::sin(a), std::cos(a)});
  nigga.render(image);

  image.flip_vertically();
  image.write_tga_file((std::string("out/") + std::to_string(i)).c_str());
  }

  return 0;
}