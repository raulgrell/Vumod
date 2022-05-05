#pragma once

#include <math/Vec.h>
#include "common.h"

void vec3fAllocate(WrenVM *vm) {
    void *bytes = wrenSetSlotNewForeign(vm, 0, 0, sizeof(Vec3));
    new(bytes) Vec3(wrenGetSlotDouble(vm, 1), wrenGetSlotDouble(vm, 2), wrenGetSlotDouble(vm, 3));
}

void vec3fFinalize(void *bytes) {}

void vec3fNorm(WrenVM *vm) {
    const auto *v = (const Vec3 *) wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, v->Length());
}

void vec3fDot(WrenVM *vm) {
    const auto *lhs = (const Vec3 *) wrenGetSlotForeign(vm, 0);
    const auto *rhs = (const Vec3 *) wrenGetSlotForeign(vm, 1);
    wrenSetSlotDouble(vm, 0, Vec3::Dot(*lhs, *rhs));
}

void vec3fCross(WrenVM *vm) {
    const auto *lhs = (const Vec3 *) wrenGetSlotForeign(vm, 0);
    const auto *rhs = (const Vec3 *) wrenGetSlotForeign(vm, 1);
    Vec3 result = Vec3::Cross(*lhs, *rhs);
    void *bytes = wrenSetSlotNewForeign(vm, 0, 0, sizeof(Vec3));
    new(bytes) Vec3(result);
}

void vec3fGetX(WrenVM *vm) {
    const auto *v = (const Vec3 *) wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, v->x);
}

void vec3fSetX(WrenVM *vm) {
    auto *v = (Vec3 *) wrenGetSlotForeign(vm, 0);
    double newx = wrenGetSlotDouble(vm, 1);
    v->x = newx;
}

void vec3fGetY(WrenVM *vm) {
    const auto *v = (const Vec3 *) wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, v->y);
}

void vec3fSetY(WrenVM *vm) {
    auto *v = (Vec3 *) wrenGetSlotForeign(vm, 0);
    double newx = wrenGetSlotDouble(vm, 1);
    v->y = newx;
}

void vec3fGetZ(WrenVM *vm) {
    const auto *v = (const Vec3 *) wrenGetSlotForeign(vm, 0);
    wrenSetSlotDouble(vm, 0, v->y);
}

void vec3fSetZ(WrenVM *vm) {
    auto *v = (Vec3 *) wrenGetSlotForeign(vm, 0);
    double newx = wrenGetSlotDouble(vm, 1);
    v->y = newx;
}