#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;
layout (binding = 0) uniform sampler2D Tex1;

out vec3 Colour;
out vec3 Vec;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

uniform struct LightInfo {
    vec4 Position; // Light position in eye coords.
    vec3 La; // Ambient light intensity
    vec3 L; // Diffuse and specular light intensity
} lights[3];

uniform struct MaterialInfo {
    vec3 Ka; // Ambient reflectivity
    vec3 Kd; // Diffuse reflectivity
    vec3 Ks; // Specular reflectivity
    float Shininess; // Specular shininess factor
} Material;

// Phong shading
vec3 phongModel( int light, vec3 position, vec3 n )
{
    // Calculate ambient using texture
    vec3 texColour = texture(Tex1, VertexTexCoord).rgb;
    vec3 ambient = texColour * lights[light].La;

    // Calculate diffuse
    vec3 s = vec3(lights[light].Position) - ( VertexPosition * lights[light].Position.w );

    float sDotN = max(dot(n, s), 0.0); //calculate dot product between s & n
    vec3 diffuse = Material.Kd * sDotN;

    // Calculate specular
    vec3 spec = vec3(0.0);
    
    if( sDotN > 0.0 )
    {
        vec3 v = normalize(-position.xyz);
        vec3 r = reflect( -s, n );
        spec = Material.Ks * pow( max( dot(r,v), 0.0 ),
        Material.Shininess );
    }

    return ambient + lights[light].L * (diffuse + spec);
}

void main()
{
    vec3 n = normalize( NormalMatrix * VertexNormal );

    vec3 camCoords = vec3(0.0);

    Colour = vec3(0.0);
    for( int i = 0; i < 3; i++ )
        Colour += phongModel( i, camCoords, n );

    Vec = VertexPosition;

    gl_Position = MVP * vec4(VertexPosition,1.0);
}
