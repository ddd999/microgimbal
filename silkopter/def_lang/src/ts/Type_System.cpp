#include "Type_System.h"

#include "Namespace.h"
#include "types/All_Real_Types.h"
#include "types/Bool_Type.h"
#include "types/String_Type.h"

namespace ts
{

void Type_System::populate_builtin_types()
{
    add_symbol(std::unique_ptr<IType>(new Bool_Type("bool")));
    add_symbol(std::unique_ptr<IType>(new String_Type("string")));

    add_symbol(std::unique_ptr<IType>(new Float_Type("float")));
    add_symbol(std::unique_ptr<IType>(new Double_Type("double")));

    add_symbol(std::unique_ptr<IType>(new Float_Type("ufloat")));
    add_symbol(std::unique_ptr<IType>(new Double_Type("udouble")));

    {
        std::unique_ptr<Namespace> math_namespace = std::unique_ptr<Namespace>(new Namespace("math"));

        math_namespace->add_symbol(std::unique_ptr<IType>(new Vec2f_Type("vec2f")));
        math_namespace->add_symbol(std::unique_ptr<IType>(new Vec2d_Type("vec2d")));
        math_namespace->add_symbol(std::unique_ptr<IType>(new Vec2u8_Type("vec2u8")));
        math_namespace->add_symbol(std::unique_ptr<IType>(new Vec2s8_Type("vec2s8")));
        math_namespace->add_symbol(std::unique_ptr<IType>(new Vec2u16_Type("vec2u16")));
        math_namespace->add_symbol(std::unique_ptr<IType>(new Vec2s16_Type("vec2s16")));
        math_namespace->add_symbol(std::unique_ptr<IType>(new Vec2u32_Type("vec2u32")));
        math_namespace->add_symbol(std::unique_ptr<IType>(new Vec2s32_Type("vec2s32")));

        math_namespace->add_symbol(std::unique_ptr<IType>(new Vec3f_Type("vec3f")));
        math_namespace->add_symbol(std::unique_ptr<IType>(new Vec3d_Type("vec3d")));
        math_namespace->add_symbol(std::unique_ptr<IType>(new Vec3u8_Type("vec3u8")));
        math_namespace->add_symbol(std::unique_ptr<IType>(new Vec3s8_Type("vec3s8")));
        math_namespace->add_symbol(std::unique_ptr<IType>(new Vec3u16_Type("vec3u16")));
        math_namespace->add_symbol(std::unique_ptr<IType>(new Vec3s16_Type("vec3s16")));
        math_namespace->add_symbol(std::unique_ptr<IType>(new Vec3u32_Type("vec3u32")));
        math_namespace->add_symbol(std::unique_ptr<IType>(new Vec3s32_Type("vec3s32")));

        math_namespace->add_symbol(std::unique_ptr<IType>(new Vec4f_Type("vec4f")));
        math_namespace->add_symbol(std::unique_ptr<IType>(new Vec4d_Type("vec4d")));
        math_namespace->add_symbol(std::unique_ptr<IType>(new Vec4u8_Type("vec4u8")));
        math_namespace->add_symbol(std::unique_ptr<IType>(new Vec4s8_Type("vec4s8")));
        math_namespace->add_symbol(std::unique_ptr<IType>(new Vec4u16_Type("vec4u16")));
        math_namespace->add_symbol(std::unique_ptr<IType>(new Vec4s16_Type("vec4s16")));
        math_namespace->add_symbol(std::unique_ptr<IType>(new Vec4u32_Type("vec4u32")));
        math_namespace->add_symbol(std::unique_ptr<IType>(new Vec4s32_Type("vec4s32")));

        add_symbol(std::move(math_namespace));
    }
}






}

