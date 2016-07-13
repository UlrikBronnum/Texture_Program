// fragment shader
#version 150

// this is how we receive the texture
uniform sampler2D       tex0;
uniform sampler2D       tex1;

uniform vec4            lightPos = vec4(100,100,100,1);
uniform vec3            ambientLight = vec3(0.7);

in vec4                 vPosition;
in mat3                 tbnMat;
in vec2                 varyingtexcoord;

out vec4                outputColor;

void main()
{
    vec3 pColor         = texture(tex0,varyingtexcoord).rgb;
    vec3 pNormal        = normalize(tbnMat * (255.0/128.0 * texture(tex1,varyingtexcoord ).xyz - vec3(1.0) ).rgb);

    vec3 lightDir;

    if(lightPos.w == 1.0){
        lightDir       = tbnMat *  normalize(lightPos.xyz-vPosition.xyz);
    }else{
        lightDir       = tbnMat *  normalize(lightPos.xyz);
    }

    vec3 lightDiffuse   = pColor * clamp( dot( pNormal, lightDir ), 0.0, 1.0);
    vec3 lightAmbient   = pColor * ambientLight;

    outputColor  = vec4(lightDiffuse + lightAmbient,1.0) ;////vec4(tbnMat * vec3(0.0,-1.0,0.0), 1.0 );
}
