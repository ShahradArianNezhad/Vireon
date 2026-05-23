#pragma once
#include "engine/materialManager/materialManager.hpp"
#include "engine/meshManager/meshManager.hpp"
#include <glm/glm.hpp>

using ComponentId = uint32_t;
using EntityId = uint32_t;

struct RenderComponent {
  MeshID mesh;
  MaterialID material;
  uint32_t color=0xFFFFFFFF;
  bool visible=true;
};

struct TransformComponent {
  vec3 position={0,0,0};

  vec3 scale={0,0,0};
  float rotation={0};
};

struct CircleColliderComponent{
  float radius;
  vec2 offset={0,0};
};

struct RectColliderComponent{
  vec2 offset={0,0};
  vec2 scale={0,0};
  float rotation;
};

struct CameraComponent2D{
  vec2 position={0,0};
  float rotation=0;
  float zoom=1.0f;
};

enum ComponentType{
  RENDER,
  TRANSFORM,
  CIRCLECOLLIDER,
  RECTCOLLIDER,
  CAMERA2D,
};


constexpr std::string_view to_string(ComponentType c) {
  switch (c) {
    case ComponentType::RENDER:   return "render component";
    case ComponentType::TRANSFORM: return "Transform Component";
    case ComponentType::CIRCLECOLLIDER:  return "Circle collider component";
    case ComponentType::RECTCOLLIDER:  return "Rect collider component";
    case ComponentType::CAMERA2D: return "2d camera component";
  }
  return "Unknown";
}

template <>
struct std::formatter<ComponentType> : std::formatter<std::string_view> {
  auto format(ComponentType c, format_context& ctx) const {
    return std::formatter<std::string_view>::format(
        to_string(c),
        ctx
        );
  }
};



template<ComponentType I>
struct EnumToType {
  using type = std::tuple_element_t<static_cast<size_t>(I), 
        std::tuple<RenderComponent,TransformComponent,CircleColliderComponent,RectColliderComponent,CameraComponent2D>>;
};



template <>
struct std::formatter<RenderComponent> {
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    auto format(const RenderComponent& rc, std::format_context& ctx) const
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
struct std::formatter<TransformComponent> {
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    auto format(const TransformComponent& tc, std::format_context& ctx) const
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
struct std::formatter<CircleColliderComponent> {
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    auto format(const CircleColliderComponent& cc, std::format_context& ctx) const
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
struct std::formatter<RectColliderComponent> {
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    auto format(const RectColliderComponent& cc, std::format_context& ctx) const
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
struct std::formatter<CameraComponent2D> {
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    auto format(const CameraComponent2D& cc, std::format_context& ctx) const
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
