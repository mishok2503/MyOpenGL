#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>

#include "matrix.h"
#include "tgaimage.h"
#include "vector.h"

class Model {
private:
  struct Face {
    std::vector<int> point = std::vector<int>(3);
    std::vector<int> texture = std::vector<int>(3);
    std::vector<int> normal = std::vector<int>(3);
    int texture_num = 0;
  };

  std::vector<Vector3d> point, texture, normal;
  std::vector<Face> face;

  Vector3d light = {1, 0, 0};
  Vector3d eye = {1, 1, 1};
  Vector3d up = {0, 1, 0};

  TGAImage diffusemap[2];
  TGAImage normalsmap[2];

  double max_cord = 0;

  void triangle(TGAImage &image, int face_num, std::vector<std::vector<double>>& zbuffer);

  Vector2i world_to_screen(Vector3d &p, TGAImage &image);
  Matrix<4, 4, double> viewport(int w, int h);
  Matrix<4, 4, double> lookat(Vector3d center);

public:
  Model(const std::string &);

  bool load_from_file(const std::string &path);
  bool load_texture(const std::string &path, const std::string& type, size_t num);
  void render(TGAImage &image);
  void set_light(const Vector3d& light);
  void set_up(const Vector3d& up);
  void set_eye(const Vector3d& eye);

};

#endif // __MODEL_H__