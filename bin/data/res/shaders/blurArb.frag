// fragment shader
#version 150

// this is how we receive the texture
uniform sampler2DRect tex0;
uniform int diameter;
uniform float kernel[625];
uniform vec3 channel;


in vec2 varyingtexcoord;
out vec4 outputColor;


void main()
{
    int size = diameter;
    if(size > 25)size = 25;

    vec4 color = vec4(0.0,0.0,0.0,1.0);
    int radius = (size-1)/2;
    vec2 pos  = varyingtexcoord;
    for(int y = -radius; y <= radius ; y++){
        for(int x = -radius; x <= radius ; x++){
			vec4 pixelColor = vec4(texture(tex0, pos + vec2(float(x), float(y))).rgb * channel,1);
            color +=  pixelColor * kernel[(y+radius) * size + (x+radius)];
        }
    }
    outputColor = color;
}
