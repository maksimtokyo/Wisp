#version 440
layout(location = 0) in vec2 coord;
layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    float qt_Opacity;
    vec2 u_resolution; 
    vec3 topColor;
    vec3 bottomColor;
};

layout(binding = 1) uniform sampler2D baseTex;

vec4 blur(vec2 uv) {
    vec4 sum = vec4(0.0);
    vec2 step = 2.0 / u_resolution;
    for(int x=-1; x<=1; x++){
        for(int y=-1; y<=1; y++){
            sum += texture(baseTex, uv + vec2(x, y) * step);
        }
    }
    return sum / 9.0;
}

void main(void)
{

    float radius = 0.0285;
    vec2 uv = coord;
    vec4 bg = blur(uv);
    float t = uv.y;


    float alpha = 1.;
    vec3 gradient = mix(bottomColor, topColor, t);
    vec3 finalColor = mix(bg.rgb, gradient, 0.25);

    if (uv.x < radius &&  uv.y < radius){
        vec2 corner = uv - vec2(radius, radius);
        if (length(corner) > radius) discard;
    }
    if (uv.x > 1.0 - radius && uv.y < radius){
        vec2 corner = uv - vec2(1.0 - radius, radius);
        if (length(corner) > radius) discard;
    }


    fragColor =  vec4(finalColor, 1.) * qt_Opacity;
}
