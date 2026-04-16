#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform float time;
uniform float aberrationAmount; // 0.0 a 1.0
uniform vec2 resolution;

out vec4 finalColor;

void main()
{
    vec2 uv = fragTexCoord;
    
    // 1. Chromatic Aberration
    float amount = aberrationAmount * 0.005;
    vec4 r = texture(texture0, uv + vec2(amount, 0.0));
    vec4 g = texture(texture0, uv);
    vec4 b = texture(texture0, uv - vec2(amount, 0.0));
    vec4 texelColor = vec4(r.r, g.g, b.b, g.a);

    // 2. Scanlines
    float scanline = sin(uv.y * resolution.y * 2.0) * 0.04;
    texelColor.rgb -= scanline;

    // 3. Vignette (escurecer bordas)
    float dist = distance(uv, vec2(0.5));
    texelColor.rgb *= smoothstep(0.8, 0.4, dist * 0.9);

    finalColor = texelColor * colDiffuse * fragColor;
}
