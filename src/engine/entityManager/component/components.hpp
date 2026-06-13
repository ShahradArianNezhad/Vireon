#pragma once
#include "engine/materialManager/materialManager.hpp"
#include "engine/meshManager/meshManager.hpp"
#include <glm/glm.hpp>

using ComponentId = uint32_t;
using EntityId = uint32_t;


namespace Component{
  struct RENDER {
    MeshID mesh;
    MaterialID material;
    uint32_t color=0xFFFFFFFF;
    bool visible=true;
  };

  struct TRANSFORM {
    vec3 position={0,0,0};

    vec3 scale={0,0,0};
    float rotation={0};
  };

  struct CIRCLECOLLIDER{
    float radius;
    vec2 offset={0,0};
  };

  struct RECTCOLLIDER{
    vec2 offset={0,0};
    vec2 scale={0,0};
    float rotation;
  };

  struct CAMERA2D{
    vec2 position={0,0};
    float rotation=0;
    float zoom=1.0f;
  };

  struct UVRECT{
    vec2 uvMin={0,0};
    vec2 uvMax={1,1};
  };

  struct LIGHT{
    float radius;
    float intensity;
    vec3 color;
  };
}

template <>
struct std::formatter<Component::RENDER> {
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    auto format(const Component::RENDER& rc, std::format_context& ctx) const
    {
        return std::format_to(
            ctx.out(),
            "RenderComponent {{ "
            "mesh: {}, "
            "material: {}, "
            "color: 0x{:08X}, "
            "visible: {} "
            "}}",
            rc.mesh,
            rc.material,
            rc.color,
            rc.visible
        );
    }
};



template <>
struct std::formatter<Component::TRANSFORM> {
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    auto format(const Component::TRANSFORM& tc, std::format_context& ctx) const
    {
        return std::format_to(
            ctx.out(),
            "TransformComponent {{ "
            "pos: {{{},{},{}}}, "
            "scale: {{{},{},{}}}, "
            "rotation: {{{}}}, "
            "}}",
            tc.position.x,tc.position.y,tc.position.z,
            tc.scale.x,tc.scale.y,tc.scale.z,
            tc.rotation
        );
    }
};


template <>
struct std::formatter<Component::CIRCLECOLLIDER> {
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    auto format(const Component::CIRCLECOLLIDER& cc, std::format_context& ctx) const
    {
        return std::format_to(
            ctx.out(),
            "CircleColliderComponent {{ "
            "offset: {{{},{}}}, "
            "radius: {}, "
            "}}",
            cc.offset.x,cc.offset.y,
            cc.radius
        );
    }
};


template <>
struct std::formatter<Component::RECTCOLLIDER> {
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    auto format(const Component::RECTCOLLIDER& cc, std::format_context& ctx) const
    {
        return std::format_to(
            ctx.out(),
            "RectColliderComponent {{ "
            "offset: {{{},{}}}, "
            "scale: {{{},{}}}, "
            "rotation: {}, "
            "}}",
            cc.offset.x,cc.offset.y,
            cc.scale.x,cc.scale.y,
            cc.rotation
        );
    }
};


template <>
struct std::formatter<Component::CAMERA2D> {
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    auto format(const Component::CAMERA2D& cc, std::format_context& ctx) const
    {
        return std::format_to(
            ctx.out(),
            "CameraComponent2D {{ "
            "position: {{{},{}}}, "
            "rotation: {{ {} }}"
            "zoom: {}, "
            "}}",
            cc.position.x,cc.position.y,
            cc.rotation,
            cc.zoom
        );
    }
};


template <>
struct std::formatter<Component::UVRECT> {
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    auto format(const Component::UVRECT& cc, std::format_context& ctx) const
    {
        return std::format_to(
            ctx.out(),
            "uv rect component {{ "
            "uvMin: {{{},{}}}, "
            "uvMax: {{ {},{} }}"
            "}}",
            cc.uvMin.x,cc.uvMin.y,
            cc.uvMax.x,cc.uvMax.y
        );
    }
};

template <>
struct std::formatter<Component::LIGHT> {
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    auto format(const Component::LIGHT& cc, std::format_context& ctx) const
    {
        return std::format_to(
            ctx.out(),
            "light component {{ "
            "radius: {{{}}}, "
            "intensity: {{ {} }}"
            "color: {{ {},{},{} }}"
            "}}",
            cc.radius,
            cc.intensity,
            cc.color[0],cc.color[1],cc.color[2]
        );
    }
};
