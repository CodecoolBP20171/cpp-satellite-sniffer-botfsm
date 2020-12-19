#pragma once

namespace Graphics {
struct rgba_color {
  float r, g, b, a;
};
struct texture_vertex {
  float posx, posy, texx, texy;
};
struct color_vertex {
// ignore gcc maybe unused attribute warning
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
  [[maybe_unused]] float posx, posy, colr, colg, colb;
#pragma GCC diagnostic pop
};
} // namespace Graphics