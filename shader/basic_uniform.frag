#version 460

in vec3 Colour;
in vec3 Vec;

layout (location = 0) out vec4 FragColor;

layout (binding = 0) uniform samplerCube SkyBoxTex;
layout (binding = 1) uniform sampler2D NoiseTex;

uniform float noiseScale = 1.0;
uniform float noiseStrength = 0.1;

void main() {
    vec3 noiseVec = normalize(Vec) * noiseScale;
    float noiseVal = texture(NoiseTex, noiseVec.xy).r;
    noiseVal = (noiseVal - 0.5) * 2.0;

    vec3 texColour = texture(SkyBoxTex, normalize(Vec)).rgb;
    texColour += noiseVal * noiseStrength;
    FragColor = vec4( Colour + texColour, 1.0 );
}
