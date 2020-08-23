#version 420

layout(location = 0) in vec2 in_tex;

layout(location = 0) out vec4 out_col;

uniform sampler2D tex_sampl;

void main(){
    out_col = texture(tex_sampl,in_tex);
}