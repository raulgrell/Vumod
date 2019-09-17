#pragma once

#include "MathUtils.h"
#include "Vec.h"
#include "Quat.h"

#include <algorithm>

struct Mat4
{
    union
    {
        float m[4 * 4] {0};
        Vec4 r[4];
    };

    constexpr Mat4() noexcept : m {0}
    {
    }

    explicit
    constexpr Mat4(float diagonal) noexcept : m {0}
    {
        r[0].c[0] = diagonal;
        r[1].c[0] = diagonal;
        r[2].c[0] = diagonal;
        r[3].c[0] = diagonal;
    }

    constexpr Mat4(const Vec4 &a, const Vec4 &b, const Vec4 &c, const Vec4 &d) noexcept
            : r {a, b, c, d}
    {
    }

    float *Get()
    {
        return &m[0];
    }

    Vec4 operator[](int index)
    {
        return r[index];
    }

    Mat4 operator*(const Mat4 &other)
    {
        Mat4 r;
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                float sum = 0;
                for (int e = 0; e < 4; e++) {
                    sum += m[e + row * 4] * other.m[col + e * 4];
                }
                r.m[col + row * 4] = sum;
            }
        }
        return r;
    };

    void operator*=(const Mat4 &other)
    {
        Mat4 r = *this;
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                float sum = 0;
                for (int e = 0; e < 4; e++) {
                    sum += r.m[e + row * 4] * other.m[col + e * 4];
                }
                m[col + row * 4] = sum;
            }
        }
    };

    Vec4 operator*(const Vec4 &other)
    {
        Vec4 v;
        v.x = r[0].x * other.x + r[0].y * other.y + r[0].z * other.z + r[0].w * other.w;
        v.y = r[1].x * other.x + r[1].y * other.y + r[1].z * other.z + r[1].w * other.w;
        v.z = r[2].x * other.x + r[2].y * other.y + r[2].z * other.z + r[2].w * other.w;
        v.w = r[3].x * other.x + r[3].y * other.y + r[3].z * other.z + r[3].w * other.w;
        return v;
    };

    bool operator==(const Mat4 &other)
    {
        for (int i = 0; i < 16; ++i) {
            if (m[i] != other.m[i])
                return false;
        }
        return true;
    }

    Mat4 operator+(const Mat4 &other)
    {
        return {{r[0].x + other.r[0].x, r[0].y + other.r[0].y, r[0].z + other.r[0].z, r[0].w + other.r[0].w},
                {r[1].x + other.r[1].x, r[1].y + other.r[1].y, r[1].z + other.r[1].z, r[1].w + other.r[1].w},
                {r[2].x + other.r[2].x, r[2].y + other.r[2].y, r[2].z + other.r[2].z, r[2].w + other.r[2].w},
                {r[3].x + other.r[3].x, r[3].y + other.r[3].y, r[3].z + other.r[3].z, r[3].w + other.r[3].w}};
    }

    Mat4 operator-(const Mat4 &other)
    {
        return {{r[0].x - other.r[0].x, r[0].y - other.r[0].y, r[0].z - other.r[0].z, r[0].w - other.r[0].w},
                {r[1].x - other.r[1].x, r[1].y - other.r[1].y, r[1].z - other.r[1].z, r[1].w - other.r[1].w},
                {r[2].x - other.r[2].x, r[2].y - other.r[2].y, r[2].z - other.r[2].z, r[2].w - other.r[2].w},
                {r[3].x - other.r[3].x, r[3].y - other.r[3].y, r[3].z - other.r[3].z, r[3].w - other.r[3].w}};
    }

    static Mat4 LookAt(const Vec3 &camera, const Vec3 &object, const Vec3 &up)
    {
        Mat4 result(1.0f);
        Vec3 f = Vec3::Normal(object - camera);
        Vec3 s = Vec3::Cross(f, Vec3::Normal(up));
        Vec3 u = Vec3::Cross(s, f);

        result.r[0].c[0] = s.x;
        result.r[1].c[0] = s.y;
        result.r[2].c[0] = s.z;
        result.r[0].c[1] = u.x;
        result.r[1].c[1] = u.y;
        result.r[2].c[1] = u.z;
        result.r[0].c[2] = -f.x;
        result.r[1].c[2] = -f.y;
        result.r[2].c[2] = -f.z;

        result *= Translate({-camera.x, -camera.y, -camera.z});

        return result;
    }

    static Mat4 Translate(const Vec3 &translation)
    {
        Mat4 result(1.0f);
        result.r[0].c[3] = translation.x;
        result.r[1].c[3] = translation.y;
        result.r[2].c[3] = translation.z;
        return result;
    }

    static Mat4 Rotate(float angle, const Vec3 &axis)
    {
        Mat4 result(1.0f);

        float r = math::radians(angle);
        float c = cosf(r);
        float s = sinf(r);
        float omc = 1.0f - c;

        float x = axis.x;
        float y = axis.y;
        float z = axis.z;

        result.r[0].c[0] = x * x * omc + c;
        result.r[1].c[0] = y * x * omc + z * s;
        result.r[2].c[0] = x * z * omc - y * s;

        result.r[0].c[1] = x * y * omc - z * s;
        result.r[1].c[1] = y * y * omc + c;
        result.r[2].c[1] = y * z * omc + x * s;

        result.r[0].c[2] = x * z * omc + y * s;
        result.r[1].c[2] = y * z * omc - x * s;
        result.r[2].c[2] = z * z * omc + c;

        return result;
    }

    static Mat4 Rotate(const Quat &quat)
    {
        Mat4 result(1.0f);

        float qx = quat.x;
        float qy = quat.y;
        float qz = quat.z;
        float qw = quat.w;
        float qx2 = (qx + qx);
        float qy2 = (qy + qy);
        float qz2 = (qz + qz);
        float qxqx2 = (qx * qx2);
        float qxqy2 = (qx * qy2);
        float qxqz2 = (qx * qz2);
        float qxqw2 = (qw * qx2);
        float qyqy2 = (qy * qy2);
        float qyqz2 = (qy * qz2);
        float qyqw2 = (qw * qy2);
        float qzqz2 = (qz * qz2);
        float qzqw2 = (qw * qz2);

        result.r[0] = Vec4(((1.0f - qyqy2) - qzqz2), (qxqy2 - qzqw2), (qxqz2 + qyqw2), 0.0f);
        result.r[1] = Vec4((qxqy2 + qzqw2), ((1.0f - qxqx2) - qzqz2), (qyqz2 - qxqw2), 0.0f);
        result.r[2] = Vec4((qxqz2 - qyqw2), (qyqz2 + qxqw2), ((1.0f - qxqx2) - qyqy2), 0.0f);
        return result;
    }

    static Mat4 Scale(const Vec3 &scale)
    {
        Mat4 result(1.0f);
        result.m[0 + 0 * 4] = scale.x;
        result.m[1 + 1 * 4] = scale.y;
        result.m[2 + 2 * 4] = scale.z;
        return result;
    }

    Mat4 Invert()
    {
        Mat4 t;

        t.m[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] +
                 m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];

        t.m[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] -
                 m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];

        t.m[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] +
                 m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];

        t.m[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11]
                 - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];

        t.m[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] -
                 m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];

        t.m[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] +
                 m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];

        t.m[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] -
                 m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];

        t.m[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] +
                 m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];

        t.m[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] +
                 m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];

        t.m[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] -
                 m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];

        t.m[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] +
                  m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];

        t.m[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] -
                  m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];

        t.m[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] -
                  m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];

        t.m[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] +
                  m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];

        t.m[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14]
                  - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];

        t.m[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] +
                  m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

        float determinant = m[0] * t.m[0] + m[1] * t.m[4] + m[2] * t.m[8] + m[3] * t.m[12];
        determinant = 1.0f / determinant;

        for (int i = 0; i < 16; i++)
            m[i] = t.m[i] * determinant;

        return *this;
    }

    Mat4 Transpose(const Mat4 &matrix)
    {
        return {{matrix.r[0].x, matrix.r[1].x, matrix.r[2].x, matrix.r[3].x},
                {matrix.r[0].y, matrix.r[1].y, matrix.r[2].y, matrix.r[3].y},
                {matrix.r[0].z, matrix.r[1].z, matrix.r[2].z, matrix.r[3].z},
                {matrix.r[0].w, matrix.r[1].w, matrix.r[2].w, matrix.r[3].w}};
    }

    static Mat4 Inverse(const Mat4 &m)
    {
        Mat4 result = m;
        result.Invert();
        return result;
    }

    static Mat4 Orthographic(float left, float right, float bottom, float top, float near_plane, float far_plane)
    {
        Mat4 result(1);
        result.r[0].c[0] = 2.0f / (right - left);
        result.r[1].c[1] = 2.0f / (top - bottom);
        result.r[2].c[2] = 2.0f / (near_plane - far_plane);
        result.r[0].c[3] = (left + right) / (left - right);
        result.r[1].c[3] = (bottom + top) / (bottom - top);
        result.r[2].c[3] = (far_plane + near_plane) / (far_plane - near_plane);
        return result;
    }

    static Mat4 Perspective(float fov, float aspectRatio, float near_plane, float far_plane)
    {
        Mat4 result(1);

        float q = 1.0f / tan(math::radians(0.5f * fov));
        float a = q / aspectRatio;

        float b = (near_plane + far_plane) / (near_plane - far_plane);
        float c = (2.0f * near_plane * far_plane) / (near_plane - far_plane);

        result.r[0].c[0] = a;
        result.r[1].c[1] = q;
        result.r[2].c[2] = b;
        result.r[3].c[2] = -1.0f;
        result.r[2].c[3] = c;
        result.r[3].c[3] = 0;

        return result;
    }

    static Mat4 Identity()
    {
        Mat4 result(1);
        return result;
    }

    static Mat4 TRS(const Vec3 &translation, const Vec3 &rotation, const Vec3 &scale)
    {
        Mat4 result;

        Mat4 t = Mat4::Translate(Vec3(translation.x, translation.y, translation.z));

        Mat4 rotx = Mat4::Rotate(math::radians(rotation.x), Vec3(1.0f, 0.0f, 0.0f));
        Mat4 roty = Mat4::Rotate(math::radians(rotation.y), Vec3(0.0f, 1.0f, 0.0f));
        Mat4 rotz = Mat4::Rotate(math::radians(rotation.z), Vec3(0.0f, 0.0f, 1.0f));
        Mat4 r = rotx * roty * rotz;

        Mat4 s = Mat4::Scale(Vec3(scale));

        result = t * r * s;
        return result;
    }
};
