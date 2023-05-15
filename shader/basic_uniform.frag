#version 460

in vec3 Colour;
in vec3 Vec;

layout (location = 0) out vec4 FragColor;

layout (binding = 0) uniform samplerCube SkyBoxTex;
layout (binding = 1) uniform sampler2D NoiseTex;

uniform float noiseScale = 1.0;
uniform float noiseStrength = 0.2;
uniform bool cloudsEnabled = true;

void main() {
    // Calculate noise for clouds
    vec3 noiseVec = normalize(Vec) * noiseScale;
    float noiseVal = texture(NoiseTex, noiseVec.xy).r;
    noiseVal = (noiseVal - 0.5) * 2.0;

    // Calculate skybox textures
    vec3 texColour = texture(SkyBoxTex, normalize(Vec)).rgb;

    if (cloudsEnabled == true)
    {
        // Apply clouds to skybox
        texColour += noiseVal * noiseStrength;
    }
    
    FragColor = vec4( Colour + texColour, 1.0 );
}
