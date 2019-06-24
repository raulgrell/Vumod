#include <stb/stb_image.h>
#include <imgui/imconfig.h>
#include <imgui/imgui.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui_internal.h>

#include <linmath/linmath.h>
#include <tinydir/tinydir.h>
#include <tinyobjloader/tiny_obj_loader.h>

extern "C" {
    #include <wren/wren.h>
}