#include "point2i.h"

Point2i::Point2i() : x(0), y(0) {}

Point2i::Point2i(int x, int y) : x(x), y(y) {}

bool operator <(const Point2i& p1, const Point2i& p2)
{
    return p1.x == p2.x ? p1.y < p2.y : p1.x < p2.x;
}

bool operator >(const Point2i& p1, const Point2i& p2)
{
    return !(p1 <= p2);
}

bool operator ==(const Point2i& p1, const Point2i& p2)
{
    return (p1.x == p2.x && p1.y == p2.y);
}

bool operator <=(const Point2i& p1, const Point2i& p2)
{
    return (p1 < p2 || p1 == p2);
}

bool operator >=(const Point2i& p1, const Point2i& p2)
{
    return (p1 > p2 || p1 == p2);
}

Point2i operator +(const Point2i& p1, const Point2i& p2)
{
    return Point2i(p1.x + p2.x, p1.y + p2.y);
}

Point2i operator -(const Point2i& p1, const Point2i& p2)
{
    return Point2i(p1.x - p2.x, p1.y - p2.y);
}

Point2i operator *(const Point2i& p1, const double k)
{
    return Point2i(p1.x * k, p1.y * k);
}

Point2i operator /(const Point2i& p1, const double k)
{
    return Point2i(p1.x / k, p1.y / k);
}
