#version 460

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

layout (binding = 0) uniform sampler2D Tex1;

uniform struct SpotLightInfo {
	vec3 Position; // Position in cam coords
	vec3 L; // Diffuse/spec intensity
	vec3 La; // Amb intensity
	vec3 Direction; // Direction of the spotlight in cam coords.
	float Exponent; // Angular attenuation exponent
	float Cutoff; // Cutoff angle (between 0 and pi/2)
} Spot;

uniform struct MaterialInfo {
    vec3 Ka; // Ambient reflectivity
    vec3 Kd; // Diffuse reflectivity
    vec3 Ks; // Specular reflectivity
    float Shininess; // Specular shininess factor
} Material;

layout( location = 0 ) out vec4 FragColor;

vec3 blinnPhongSpot( vec3 position, vec3 n )
{
	vec3 texColor = texture(Tex1, TexCoord).rgb;

	vec3 ambient = Spot.La * texColor; //calculate ambient
	vec3 s = Spot.Position - ( Position * 1.0f ); //calculate s vector
	float cosAng = dot(-s, normalize(Spot.Direction)); //cosine of the angle
	float angle = acos( cosAng ); //gives you the actual angle
	float spotScale = 0.0;

	vec3 diffuse = vec3(0.0f);
	vec3 spec = vec3(0.0);

	if(angle < Spot.Cutoff )
	{
		spotScale = pow( cosAng, Spot.Exponent );
		float sDotN = max(dot(n, s), 0.0); //calculate dot product between s and n
		diffuse = Material.Kd * sDotN; //calculate the diffuse

		if( sDotN > 0.0 )
		{
			vec3 v = normalize(-position.xyz);
			vec3 r = reflect( -s, n );
			spec = Material.Ks * pow( max( dot(r,v), 0.0 ),
			Material.Shininess );
		}
	}

	return ambient + spotScale * Spot.L * (diffuse + spec);
}

void main()
{
	FragColor = vec4(blinnPhongSpot(Position, normalize(Normal)), 1);
}