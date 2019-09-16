#pragma once

#include <cmath>

struct Vec2
{
    union
    {
        struct
        {
            float x, y;
        };
        float c[2]{0};
    };

    constexpr Vec2() noexcept : x(0), y(0) {}
    constexpr Vec2(float value) noexcept : x(value), y(value) {}
    constexpr Vec2(float x, float y) noexcept : x(x), y(y) {}

    float& operator[](int index) {
        return c[index];
    }

    Vec2 operator-(Vec2 vec) const
    {
        return {x - vec.x, y - vec.y};
    }

    Vec2 operator+(Vec2 vec) const
    {
        return {x + vec.x, y + vec.y};
    }

    Vec2 operator*(float value) const
    {
        return {x * value, y * value};
    }

    Vec2 operator/(float value) const
    {
        return {x / value, y / value};
    }
};

struct Vec3
{
    union
    {
        struct
        {
            float x, y, z;
        };
        float c[3] {0};
    };

    constexpr Vec3() noexcept : x(0), y(0), z(0) {}
    constexpr Vec3(float value) noexcept : x(value), y(value), z(value) {}
    constexpr Vec3(float x, float y, float z) noexcept : x(x), y(y), z(z) {}

    float& operator[](int index) {
        return c[index];
    }

    void operator*=(float value)
    {
        x *= value;
        y *= value;
        z *= value;
    }

    void operator+=(float value)
    {
        x += value;
        y += value;
        z += value;
    }

    void operator+=(Vec3 vec)
    {
        x += vec.x;
        y += vec.y;
        z += vec.z;
    }

    Vec3 operator-() const
    {
        return {-x, -y, -z};
    }

    Vec3 operator-(const Vec3 vec) const
    {
        return {x - vec.x, y - vec.y, z - vec.z};
    }

    Vec3 operator+(const Vec3 vec) const
    {
        return {x + vec.x, y + vec.y, z + vec.z};
    }

    Vec3 operator/(float value) const
    {
        return {x / value, y / value, z / value};
    }

    Vec3 operator*(float value) const
    {
        return {x * value, y * value, z * value};
    }

    Vec3 Normalize()
    {
        float length = Length();
        x /= length;
        y /= length;
        z /= length;
        return *this;
    }

    float Length() const
    {
        return sqrtf(x * x + y * y + z * z);
    }

    static Vec3 Normal(Vec3 v)
    {
        Vec3 vec = v;
        vec.Normalize();
        return vec;
    }

    static Vec3 Cross(Vec3 v, Vec3 k)
    {
        return {v.y * k.z - v.z * k.y,
                v.z * k.x - v.x * k.z,
                v.x * k.y - v.y * k.x};
    }

    static float Dot(Vec3 v, Vec3 k)
    {
        return v.x * k.x + v.y * k.y + v.z * k.z;
    }

    static float Length(Vec3 v)
    {
        return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    }
};

struct Vec4
{
    union
    {
        struct
        {
            float x, y, z, w;
        };
        float c[4] {0};
    };

    constexpr Vec4() noexcept : x(0), y(0), z(0), w(0) {}
    constexpr Vec4(float value) noexcept : x(value), y(value), z(value), w(value) {};
    constexpr Vec4(float x, float y, float z, float w) noexcept : x(x), y(y), z(z), w(w) {}

    float& operator[](int index) {
        return c[index];
    }
};

