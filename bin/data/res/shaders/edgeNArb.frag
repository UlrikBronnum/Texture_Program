// fragment shader
#version 150

// this is how we receive the texture
uniform sampler2D       tex0;
uniform float           texSize;

uniform int             Hkernel[9];
uniform int             Vkernel[9];

uniform vec3            greyConvertion;

in vec2                 varyingtexcoord;
out vec4                outputColor;

void main()
{
    float valueH = 0.0;
    float valueV = 0.0;

    int diameter = 3;
    int radius = (diameter-1)/2;

    float stepSize = 1.0 / texSize;

    for(int y = -radius; y <= radius ; y++){
        for(int x = -radius; x <= radius ; x++){
            vec4 color = texture(tex0, varyingtexcoord + vec2(float(x), float(y)) * stepSize);

            float grey = dot(color.rgb,greyConvertion);

            valueH += grey * float(Hkernel[(y+1) * diameter + (x+1)]);
            valueV += grey * float(Vkernel[(y+1) * diameter + (x+1)]);
        }
    }
    vec3 c = vec3(0.5 + valueV,0.5 + valueH,0.0);

    outputColor = vec4(c,1.0);
}
