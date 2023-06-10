$input a_position, a_normal, a_texcoord0
$output v_texcoord0, v_normal

#include <bgfx_shader.sh>

void main() {
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));

    v_texcoord0 = a_texcoord0;
    //v_color0 = a_color0;
    v_normal = normalize(a_normal) * 0.5 + 0.5; // normalize, compress
}