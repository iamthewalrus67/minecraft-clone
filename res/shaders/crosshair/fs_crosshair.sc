$input v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(s_texCrosshair, 0);

void main() {
    vec4 color = texture2D(s_texCrosshair, v_texcoord0);
    color.a *= 0.4f;

    gl_FragColor = color;
}