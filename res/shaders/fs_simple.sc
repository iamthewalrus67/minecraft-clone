$input v_texcoord0, v_normal, v_color0

#include <bgfx_shader.sh>

SAMPLER2D(s_texColor, 0);

void main() {
    vec4 color = texture2D(s_texColor, v_texcoord0);

    vec3 outCol = color.xyz * v_color0.xyz;

    gl_FragColor = vec4(outCol, 1.0f);
}