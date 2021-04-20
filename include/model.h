#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>

#include "matrix.h"
#include "primitives.h"
#include "tgaimage.h"
#include "vector.h"

// TODO: parametrs names in .h

struct Face {
  std::vector<int> point = std::vector<int>(3);
  std::vector<int> texture = std::vector<int>(3);
  std::vector<int> normal = std::vector<int>(3);
};

class Model {
private:
  std::vector<Vector3d> point;
  std::vector<Vector3d> texture;
  std::vector<Vector3d> normal;
  std::vector<Face> face;

  TGAImage diffusemap;
  TGAImage normalsmap;

  void triangle(TGAImage &, int, std::vector<std::vector<double>> &,
                const Vector3d &);

  Vector2i world_to_screen(Vector3d &, TGAImage &);
  Matrix<4, 4, double> viewport(int w, int h);
  Matrix<4, 4, double> lookat(Vector3d eye, Vector3d center, Vector3d up);

public:
  Model(const std::string &);

  bool load_from_file(const std::string &);
  bool load_texture(const std::string &, const std::string);
  void render(TGAImage &);
};

#endif // __MODEL_H__