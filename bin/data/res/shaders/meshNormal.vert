#version 150


// these are for the programmable pipeline system and are passed in
// by default from OpenFrameworks
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec4 color;
in vec3 normal;
in vec2 texcoord;
in vec3 tangent;

// this is the end of the default functionality

// this is something we're creating for this shader
out vec2 varyingtexcoord;
out vec4 vPosition;
out mat3 tbnMat;

void main()
{
    // here we move the texture coordinates
    varyingtexcoord = texcoord ;

    vec3 n = normalize(modelViewMatrix * vec4(normalize(normal),0.0)).xyz;
    vec3 t = normalize(modelViewMatrix * vec4(normalize(tangent),0.0)).xyz;
    t = normalize(t - dot(t,n) * n);
    vec3 bT = cross(t,n);
    tbnMat = mat3(t,bT,n);


    vPosition = modelViewProjectionMatrix * position;

    gl_Position = modelViewProjectionMatrix * position;
}
