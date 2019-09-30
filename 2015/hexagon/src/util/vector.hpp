/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#pragma once

#include <algorithm>
#include <cstdint>
#include <vector>

template<typename T, int N>
class Vector
{
public:
    Vector()
    {
        std::fill(std::begin(v), std::end(v), (T)0);
    }

    explicit Vector(const T& v0)
    {
        std::fill(std::begin(v), std::end(v), v0);
    }

    Vector(const T& v0, const T& v1)
    {
        v[0] = v0;
        v[1] = v1;
    }

    Vector(const T& v0, const T& v1, const T& v2)
    {
        v[0] = v0;
        v[1] = v1;
        v[2] = v2;
    }

    Vector(const T& v0, const T& v1, const T& v2, const T& v3)
    {
        v[0] = v0;
        v[1] = v1;
        v[2] = v2;
        v[3] = v3;
    }
    
    const T& operator[](int i) const
    {
        return v[i];
    }

    T& operator[](int i)
    {
        return v[i];
    }

    const T& operator()(int i) const
    {
        return v[i];
    }

    T& operator()(int i)
    {
        return v[i];
    }

    Vector operator*(T a) const
    {
        Vector r;
        for(int i=0; i<N; ++i)
            r[i] = v[i] * a;
        return r;
    }

    Vector operator+(T a) const
    {
        Vector result;
        for(int i=0; i<N; ++i)
            result[i] = v[i] + a;
        return result;
    }

    Vector operator-(T a) const
    {
        Vector result;
        for(int i=0; i<N; ++i)
            result[i] = v[i] - a;
        return result;
    }

    Vector operator*(const Vector& x) const
    {
        Vector result;
        for(int i=0; i<N; ++i)
            result[i] = v[i] * x[i];
        return result;
    }

    Vector operator/(const Vector& x) const
    {
        Vector result;
        for(int i=0; i<N; ++i)
            result[i] = v[i] / x[i];
        return result;
    }

    Vector operator+(const Vector& x) const
    {
        Vector result;
        for(int i=0; i<N; ++i)
            result[i] = v[i] + x[i];
        return result;
    }

    Vector operator-(const Vector& x) const
    {
        Vector result;
        for(int i=0; i<N; ++i)
            result[i] = v[i] - x[i];
        return result;
    }

    Vector& operator+=(const Vector& x)
    {
        *this = *this + x;
        return *this;
    }

    Vector& operator-=(const Vector& x)
    {
        *this = *this - x;
        return *this;
    }

    Vector& operator*=(const Vector& x)
    {
        *this = *this * x;
        return *this;
    }

    Vector& operator/=(const Vector& x)
    {
        *this = *this / x;
        return *this;
    }

    Vector cross(const Vector& x);
    
    Vector operator-() const
    {
        Vector result;
        for(int i=0; i<N; ++i)
            result[i] = -v[i];
        return result;
    }

    T dot(const Vector& x) const
    {
        T result = (T)0;
        for(int i=0; i<N; ++i)
            result += v[i] * x[i];
        return result;
    }

    Vector abs() const
    {
        Vector result;
        for(int i=0; i<N; ++i)
            result[i] = ::abs(v[i]);
        return result;
    }

    Vector floor() const
    {
        Vector result;
        for(int i=0; i<N; ++i)
            result[i] = ::floor(v[i]);
        return result;
    }

    Vector ceil() const
    {
        Vector result;
        for(int i=0; i<N; ++i)
            result[i] = ::ceil(v[i]);
        return result;
    }

    float sqrlength() const
    {
        T sum = 0.f;
        for(int i=0; i<N; ++i)
            sum += v[i]*v[i];
        return sum;
    }

    float length() const
    {
        return sqrt(sqrlength());
    }

    void normalize()
    {
        float linv = 1./length();
        for(int i=0; i<N; ++i)
            v[i] = v[i] * linv;
    }


private:
    T v[N];
};

typedef Vector<int8_t, 2> Vec2b;
typedef Vector<int8_t, 3> Vec3b;
typedef Vector<int8_t, 4> Vec4b;

typedef Vector<uint8_t, 2> Vec2ub;
typedef Vector<uint8_t, 3> Vec3ub;
typedef Vector<uint8_t, 4> Vec4ub;

typedef Vector<int, 2> Vec2i;
typedef Vector<int, 3> Vec3i;
typedef Vector<int, 4> Vec4i;

typedef Vector<float, 2> Vec2f;
typedef Vector<float, 3> Vec3f;
typedef Vector<float, 4> Vec4f;

typedef Vector<double, 2> Vec2d;
typedef Vector<double, 3> Vec3d;
typedef Vector<double, 4> Vec4d;

template<>
inline Vector<float, 3> Vector<float, 3>::cross(const Vector<float, 3>& x)
{
    return Vector<float, 3>(v[1]*x[2] - v[2]*x[1],
                            v[2]*x[0] - v[0]*x[2],
                            v[0]*x[1] - v[1]*x[0]);
}

template<typename T, int N>
Vector<T, N> operator*(T a, const Vector<T, N>& vec)
{
    return vec * a;
}

template<typename T, int N>
Vector<T, N> operator+(T a, const Vector<T, N>& vec)
{
    return vec + a;
}


template<typename T, int N>
Vector<T, N> operator-(T a, const Vector<T, N>& vec)
{
    return -vec;// + a;
}

template<typename T, int N>
std::ostream& operator<<(std::ostream& out, const Vector<T, N>& vec)
{
    out << "[";
    for(int i=0; i<N-1; ++i)
    {
        out << vec[i] << " ";
    }
    out << vec[N-1] << "]";
    return out;
}
