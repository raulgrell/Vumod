
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


    assert(Mat4(1) + Mat4(1) == Mat4(2));
    assert(Mat4(2) - Mat4(1) == Mat4(1));

    assert(Mat4(1) * Mat4(1) == Mat4(1));
    assert(Mat4(1) * Mat4(2) == Mat4(2));
    assert(Mat4(1) * Vec4(1) == Vec4(1));
}

