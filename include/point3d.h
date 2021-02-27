#ifndef __POINT3D_H__
#define __POINT3D_H__

struct Point3d
{
    double x;
    double y;
    double z;

    Point3d();
    Point3d(double x, double y, double z);

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


typedef Point3d Vector3d;


#endif // __POINT3D_H__
