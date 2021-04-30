#include "tgaimage.h"

#include <cstring>
#include <iostream>
#include <string>
#include "model.h"

int main(int argc, char** argv) {
  if (argc == 2 && strcmp(argv[1], "--help") == 0) {
    std::cout << "Usage: render [options] input-file output-file\n";
    std::cout << "Render .obj file to .tga image.\n\n";
    std::cout << "Options:\n"
    "-d \"file\"\t\tload diffuse map for model\n"
    "-n \"file\"\t\tload normals map for model\n"
    "-l \"x\" \"y\" \"z\"\t\tset light direction\n"
    "-г \"x\" \"y\" \"z\"\t\tset \"up\" direction\n"
    "-e \"x\" \"y\" \"z\"\t\tset eye position\n"
    "-s \"x\" \"y\" \t\tset output image size\n";
    return 0;
  }

  if (argc < 3) {
    std::cout << "Usage: render [options] input-file output-file\n";
    std::cout << "Or try \"render --help\"\n";
    return 1;
  }

  std::string out_file;
  int im_width = 800;
  int im_height = 800;
  Model model; 
  int non_key_params = 0; 

  for (int i=1; i < argc; ++i) {
    if (argv[i][0] != '-') {
      if (non_key_params == 0) {
        model.load_from_file(argv[i]);
        non_key_params = 1;
      } else if (non_key_params == 1) {
        out_file = argv[i];
        non_key_params = 2;
      } else {
        std::cout << "Extra parametrs\n";
        std::cout << "Try \"render --help\"\n";
        return 1;
      }
    }
  }

  TGAImage image(im_width, im_height, TGAImage::RGB);
  model.load_texture("res/models/Nigger/nigger_texture.tga", "texture");
  model.load_texture("res/models/Nigger/nigger_normals.tga", "normals");
  model.render(image);
  image.flip_vertically();
  image.write_tga_file(out_file.c_str());

  return 0;
}