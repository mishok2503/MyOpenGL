#include "tgaimage.h"

#include "model.h"
#include <cmath>//TODO: remove
#include <string>

int main() {

  Model model("res/models/Gun/Gun.obj");
  model.load_texture("res/models/Gun/Gun_DIFF.tga", "texture", 0);
  model.load_texture("res/models/Gun/Gun_NRM.tga", "normals", 0);
  model.load_texture("res/models/Gun/Gun1_DIFF.tga", "texture", 1);
  model.load_texture("res/models/Gun/Gun1_NRM.tga", "normals", 1);


  TGAImage image(3000, 3000, TGAImage::RGB);

  // for (int i=0; i < 60; ++i) {
  // 	// model.set_eye({0, 0.5, 1});
  // 	// model.set_up({0, 1, 1});
  // 	model.set_eye({i * 0.06, 1, 1});
  // 	model.set_up({0, 1, 0});

	model.render(image);

	image.flip_vertically();
	image.write_tga_file("out/result.tga");
	// image.clear();
 //  }

  return 0;
}