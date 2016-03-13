//shader version
#version 150 core

//mode of drawing
//if is true, then use Texture
//otherwise draw gradient
uniform int useTexture;

//texture object
uniform sampler2D textureSampler;

//retrieve this data form vertex shader
// все координаты относительно камеры

in VertexData
{
	vec3 position;
	vec3 normal;
	vec2 texcoord;
} VertexIn;

uniform vec3 lightPos;
uniform vec3 lightCol;

out vec4 fragColor;

//TODO: you should use VertexIn.normal value to evaluate Phong Lightning for this pixel
// 
		
void main()
{
	if (useTexture>0)
		//take color from texture using texture2D
		fragColor = vec4(texture(textureSampler,VertexIn.texcoord.xy).rgb,length(VertexIn.normal)*length(VertexIn.position));
	else
	{
		//use default color (brown)
		fragColor = vec4(0.5,0.2,0.1,1);
	}

	vec4 surfaceColor = texture(textureSampler, VertexIn.texcoord );
	if (useTexture<=0) surfaceColor = vec4(0.5,0.2,0.1,1);

	vec3 lightColor;
	vec3 lightDir = lightPos - VertexIn.position;
	vec3 viewDir = vec3(0.0f, 0.0f, 0.0f) - VertexIn.position;
	vec3 reflectDir = reflect(-lightDir, VertexIn.normal);
	float scalar;
	const float alpha = 80.0f;
	lightDir = normalize(lightDir);
	viewDir = normalize(viewDir);
	reflectDir = normalize(reflectDir);

	// мощность фонового освещения 0.1f
	lightColor = 0.2f * vec3(1.0f, 1.0f, 1.0f) * lightCol * surfaceColor.rgb;

	// мощность рассеяного освещения 0.5f
	scalar = dot(reflectDir, VertexIn.normal);
	if (scalar < 0) scalar = 0.0f;
	lightColor += 0.6f * scalar * lightCol * surfaceColor.rgb;

	// зеркальные блики, мощность 0.3f
	scalar = dot(reflectDir, viewDir);
	if (scalar < 0) scalar = 0.0f;
	scalar = pow(scalar, alpha);
	lightColor += 0.2f * scalar * lightCol;

	fragColor = vec4( lightColor, surfaceColor.a );
}
