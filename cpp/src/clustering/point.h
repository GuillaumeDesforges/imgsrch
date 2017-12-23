#pragma once

#include <vector>
#include <algorithm>
#include <cassert>
#include <string>
using namespace std;

template<typename T>
class Point {
public:
    Point(int dimension);
    Point(const Point<T> &other);

    void operator=(const Point<T> &other);

    T &operator[](int i);
    const T operator[](int i) const;

    const Point<T> operator+(const Point<T> &other) const;
    const Point<T> operator-(const Point<T> &other) const;
    const Point<T> operator*(T value) const;
    const Point<T> operator/(T value) const;

    void operator+=(const Point<T> &other);
    void operator-=(const Point<T> &other);
    void operator/=(T value);
    void operator*=(T value);

    const T norm2() const;

    const int getDimension() const;
private:
    vector<T> x;
    int dimension;
};

template<typename T>
Point<T>::Point(int dimension) {
    this->dimension = dimension;
    vector<T> v(dimension);
    this->x = v;
}

template<typename T>
Point<T>::Point(const Point<T> &other) {
    this->x = other.x;
    this->dimension = other.dimension;
}

template<typename T>
void Point<T>::operator=(const Point<T> &other) {
    this->x = other.x;
    this->dimension = other.dimension;
}

template<typename T>
T &Point<T>::operator[](int i) {
    return x[i];
}

template<typename T>
const T Point<T>::operator[](int i) const {
    return x[i];
}

template<typename T>
const Point<T> Point<T>::operator+(const Point<T> &other) const {
    assert(dimension == other.dimension);
    Point<T> result(dimension);
    for(int i = 0; i < dimension; i++) {
        result[i] = x[i] + other[i];
    }
    return result;
}

template<typename T>
const Point<T> Point<T>::operator-(const Point<T> &other) const {
    assert(dimension == other.dimension);
    Point<T> result(dimension);
    for(int i = 0; i < dimension; i++) {
        result[i] = x[i] - other[i];
    }
    return result;
}

template<typename T>
const Point<T> Point<T>::operator*(T value) const {
    Point<T> result(dimension);
    for(int i = 0; i < dimension; i++) {
        result[i] = x[i] * value;
    }
    return result;
}

template<typename T>
const Point<T> Point<T>::operator/(T value) const {
    Point<T> result(dimension);
    for(int i = 0; i < dimension; i++) {
        result[i] = x[i] / value;
    }
    return result;
}


template<typename T>
void Point<T>::operator+=(const Point<T> &other) {
    assert(dimension == other.dimension);
    for(int i = 0; i < dimension; i++) {
        x[i] += other.x[i];
    }
}

template<typename T>
void Point<T>::operator-=(const Point<T> &other) {
    assert(dimension == other.dimension);
    for(int i = 0; i < dimension; i++) {
        x[i] -= other.x[i];
    }
}

template<typename T>
void Point<T>::operator/=(T value) {
    for(int i = 0; i < dimension; i++) {
        x[i] /= value;
    }
}

template<typename T>
void Point<T>::operator*=(T value) {
    for(int i = 0; i < dimension; i++) {
        x[i] *= value;
    }
}

template<typename T>
const T Point<T>::norm2() const {
    T result = 0.0;
    for(int i = 0; i < dimension; i++) {
        result += x[i]*x[i];
    }
    return result;
}

template<typename T>
const int Point<T>::getDimension() const {
    return dimension;
}

template<typename T>
std::ostream& operator<<(ostream& stream, const Point<T>& point) {
    if(point.getDimension() == 0) {
        stream << string("(EMPTY)");
        return stream;
    }
    stream << string("(");
    for(int d = 0; d < point.getDimension(); d++) {
        stream << point[d];
        if(d+1 < point.getDimension()) {
            stream << string(", ");
        }
    }
    stream << string(")");
    return stream;
}
