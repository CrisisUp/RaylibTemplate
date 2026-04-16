#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

out vec4 finalColor;

void main()
{
    vec4 source = texture(texture0, fragTexCoord);
    
    // Filtro de brilho (Threshold)
    // Apenas cores com brilho > 0.7 contribuem para o bloom
    float brightness = dot(source.rgb, vec3(0.2126, 0.7152, 0.0722));
    vec4 bloomColor = vec4(0.0);
    if (brightness > 0.6) bloomColor = source;

    // Simulação simplificada de blur (amostragem 3x3)
    vec2 size = vec2(1.0) / textureSize(texture0, 0);
    vec4 sum = vec4(0.0);
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            sum += texture(texture0, fragTexCoord + vec2(x, y) * size);
        }
    }
    vec4 blur = sum / 9.0;

    finalColor = source + (blur * 0.4); // Combina a cena original com o brilho
}
