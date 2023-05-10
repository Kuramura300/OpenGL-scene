#version 460

in vec3 Colour;
in vec3 Vec;

layout (location = 0) out vec4 FragColor;

layout (binding = 0) uniform samplerCube SkyBoxTex;

void main() {
    vec3 texColour = texture(SkyBoxTex, normalize(Vec)).rgb;
    FragColor = vec4( Colour + texColour, 1.0 );
}
