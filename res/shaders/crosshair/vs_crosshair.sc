$input a_position, a_texcoord0
$output v_texcoord0

#include <bgfx_shader.sh>

void main() {
    gl_Position = vec4(a_position, 1.0f);
    v_texcoord0 = a_texcoord0;
}