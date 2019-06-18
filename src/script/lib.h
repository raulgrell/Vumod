struct Vec3f {
    union {
        float v[3];
        struct { float x, y, z; };
    };

    Vec3f( float x, float y, float z )
            : v{ x, y, z } {}

    Vec3f()
            : v{ 0.f, 0.f, 0.f } {}

    float norm() const {
        return sqrt( x*x + y*y + z*z );
    }

    float dot( const Vec3f& rhs ) const {
        return x*rhs.x + y*rhs.y + z*rhs.z;
    }

    Vec3f cross( const Vec3f& rhs ) const {
        return Vec3f {
                y*rhs.z - z*rhs.y,
                z*rhs.x - x*rhs.z,
                x*rhs.y - y*rhs.x
        };
    }
};

void vec3fAllocate(WrenVM* vm) {
    void* bytes = wrenSetSlotNewForeign(vm, 0, 0, sizeof(Vec3f));
    new (bytes) Vec3f(wrenGetSlotDouble(vm, 1), wrenGetSlotDouble(vm, 2), wrenGetSlotDouble(vm, 3));
}

void vec3fFinalize(void* bytes) {}

void vec3fNorm(WrenVM* vm) {
    // we can access the byte array of the object in slot zero
    const auto* v = (const Vec3f*)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, v->norm());
}

void vec3fDot(WrenVM* vm) {
    const auto* lhs = (const Vec3f*)wrenGetSlotForeign(vm, 0);
    const auto* rhs = (const Vec3f*)wrenGetSlotForeign(vm, 1);
    wrenSetSlotDouble(vm, 0, lhs->dot(*rhs));
}

void vec3fCross(WrenVM* vm) {
    const auto* lhs = (const Vec3f*)wrenGetSlotForeign(vm, 0);
    const auto* rhs = (const Vec3f*)wrenGetSlotForeign(vm, 1);
    Vec3f result = lhs->cross(*rhs);
    // return a new Vec3f to wren
    void* bytes = wrenSetSlotNewForeign(vm, 0, 0, sizeof(Vec3f));
    new (bytes) Vec3f(result);
}

void vec3fGetX(WrenVM* vm) {
    const auto* v = (const Vec3f*)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, v->x);
}

void vec3fSetX(WrenVM* vm) {
    auto* v = (Vec3f*)wrenGetSlotForeign(vm, 0);
    double newx = wrenGetSlotDouble(vm, 1);
    v->x = newx;
}

void vec3fGetY(WrenVM* vm) {
    const auto* v = (const Vec3f*)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, v->y);
}

void vec3fSetY(WrenVM* vm) {
    Vec3f* v = (Vec3f*)wrenGetSlotForeign(vm, 0);
    double newx = wrenGetSlotDouble(vm, 1);
    v->y= newx;
}

void vec3fGetZ(WrenVM* vm) {
    const auto* v = (const Vec3f*)wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, v->y);
}

void vec3fSetZ(WrenVM* vm) {
    Vec3f* v = (Vec3f*)wrenGetSlotForeign(vm, 0);
    double newx = wrenGetSlotDouble(vm, 1);
    v->y= newx;
}