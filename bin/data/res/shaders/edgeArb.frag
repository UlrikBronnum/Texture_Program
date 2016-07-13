// fragment shader
#version 150

// this is how we receive the texture
uniform sampler2DRect   tex0;

uniform int             Hkernel[9];
uniform int             Vkernel[9];

uniform vec3            greyConvertion;
uniform int             outputSetting;

in vec2                 varyingtexcoord;
out vec4                outputColor;

void main()
{
    float valueH = 0.0;
    float valueV = 0.0;

    int diameter = 3;
    int radius = (diameter-1)/2;

    for(int y = -radius; y <= radius ; y++){
        for(int x = -radius; x <= radius ; x++){
            vec4 color = texture(tex0, varyingtexcoord + vec2(float(x), float(y)));

            float grey = dot(color.rgb,greyConvertion);

            valueH += grey * float(Hkernel[(y+1) * diameter + (x+1)]);
            valueV += grey * float(Vkernel[(y+1) * diameter + (x+1)]);
        }
    }
    vec3 c;
    if(outputSetting == 0){
        valueV = (abs(valueV) > 0.01) ? 1.0 : 0.0;
        valueH = (abs(valueH) > 0.01) ? 1.0 : 0.0;
        c = vec3(abs(valueV),abs(valueH),0.0);
    }else
    if(outputSetting == 1){
        c = vec3(abs(valueV),abs(valueH),0.0) * 2.0;
    }else
    if(outputSetting == 2){
        c = vec3(0.5 + valueV,0.5 + valueH,0.0);
    }
    outputColor = vec4(c,1.0);
}
