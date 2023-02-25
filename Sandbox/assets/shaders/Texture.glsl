#type vertex
#version 330 core
			
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inTexCoord;

uniform mat4 u_ViewProjection;

out vec2 texCoord;
out vec4 color;

void main()
{
	gl_Position = u_ViewProjection * vec4(inPos, 1.0f);
	texCoord = inTexCoord;
	color = inColor;
}

#type fragment
#version 330 core
			
layout(location = 0) out vec4 fragColor;

uniform sampler2D u_Texture;
uniform float u_ScalingFactor;
uniform vec4 u_Color;

in vec2 texCoord;
in vec4 color;

void main()
{
//	fragColor = texture(u_Texture , texCoord * u_ScalingFactor) * u_Color;
	fragColor = color;
}