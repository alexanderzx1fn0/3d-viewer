#version 330 core
uniform sampler2D renderTex;

uniform vec2 u_viewPort;

layout (location = 0) out vec4 finalColor;


uniform float Weight[5];

vec4 pass2() {
    ivec2 pix = ivec2( gl_FragCoord.xy );
    vec4 sum = texelFetch(renderTex, pix, 0) * Weight[0];
    sum += texelFetchOffset( renderTex, pix, 0, ivec2(0,1) ) * Weight[1];
    sum += texelFetchOffset( renderTex, pix, 0, ivec2(0,-1) ) * Weight[1];
    sum += texelFetchOffset( renderTex, pix, 0, ivec2(0,2) ) * Weight[2];
    sum += texelFetchOffset( renderTex, pix, 0, ivec2(0,-2) ) * Weight[2];
    sum += texelFetchOffset( renderTex, pix, 0, ivec2(0,3) ) * Weight[3];
    sum += texelFetchOffset( renderTex, pix, 0, ivec2(0,-3) ) * Weight[3];
    sum += texelFetchOffset( renderTex, pix, 0, ivec2(0,4) ) * Weight[4];
    sum += texelFetchOffset( renderTex, pix, 0, ivec2(0,-4) ) * Weight[4];
    return sum;
}

void main()
{
    vec2 tex_coord = gl_FragCoord.xy/u_viewPort; // map to [0..1]
    finalColor = pass2();

//     finalColor = texture2D(renderTex, tex_coord);
}
