#pragma once

namespace Graphics {
struct rgba_color {
  float r, g, b, a;
};
struct texture_vertex {
  float posx, posy, texx, texy;
};
struct color_vertex {
  [[maybe_unused]] float posx, posy, colr, colg, colb;
};
} // namespace Graphics