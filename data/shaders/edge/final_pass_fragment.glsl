#version 330 core
uniform sampler2D renderTex;
uniform vec2 u_viewPort;
in vec2 uv;

out vec4 finalColor;

uniform float Weight[5];


vec4 pass3() {
    ivec2 pix = ivec2( gl_FragCoord.xy );
    vec4 sum = texelFetch(renderTex, pix, 0) * Weight[0];
    sum += texelFetchOffset( renderTex, pix, 0, ivec2(1,0) ) * Weight[1];
    sum += texelFetchOffset( renderTex, pix, 0, ivec2(-1,0) ) * Weight[1];
    sum += texelFetchOffset( renderTex, pix, 0, ivec2(2,0) ) * Weight[2];
    sum += texelFetchOffset( renderTex, pix, 0, ivec2(-2,0) ) * Weight[2];
    sum += texelFetchOffset( renderTex, pix, 0, ivec2(3,0) ) * Weight[3];
    sum += texelFetchOffset( renderTex, pix, 0, ivec2(-3,0) ) * Weight[3];
    sum += texelFetchOffset( renderTex, pix, 0, ivec2(4,0) ) * Weight[4];
    sum += texelFetchOffset( renderTex, pix, 0, ivec2(-4,0) ) * Weight[4];
    return sum;
}

void main()
{
    vec2 tex_coord = gl_FragCoord.xy/u_viewPort; // map to [0..1]


    finalColor = pass3();
    // finalColor = texture2D(renderTex, tex_coord);


}
