#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <cassert>
#include <cmath>


template<size_t dim, typename T> class Vector
{
private:

    T data[dim];

public:

    Vector()
    {
        for (size_t i = dim; i--; data[i] = T());
    }

    T& operator [](size_t i)
    {
        assert(i < dim);
        return data[i];
    }
    const T& operator [](size_t i) const
    {
        assert(i < dim);
        return data[i];
    }

    const size_t size()
    {
        return dim;
    }
};


template<typename T> struct Vector<2, T>
{
    T x = T(), y = T();

    Vector() {}
    Vector(T x, T y) : x(x), y(y) {}

    T& operator [](size_t i)
    {
        assert(i < 2);
        return i == 0 ? x : y;
    }
    const T& operator [](size_t i) const
    {
        assert(i < 2);
        return i == 0 ? x : y;
    }

    size_t size()
    {
        return 2;
    }
};


template<typename T> struct Vector<3, T>
{
public:

    T x = T(), y = T(), z = T();

    Vector() {}
    Vector(T x, T y, T z) : x(x), y(y), z(z) {}

    T& operator [](const size_t i) { assert(i < 3); return (i == 0 ? x : (i == 1 ? y : z)); }
    const T& operator [](const size_t i) const { assert(i < 3); return (i == 0 ? x : (i == 1 ? y : z)); }

    size_t size()
    {
        return 3;
    }

    double length()
    {
        return sqrt(x * x + y * y + z * z);
    }

    Vector<3, T>& normalize(double l = 1)
    {
        return *this = (*this) * (l / length());
    }
};



template<size_t dim, typename T> Vector<dim, T> operator +(Vector<dim, T> left, const Vector<dim, T>& right)
{
    for (size_t i = dim; i--; left[i] += right[i]);
    return left;
}

template<size_t dim, typename T> Vector<dim, T> operator -(Vector<dim, T> left, const Vector<dim, T>& right)
{
    for (size_t i = dim; i--; left[i] -= right[i]);
    return left;
}

template<size_t dim, typename T> T operator *(const Vector<dim, T>& left, const Vector<dim, T>& right)
{
    T res = T();
    for (size_t i = dim; i--; res += left[i] * right[i]);
    return res;
}

template<size_t dim, typename T> Vector<dim, double> operator *(const Vector<dim, T>& left, const double right)
{
    Vector<dim, double> res;
    for (size_t i = dim; i--; res[i] = left[i] * right);
    return res;
}

template<size_t dim, typename T> Vector<dim, double> operator /(const Vector<dim, T>& left, const double right)
{
    Vector<dim, double> res;
    for (size_t i = dim; i--; res[i] = left[i] / right);
    return res;
}



typedef Vector<3, double> Vector3d;
typedef Vector<3, int> Vector3i;
typedef Vector<2, double> Vector2d;
typedef Vector<2, int> Vector2i;

#endif // __VECTOR_H__
