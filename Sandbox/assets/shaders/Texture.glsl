#type vertex
#version 330 core
			
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inTexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 texCoord;

void main()
{
	gl_Position = u_ViewProjection * u_Transform * vec4(inPos, 1.0f);
	texCoord = inTexCoord;
}

#type fragment
#version 330 core
			
layout(location = 0) out vec4 fragColor;

uniform sampler2D u_Texture;

in vec2 texCoord;

void main()
{
	fragColor = texture(u_Texture, texCoord);
}