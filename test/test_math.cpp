
#include <math/Math.h>

#include <cassert>

int main()
{
    assert(Vec2(1, -1) + Vec2(1, -1) == Vec2(2, -2));
    assert(Vec2(1, -1) - Vec2(1, -1) == Vec2(0, 0));
    assert(Vec2(1, 2) * Vec2(1, 2) == Vec2(1, 4));
    assert(Vec2(1, 2) / Vec2(1, 2) == Vec2(1, 1));
    assert(Vec2() * 2 == Vec2());
    assert(Vec2() / 2 == Vec2());

    assert(Vec3(1, 1, 1) + Vec3(1, 1, 1) == Vec3(2, 2, 2));
    assert(Vec3(1, 1, 1) - Vec3(1, 1, 1) == Vec3(0, 0, 0));
    assert(Vec3(1, 1, 1) * Vec3(2, 2, 2) == Vec3(2, 2, 2));
    assert(Vec3(2, 2, 2) / Vec3(2, 2, 2) == Vec3(1, 1, 1));
    assert(Vec3::Dot(Vec3(1, 3, -5), Vec3(4, -2, -1)) == 3);
    assert(Vec3::Cross(Vec3(3, -3, 1), Vec3(4, 9, 2)) == Vec3(-15, -2, 39));
    assert(Vec3(1) * 2 == Vec3(2));
    assert(Vec3(2) / 2 == Vec3(1));

    assert(Vec4(1) + Vec4(1) == Vec4(2));
    assert(Vec4(2) - Vec4(1) == Vec4(1));
    assert(Vec4(1) * Vec4(1) == Vec4(1));
    assert(Vec4(2) / Vec4(1) == Vec4(2));
    assert(Vec4(2) * 2 == Vec4(4));
    assert(Vec4(2) / 2 == Vec4(1));

    Mat4 m(1);

    Mat4 a({1, 2, 3, 4},
           {5, 6, 7, 8},
           {9, 8, 7, 6},
           {5, 4, 3, 2});

    Mat4 b({1, 3, 5, 7},
           {2, 4, 6, 8},
           {9, 9, 9, 1},
           {1, 1, 1, 1});

    Mat4 c({36, 42, 48, 30},
           {88, 110, 132, 98},
           {94, 128, 162, 140},
           {42, 60, 78, 72});

    Mat4 d({2, 5, 8, 11},
           {7, 10, 13, 16},
           {18, 17, 16, 7},
           {6, 5, 4, 3});

    assert(a * b == c);
    assert(a + b == d);

    a *= b;
    assert(a == c);

    assert(Mat4(1) * Mat4(1) == Mat4(1));
    assert(Mat4(1) * Mat4(1) == Mat4(1));
    assert(Mat4(1) * Mat4(2) == Mat4(2));
    assert(Mat4(1) * Vec4(1) == Vec4(1));
}

