#pragma once

#include <cstddef>
#include <memory>
#include <stdexcept>


template <typename T> struct Matrix {
  Matrix(size_t rows, size_t cols)
      : data(new T[rows * cols]), rows(rows), cols(cols) {}
  std::unique_ptr<T[]> data;
  size_t rows;
  size_t cols;
  T &operator()(size_t i, size_t j) { return data.get()[i * cols + j]; }
  const T &operator()(size_t i, size_t j) const {
    return data.get()[i * cols + j];
  }
  T &at(size_t i, size_t j) {
    if (i >= rows || j >= cols)
      throw std::out_of_range("Matrix index out of range");
    return data.get()[i * cols + j];
  }
  const T &at(size_t i, size_t j) const {
    if (i >= rows || j >= cols)
      throw std::out_of_range("Matrix index out of range");
    return data.get()[i * cols + j];
  }
};

template <typename T>
Matrix<T> allocate_multiply(const Matrix<T> &a, const Matrix<T> &b) {
  return Matrix<T>(a.rows, b.cols);
}

template <typename T>
void multiply_ikj(const Matrix<T> &a, const Matrix<T> &b, Matrix<T> &c) {
  for (size_t i = 0; i < a.rows; i++) {
    for (size_t k = 0; k < a.cols; k++) {
      for (size_t j = 0; j < b.cols; j++) {
        c(i, j) += a(i, k) * b(k, j);
      }
    }
  }
}

template <typename T>
void multiply_ijk(const Matrix<T> &a, const Matrix<T> &b, Matrix<T> &c) {
  for (size_t i = 0; i < a.rows; i++) {
    for (size_t j = 0; j < b.cols; j++) {
      for (size_t k = 0; k < a.cols; k++) {
        c(i, j) += a(i, k) * b(k, j);
      }
    }
  }
}

template <typename T>
void multiply_kij(const Matrix<T> &a, const Matrix<T> &b, Matrix<T> &c) {
  for (size_t k = 0; k < a.cols; k++) {
    for (size_t i = 0; i < a.rows; i++) {
      for (size_t j = 0; j < b.cols; j++) {
        c(i, j) += a(i, k) * b(k, j);
      }
    }
  }
}

template <typename T>
void multiply_kji(const Matrix<T> &a, const Matrix<T> &b, Matrix<T> &c) {
  for (size_t k = 0; k < a.cols; k++) {
    for (size_t j = 0; j < b.cols; j++) {
      for (size_t i = 0; i < a.rows; i++) {
        c(i, j) += a(i, k) * b(k, j);
      }
    }
  }
}

template <typename T>
void multiply_jki(const Matrix<T> &a, const Matrix<T> &b, Matrix<T> &c) {
  for (size_t j = 0; j < b.cols; j++) {
    for (size_t k = 0; k < a.cols; k++) {
      for (size_t i = 0; i < a.rows; i++) {
        c(i, j) += a(i, k) * b(k, j);
      }
    }
  }
}

template <typename T>
void multiply_jik(const Matrix<T> &a, const Matrix<T> &b, Matrix<T> &c) {
  for (size_t j = 0; j < b.cols; j++) {
    for (size_t i = 0; i < a.rows; i++) {
      for (size_t k = 0; k < a.cols; k++) {
        c(i, j) += a(i, k) * b(k, j);
      }
    }
  }
}

template <typename T>
void multiply_ikj_safe(const Matrix<T> &a, const Matrix<T> &b, Matrix<T> &c) {
  for (size_t i = 0; i < a.rows; i++) {
    for (size_t k = 0; k < a.cols; k++) {
      for (size_t j = 0; j < b.cols; j++) {
        c.at(i, j) += a.at(i, k) * b.at(k, j);
      }
    }
  }
}

template <typename T>
void multiply_ijk_safe(const Matrix<T> &a, const Matrix<T> &b, Matrix<T> &c) {
  for (size_t i = 0; i < a.rows; i++) {
    for (size_t j = 0; j < b.cols; j++) {
      for (size_t k = 0; k < a.cols; k++) {
        c.at(i, j) += a.at(i, k) * b.at(k, j);
      }
    }
  }
}

template <typename T>
void multiply_kij_safe(const Matrix<T> &a, const Matrix<T> &b, Matrix<T> &c) {
  for (size_t k = 0; k < a.cols; k++) {
    for (size_t i = 0; i < a.rows; i++) {
      for (size_t j = 0; j < b.cols; j++) {
        c.at(i, j) += a.at(i, k) * b.at(k, j);
      }
    }
  }
}

template <typename T>
void multiply_kji_safe(const Matrix<T> &a, const Matrix<T> &b, Matrix<T> &c) {
  for (size_t k = 0; k < a.cols; k++) {
    for (size_t j = 0; j < b.cols; j++) {
      for (size_t i = 0; i < a.rows; i++) {
        c.at(i, j) += a.at(i, k) * b.at(k, j);
      }
    }
  }
}

template <typename T>
void multiply_jki_safe(const Matrix<T> &a, const Matrix<T> &b, Matrix<T> &c) {
  for (size_t j = 0; j < b.cols; j++) {
    for (size_t k = 0; k < a.cols; k++) {
      for (size_t i = 0; i < a.rows; i++) {
        c.at(i, j) += a.at(i, k) * b.at(k, j);
      }
    }
  }
}

template <typename T>
void multiply_jik_safe(const Matrix<T> &a, const Matrix<T> &b, Matrix<T> &c) {
  for (size_t j = 0; j < b.cols; j++) {
    for (size_t i = 0; i < a.rows; i++) {
      for (size_t k = 0; k < a.cols; k++) {
        c.at(i, j) += a.at(i, k) * b.at(k, j);
      }
    }
  }
}
