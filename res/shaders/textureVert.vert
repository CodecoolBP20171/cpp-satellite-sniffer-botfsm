#version 420

layout(location = 0) in vec2 in_pos;
layout(location = 1) in vec2 in_tex;

layout(location = 0) out vec2 out_tex;

uniform vec2 zoom_center;
uniform float zoom_level;

void main() {
    out_tex = in_tex;
    vec2 zc = zoom_center * 2 - 1;
    vec2 pos = in_pos * 2 - 1;
    zc.y *= -1;
    pos.y *= -1;
    vec2 final_pos = (pos - zc) * zoom_level;
    gl_Position = vec4(final_pos, 0, 1);
}