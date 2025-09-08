#version 440
layout(location = 0) in vec2 coord;
layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    float qt_Opacity;
    float uu_time;
    vec2 u_mouse;
    vec2 u_resolution;
};

vec3 mandelbrot(vec2 uv, vec2 offset) {
    vec2 c = uv + offset;
    vec2 z = vec2(0.0);
    int maxIter = 80;
    int i;
    for(i = 0; i < maxIter; i++) {
        float x = z.x * z.x - z.y * z.y + c.x;
        float y = 2.0 * z.x * z.y + c.y;
        z = vec2(x, y);
        if(length(z) > 2.0) break;
    }

    if(i == maxIter) return vec3(0.1);
    float t = float(i) / float(maxIter);
    return (0.7 + 0.5*sin(6.2831*vec3(t, t*1.3, t*1.7))) * exp(-2.0*t);
}

void main() {
    vec2 uv = (coord - 0.5) * 3.0;
    uv.x *= u_resolution.x / u_resolution.y;

    vec2 offset1 = 0.3 * vec2(sin(uu_time*0.5), cos(uu_time*0.3));
    vec2 offset2 = 0.3 * vec2(cos(uu_time*0.4), sin(uu_time*0.6)*sin(uu_time*0.6));
    vec2 offset3 = 0.3 * vec2(cos(-uu_time*0.4), sin(-uu_time*0.6));

    vec3 color1 = mandelbrot(uv, offset1 + vec2(0.3));
    vec3 color2 = mandelbrot(uv, offset2 - vec2(0.5));
    vec3 color3 = mandelbrot(uv, offset3 +vec2(-0.9, 0.5));
    vec3 color = max(color1, color2);
    color = max(color, color3);

    fragColor = vec4(color, 1.0);
}
