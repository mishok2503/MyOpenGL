#include "model.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "vector.h"

namespace {

double triangle_area(const Vector2i &a, const Vector2i &b, const Vector2i &c) {
  return (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)) /
         (double)2;
}

Vector3d barycentric2d(const Vector2i &a, const Vector2i &b, const Vector2i &c,
                       const Vector2i &p, const double area) {
  double s1 = triangle_area(p, b, c);
  double s2 = triangle_area(a, p, c);
  return Vector3d(s1, s2, area - s1 - s2) / area;
}

} // namespace

Matrix<4, 4, double> Model::viewport(int w, int h) {
  Matrix<4, 4, double> m(1);
  int depth = 255;
  m[0][3] = w / 2.0;
  m[1][3] = h / 2.0;
  m[2][3] = depth / 2.0;

  m[0][0] = w * 3 / 8.0;
  m[1][1] = h * 3 / 8.0;
  m[2][2] = depth / 2.0;
  return m;
}

Matrix<4, 4, double> Model::lookat(Vector3d center) {
  Vector3d z = eye - center;
  Vector3d x = up ^ z;
  Vector3d y = z ^ x;
  x.normalize();
  y.normalize();
  z.normalize();
  Matrix<4, 4, double> res(1);
  for (int i = 0; i < 3; i++) {
    res[0][i] = x[i];
    res[1][i] = y[i];
    res[2][i] = z[i];
    res[i][3] = -center[i];
  }
  return res;
}

Vector2i Model::world_to_screen(Vector3d &p, TGAImage &image) {
  double c = 4;
  Matrix<4, 4, double> vp = viewport(image.get_width(), image.get_height());
  Matrix<4, 4, double> lt = lookat({-0.4, 0, 0});
  Matrix<4, 4, double> pr(
      {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, -1 / c, 1}});
  Matrix<4, 1, double> world({{p.x}, {p.y}, {p.z}, {1}});
  world = vp * pr * lt * world;
  return Vector2i(world[0][0] / world[3][0], world[1][0] / world[3][0]);
}

void Model::triangle(TGAImage &image, int face_num,
                     std::vector<std::vector<double>> &zbuffer) {
  Vector3d world[3], texture_p[3];
  Vector2i screen[3];
  for (int i = 0; i < 3; ++i) {
    world[i] = point[face[face_num].point[i]] / max_cord;
    screen[i] = world_to_screen(world[i], image);
    texture_p[i] = texture[face[face_num].texture[i]];
  }

  int im_w = image.get_width();
  int im_h = image.get_height();

  Vector2i lb(im_w - 1, im_h - 1), rt(0, 0);
  for (int i = 0; i < 3; ++i) {
  	for (int j=0; j < 2; ++j) {
	  lb[j] = std::min(lb[j], screen[i][j]);
      rt[j] = std::max(rt[j], screen[i][j]);
	  }
  }


  double area = triangle_area(screen[0], screen[1], screen[2]);
  for (Vector2i pt(lb.x, 0); pt.x < rt.x; ++pt.x) {
    for (pt.y = lb.y; pt.y < rt.y; ++pt.y) {
      if (pt.x < 0 || pt.x >= im_w || pt.y < 0 || pt.y >= im_h)
        continue;
      Vector3d bc = barycentric2d(screen[0], screen[1], screen[2], pt, area);

      if (bc.x >= 0 && bc.y >= 0 && bc.z >= 0) {
        double z = world[0].z * bc.x + world[1].z * bc.y + world[2].z * bc.z; // TODO: v * v
        if (z > zbuffer[pt.x][pt.y]) {
          zbuffer[pt.x][pt.y] = z;
          double xd = Vector3d(texture_p[0].x, texture_p[1].x, texture_p[2].x) * bc;
          double yd = Vector3d(texture_p[0].y, texture_p[1].y, texture_p[2].y) * bc;
          TGAColor color = {255, 255, 255, 0};
          if (has_diffusemap)
            color = diffusemap.get(xd * diffusemap.get_width() + 0.5,
                                   yd * diffusemap.get_height() + 0.5);
          Vector3d n;
          if (has_normalsmap) {
            TGAColor nc = normalsmap.get(xd * normalsmap.get_width() + 0.5,
                                        normalsmap.get_height() * yd + 0.5);
            n = Vector3d(nc.r, nc.g, nc.b);
          } else {
            std::vector<int> norms = face[face_num].normal;
            n = normal[norms[0]] * bc.x + normal[norms[1]] * bc.y + normal[norms[2]] * bc.z;
          }
          n.normalize();
          double intensivity = n * light;
          if (intensivity < 0)
          	continue;
          color.r *= intensivity;
          color.g *= intensivity;
          color.b *= intensivity;
          image.set(pt.x, pt.y, color);
        }
      }
    }
  }
}

Model::Model(const std::string &path) { load_from_file(path); }

bool Model::load_from_file(const std::string &path) {
  std::ifstream ifs(path);

  if (ifs.fail()) {
    std::cerr << "Can't open " << path << '\n';
    return false;
  }

  char t;
  std::string line;
  while (!ifs.eof()) {
    std::getline(ifs, line);
    std::istringstream iss(line.c_str());

    if (line.compare(0, 2, "v ") == 0) {
      Vector3d p;
      iss >> t >> p.x >> p.y >> p.z;
      max_cord = std::max(std::abs(p.x), std::max(std::abs(p.y),
      						std::max(std::abs(p.z), max_cord)));

      point.push_back(p);
    } else if (line.compare(0, 3, "vt ") == 0) {
      Vector3d v;
      iss >> t >> t >> v.x >> v.y >> v.z;

      texture.push_back(v);
    } else if (line.compare(0, 3, "vn ") == 0) {
      Vector3d v;
      iss >> t >> t >> v.x >> v.y >> v.z;

      normal.push_back(v);
    } else if (line.compare(0, 2, "f ") == 0) {
      Face f;
      iss >> t;

      for (int i = 0; i < 3; ++i) {
        iss >> f.point[i] >> t >> f.texture[i] >> t >> f.normal[i];

        f.point[i]--;
        f.texture[i]--;
        f.normal[i]--;
      }
      face.push_back(f);
    }
  }

  ifs.close();
  return true;
}

bool Model::load_texture(const std::string &path, const std::string& type) {
  TGAImage t;
  if (!t.read_tga_file(path.c_str()))
    return false;
  t.flip_vertically();
  if (type == "texture") {
    diffusemap = t;
    has_diffusemap = true;
  }
  if (type == "normals") {
    normalsmap = t;
    has_normalsmap = true;
  }
  return true;
}

void Model::render(TGAImage &image) {
  std::vector<std::vector<double>> zbuffer(
      image.get_width(), std::vector<double>(image.get_height(), -1000));

  for (size_t i = 0; i < face.size(); ++i)
    triangle(image, i, zbuffer);
}

void Model::set_light(const Vector3d& light) {
	this->light = light;
	this->light.normalize();
}

void Model::set_up(const Vector3d& up) {
	this->up = up;
	this->up.normalize();
}

void Model::set_eye(const Vector3d& eye) {
	this->eye = eye;
}