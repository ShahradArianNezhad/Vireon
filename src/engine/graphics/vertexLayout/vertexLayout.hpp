#pragma once
#include <format>
#include <string_view>
enum class VertexLayout { Pos, PosColor, PosUV };


constexpr std::string_view to_string(VertexLayout c) {
    switch (c) {
      case VertexLayout::Pos:   return "Pos";
      case VertexLayout::PosColor: return "PosColor";
      case VertexLayout::PosUV:  return "PosUV";
    }

    return "Unknown";
}

template <>
struct std::formatter<VertexLayout> : std::formatter<std::string_view> {
    auto format(VertexLayout c, format_context& ctx) const {
        return std::formatter<std::string_view>::format(
            to_string(c),
            ctx
        );
    }
};
