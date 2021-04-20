#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "vector.h"
#include <vector>

template <size_t W, size_t H, typename T> struct Matrix;

namespace {

template <size_t dim, typename T> struct Determinant {
  static T det(Matrix<dim, dim, T> m) {
    T res = T();
    for (size_t i = dim; i--; res += m[i][0] * m.cofactor(i, 0))
      ;
    return res;
  }
};

template <typename T> struct Determinant<1, T> {
  static T det(Matrix<1, 1, T> m) { return m[0][0]; }
};

} // namespace

template <size_t H, size_t W, typename T> struct Matrix {
private:
  Vector<W, T> data[H];

public:
  Matrix() {
    for (size_t i = H; i--;)
      for (size_t j = W; j--; data[i][j] = T())
        ;
  }

  Matrix(const T& v) {
    for (size_t i = H; i--;)
      for (size_t j = W; j--; data[i][j] = i == j ? T(v) : T())
        ;
  }

  Matrix(const std::vector<std::vector<T>> &v) {
    assert(v.size() == H && v[0].size() == W);
    for (size_t i = H; i--;)
      for (size_t j = W; j--; data[i][j] = v[i][j])
        ;
  }

  Vector<W, T> &operator[](const size_t i) {
    assert(i < H);
    return data[i];
  }
  const Vector<W, T> &operator[](const size_t i) const {
    assert(i < H);
    return data[i];
  }

  Vector<H, T> get_column(const size_t i) const {
    assert(i < W);
    Vector<H, T> res;
    for (size_t j = H; j--; res[j] = data[j][i])
      ;
    return res;
  }

  void set_column(const size_t i, const Vector<H, T> &right) {
    assert(i < W);
    for (size_t j = H; j--; data[j][i] = right[j])
      ;
  }

  const Vector<2, size_t> size() const { return Vector<2, size_t>(W, H); }

  T det() const {
    static_assert(W == H, "The determinant need square matrix");
    static_assert(W != 0, "Determinant from empty matrix");
    return Determinant<W, T>::det(*this);
  }

  T cofactor(size_t y, size_t x) const {
    assert(y < H && x < W);
    return ((y + x) % 2 ? -1 : 1) * minor(y, x);
  }

  T minor(size_t y, size_t x) const {
    assert(y < H && x < W);
    Matrix<H - 1, W - 1, T> res;
    for (size_t i = H - 1; i--;)
      for (size_t j = W - 1; j--; res[i][j] = data[i + (i >= y)][j + (j >= x)])
        ;
    return res.det();
  }

  Matrix<H, W, T> adjugate() const {
    Matrix<H, W, T> res;
    for (size_t i = H; i--;)
      for (size_t j = W; j--; res[i][j] = cofactor(i, j))
        ;
    return res;
  }

  Matrix<W, H, T> transpose() const {
    Matrix<W, H, T> res;
    for (size_t i = W; i--; res[i] = get_column(i))
      ;
    return res;
  }

  Matrix<W, H, double> inverse() const {
    Matrix<H, W, T> t = adjugate();
    Matrix<H, W, double> res;
    for (size_t i = H; i--;)
      for (size_t j = W; j--; res[i][j] = static_cast<double>(t[i][j]))
        ;
    res = res / (data[0] * t[0]);
    return res.transpose();
  }
};

template <size_t W, size_t H, typename T>
Matrix<H, W, T> operator+(Matrix<H, W, T> left, const Matrix<H, W, T> &right) {
  for (size_t i = H; i--; left[i] = left[i] + right[i])
    ;
  return left;
}

template <size_t W, size_t H, typename T>
Matrix<H, W, T> operator-(Matrix<H, W, T> left, const Matrix<H, W, T> &right) {
  for (size_t i = H; i--; left[i] = left[i] - right[i])
    ;
  return left;
}

template <size_t H1, size_t W1, size_t W2, typename T>
Matrix<H1, W2, T> operator*(const Matrix<H1, W1, T> &left,
                            const Matrix<W1, W2, T> &right) {
  Matrix<H1, W2, T> res;
  for (size_t i = H1; i--;)
    for (size_t j = W2; j--; res[i][j] = left[i] * right.get_column(j))
      ;
  return res;
}

template <size_t W, size_t H, typename T>
Vector<H, T> operator*(const Matrix<H, W, T> &left, const Vector<W, T> &right) {
  Vector<H, T> res;
  for (size_t i = H; i--; res[i] = left[i] * right)
    ;
  return res;
}

template <size_t W, size_t H, typename T>
Matrix<H, W, double> operator*(Matrix<H, W, T> left, const double right) {
  for (size_t i = H; i--; left[i] = left[i] * right)
    ;
  return left;
}

template <size_t W, size_t H, typename T>
Matrix<H, W, double> operator/(Matrix<H, W, T> left, const double right) {
  for (size_t i = H; i--; left[i] = left[i] / right)
    ;
  return left;
}

#endif // __MATRIX_H__