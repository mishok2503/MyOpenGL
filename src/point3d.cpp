#include "point3d.h"
#include <cmath>

Point3d::Point3d() : x(0), y(0), z(0) {}

Point3d::Point3d(double x, double y, double z) : x(x), y(y), z(z) {}

double& Point3d::operator [](size_t i)
{
	return i == 0 ? x : (i == 1 ? y : z);
}

const double& Point3d::operator [](size_t i) const
{
	return i == 0 ? x : (i == 1 ? y : z);
}

bool operator <(const Point3d& p1, const Point3d& p2)
{
    return p1.x == p2.y ? (p1.y == p2.y ? p1.z < p2.z : p1.y < p2.y) : p1.x < p2.x;
}

bool operator >(const Point3d& p1, const Point3d& p2)
{
    return !(p1 <= p2);
}

bool operator ==(const Point3d& p1, const Point3d& p2)
{
    return (p1.x == p2.x && p1.y == p2.y && p1.z == p2.z);
}

bool operator <=(const Point3d& p1, const Point3d& p2)
{
    return (p1 < p2 || p1 == p2);
}

bool operator >=(const Point3d& p1, const Point3d& p2)
{
    return (p1 > p2 || p1 == p2);
}

const Point3d operator +(const Point3d& p1, const Point3d& p2)
{
    return Point3d(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
}

const Point3d operator -(const Point3d& p1, const Point3d& p2)
{
    return Point3d(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
}


const Point3d operator *(const Point3d& p1, const double k)
{
    return Point3d(p1.x * k, p1.y * k, p1.z * k);
}

const Point3d operator /(const Point3d& p1, const double k)
{
    return Point3d(p1.x / k, p1.y / k, p1.z / k);
}

double operator *(const Point3d& p1, const Point3d& p2)
{
    return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
}

Point3d Point3d::cross(const Point3d& p)
{
    return Point3d(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
}

void Point3d::normalize()
{
    double flip_len = 1 / sqrt(x * x + y * y + z * z);
    x *= flip_len;
    y *= flip_len;
    z *= flip_len;
}
