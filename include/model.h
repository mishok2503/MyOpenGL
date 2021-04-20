#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>

#include "matrix.h"
#include "tgaimage.h"
#include "vector.h"

// TODO: parametrs names in .h

class Model {
private:
  struct Face {
    std::vector<int> point = std::vector<int>(3);
    std::vector<int> texture = std::vector<int>(3);
    std::vector<int> normal = std::vector<int>(3);
  };

  std::vector<Vector3d> point, texture, normal;
  std::vector<Face> face;

  Vector3d light = {1, 0, 0};

  TGAImage diffusemap;
  TGAImage normalsmap;

  void triangle(TGAImage &image, int face_num, std::vector<std::vector<double>>& zbuffer);

  Vector2i world_to_screen(Vector3d &, TGAImage &);
  Matrix<4, 4, double> viewport(int w, int h);
  Matrix<4, 4, double> lookat(Vector3d eye, Vector3d center, Vector3d up);

public:
  Model(const std::string &);

  bool load_from_file(const std::string &);
  bool load_texture(const std::string &, const std::string);
  void render(TGAImage &);
  void set_light(const Vector3d& light);
};

#endif // __MODEL_H__