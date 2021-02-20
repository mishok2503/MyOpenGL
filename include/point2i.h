#ifndef __POINT2I_H__
#define __POINT2I_H__

struct Point2i
{
    int x;
    int y;

    Point2i();
    Point2i(int x, int y);

    friend bool operator <(const Point2i& p1, const Point2i& p2);
    friend bool operator >(const Point2i& p1, const Point2i& p2);
    friend bool operator <=(const Point2i& p1, const Point2i& p2);
    friend bool operator >=(const Point2i& p1, const Point2i& p2);
    friend bool operator ==(const Point2i& p1, const Point2i& p2);

    friend Point2i operator +(const Point2i& p1, const Point2i& p2);
    friend Point2i operator -(const Point2i& p1, const Point2i& p2);
    friend Point2i operator *(const Point2i& p1, const double k);
    friend Point2i operator /(const Point2i& p1, const double k);
};

typedef Point2i Vector2i;

#endif //__POINT2I_H__
