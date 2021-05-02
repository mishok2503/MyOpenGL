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
    "-l \"x y z\"\t\tset light direction\n"
    "-u \"x y z\"\t\tset \"up\" direction\n"
    "-e \"x y z\"\t\tset eye position\n"
    "-s \"x y\" \t\tset output image size\n";
    return 0;
  }

  if (argc < 3) {
    std::cerr << "Usage: render [options] input-file output-file\n"
                 "Or try \"render --help\"\n";
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
        if (!model.load_from_file(argv[i]))
          return 1;
        non_key_params = 1;
      } else if (non_key_params == 1) {
        out_file = argv[i];
        non_key_params = 2;
      } else {
        std::cerr << "Extra parametrs\n";
        std::cerr << "Try \"render --help\"\n";
        return 1;
      }
    } else {
      if (strlen(argv[i]) != 2) {
        std::cerr << "Invalid option: " << argv[i] << "\ntry \"render --help\"\n";
        return 1;
      }

      auto read_vector3d = [](const char* str) {
          char* next = nullptr;
          double x = strtod(str, &next);
          double z = strtod(next, &next);
          double y = strtod(next, nullptr);
          return Vector3d(x, y, z);
      };

      switch (argv[i++][1]) {
        case 'd':
          if (!model.load_texture(argv[i], "texture"))
            return 1;
          break;
        case 'n':
          if (!model.load_texture(argv[i], "normals"))
            return 1;
          break;
        case 's': {
          char* next = nullptr;
          im_width = strtol(argv[i], &next, 10);
          im_height = strtol(next, nullptr, 10);
          if (im_width <= 0 || im_height <= 0) {
            std::cerr << "Image size must be positive\n";
            return 1;
          }
        } break;
        case 'l': {
          auto light = read_vector3d(argv[i]);
          if (light == Vector3d(0, 0, 0)) {
            std::cerr << "Light must be non zero vector\n";
            return 1;
          }
          model.set_light(light);
        } break;
        case 'u': {
          auto up = read_vector3d(argv[i]);
          if (up == Vector3d(0, 0, 0)) {
            std::cerr << "\"Up\" must be non zero vector\n";
            return 1;
          }
          model.set_up(up);
        } break;
        case 'e':
          model.set_eye(read_vector3d(argv[i]));
          break;
        default:
          std::cerr << "Invalid option: " << argv[i - 1] << "\ntry \"render --help\"\n";
          return 1;
      }
    }
  }

  TGAImage image(im_width, im_height, TGAImage::RGB);
  model.render(image);
  image.flip_vertically();
  image.write_tga_file(out_file.c_str());

  return 0;
}