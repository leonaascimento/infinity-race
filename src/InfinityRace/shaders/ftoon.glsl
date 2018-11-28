#version 410

in vec3 fN;
in vec3 fL;

uniform vec4 ambientProduct;
uniform vec4 diffuseProduct;

out vec4 frag_color;

float Toon(vec3 l, vec3 n)
{
    const int levels = 3;

    float cosine = max(0.0, dot(l, n));
    return floor(cosine * levels) / levels;
}

void main()
{
    frag_color = ambientProduct + diffuseProduct * Toon(normalize(fN), normalize(fL));
    frag_color.a = 1.0;
}
