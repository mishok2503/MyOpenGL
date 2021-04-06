#ifndef __POINT3D_H__
#define __POINT3D_H__

#include <cmath>

struct Point3d
{
    double x;
    double y;
    double z;

    Point3d();
    Point3d(double x, double y, double z);

    double& operator [](size_t i);
    const double& operator [](size_t i) const;

    friend bool operator <(const Point3d& p1, const Point3d& p2);
    friend bool operator >(const Point3d& p1, const Point3d& p2);
    friend bool operator <=(const Point3d& p1, const Point3d& p2);
    friend bool operator >=(const Point3d& p1, const Point3d& p2);
    friend bool operator ==(const Point3d& p1, const Point3d& p2);

    friend const Point3d operator +(const Point3d& p1, const Point3d& p2);
    friend const Point3d operator -(const Point3d& p1, const Point3d& p2);

    friend const Point3d operator *(const Point3d& p1, const double k);
    friend const Point3d operator /(const Point3d& p1, const double k);

    friend double operator *(const Point3d& p1, const Point3d& p2);

    Point3d cross(const Point3d& p);

    void normalize();
};


#endif // __POINT3D_H__
