#include "model.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "vector.h"

namespace {

double triangle_area(const Vector2i& a, const Vector2i& b, const Vector2i& c)
{
    return (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)) / (double)2;
}

Vector3d barycentric2d(const Vector2i& a, const Vector2i& b, const Vector2i& c, const Vector2i& p, const double area)
{
    double s1 = triangle_area(p, b, c);
    double s2 = triangle_area(a, p, c);
    return Vector3d(s1, s2, area - s1 - s2) / area;
}

}

void Model::triangle(TGAImage& image, int face_num, std::vector< std::vector<double> >& zbuffer, const Vector3d& light)
{
    Vector3d P[3];
    Vector2i p[3];
    Vector3d texture_p[3];
    for (int i=0; i < 3; ++i)
    {
        P[i] = point[face[face_num].point[i]];
        p[i] = {(int)((P[i][0] + 1) * image.get_width() / 2), (int)((P[i][1] + 1) * image.get_height() / 2)};
        // p[i] = world_to_screen(P[i], image);
        texture_p[i] = texture[face[face_num].texture[i]];
    }

    // if (!is_in_line(p[0], p[1], p[2]))
    // {
        Vector2i lb(image.get_width() - 1, image.get_height() - 1);
        Vector2i rt(0, 0);
        for (int i=0; i < 3; ++i)
        {
            lb.x = std::min(lb.x, p[i].x);
            lb.y = std::min(lb.y, p[i].y);
            rt.x = std::max(rt.x, p[i].x);
            rt.y = std::max(rt.y, p[i].y);
        }

        double area = triangle_area(p[0], p[1], p[2]);
        Vector2i pt;
        for (pt.x = lb.x; pt.x < rt.x; ++pt.x)
        {
            for (pt.y = lb.y; pt.y < rt.y; ++pt.y)
            {
                Vector3d bc = barycentric2d(p[0], p[1], p[2], pt, area);

                if (bc.x >= 0 && bc.y >= 0 && bc.z >= 0)
                {
                    double z = P[0].z * bc.x + P[1].z * bc.y + P[2].z * bc.z; // TODO: v * v
                    if (z > zbuffer[pt.x][pt.y])
                    {
                        zbuffer[pt.x][pt.y] = z;
                        double xd = Vector3d(texture_p[0].x, texture_p[1].x, texture_p[2].x) * bc;
                        double yd = Vector3d(texture_p[0].y, texture_p[1].y, texture_p[2].y) * bc;
                        TGAColor color = diffusemap.get(xd * diffusemap.get_width() + 0.5, diffusemap.get_height() * yd + 0.5);
                        TGAColor nc = normalsmap.get(xd * normalsmap.get_width() + 0.5, normalsmap.get_height() * yd + 0.5);
                        Vector3d n(nc.r, nc.g, nc.b);
                        n.normalize();
                        double intensivity = n * light;
                        color.r *= intensivity;
				        color.g *= intensivity;
				        color.b *= intensivity;
                        image.set(pt.x, pt.y, color);
                    }
                }
            }
        }

    // }
}

Model::Model(const std::string& path)
{
    load_from_file(path);
}

bool Model::load_from_file(const std::string& path)
{
    std::ifstream ifs(path);

    if (ifs.fail())
    {
        std::cerr << "Can't open " << path << '\n';
        return false;
    }

    char t;
    std::string line;
    while (!ifs.eof())
    {
        std::getline(ifs, line);
        std::istringstream iss(line.c_str());

        if (line.compare(0, 2, "v ") == 0)
        {
            Vector3d p;
            iss >> t >> p.x >> p.y >> p.z;

            point.push_back(p);
        }
        else if (line.compare(0, 3, "vt ") == 0)
        {
            Vector3d v;
            iss >> t >> t >> v.x >> v.y >> v.z;

            texture.push_back(v);
        }
        else if (line.compare(0, 3, "vn ") == 0)
        {
            Vector3d v;
            iss >> t >> t >> v.x >> v.y >> v.z;

            normal.push_back(v);
        }
        else if (line.compare(0, 2, "f ") == 0)
        {
            Face f;
            iss >> t;

            for (int i=0; i < 3; ++i)
            {
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

bool Model::load_texture(const std::string& file, const std::string type)
{
    TGAImage t;
    if (!t.read_tga_file(file.c_str()))
        return false;
    t.flip_vertically();
    if (type == "texture")
        diffusemap = t;
    if (type == "normals")
        normalsmap = t;
    return true;
}

void Model::render(TGAImage& image)
{
    // Vector3d P1, P2, P3, light(0, 0, 1), normal, d1, d2, d3;
    Vector3d light(0, 0, 1);
    // TGAColor color(0, 0, 0, 255);

    double max_depth = -1000;

    std::vector< std::vector<double> > zbuffer(image.get_width(), std::vector<double>(image.get_height(), max_depth));

    for (size_t i=0; i < face.size(); ++i)
        triangle(image, i, zbuffer, light);
}



////////////////////////////////////////////
// Matrix<4, 4, double> viewport(int x, int y, int w, int h) {
//     Matrix<4, 4, double> m({{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}});
//     int depth = 255;
//     m[0][3] = x+w/2.f;
//     m[1][3] = y+h/2.f;
//     m[2][3] = depth/2.f;

//     m[0][0] = w/2.f;
//     m[1][1] = h/2.f;
//     m[2][2] = depth/2.f;
//     return m;
// }

// Vector2i Model::world_to_screen(Vector3d& p, TGAImage& image)
// {
//     int width = image.get_width();
//     int height = image.get_height();
//     Matrix<4, 4, double> vp = viewport(width/8, height/8, width*3/4, height*3/4);
//     Matrix<4, 4, double> pr({{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, -1 / 2, 1}});
//     Matrix<4, 1, double> P({{p.x}, {p.y}, {p.z}, {1}});
//     P = vp * pr * P;
//     return Vector2i(P[0][0] / P[3][0], P[1][0] / P[3][0]);
// }
