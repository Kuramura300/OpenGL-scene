#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 Colour;

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

vec3 phongModel( int light, vec3 position, vec3 n )
{
    //calculate ambient here, to access each light La value use this:
    //lights[light].La
    vec3 ambient = Material.Ka * lights[light].La;

    //calculate diffuse here
    vec3 s = vec3(lights[light].Position) - ( VertexPosition * lights[light].Position.w ); //find out s vector

    float sDotN = max(dot(n, s), 0.0); //calculate dot product between s & n
    vec3 diffuse = Material.Kd * sDotN;

    //calculate specular here
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
    vec4 p = normalize( ModelViewMatrix * vec4( VertexPosition, 1.0 ) );

    vec3 camCoords = vec3(0.0);

    Colour = vec3(0.0);
    for( int i = 0; i < 3; i++ )
        Colour += phongModel( i, camCoords, n ); // What is camCoords?

    gl_Position = MVP * vec4(VertexPosition,1.0);
}
