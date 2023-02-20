#type vertex
#version 330 core
			
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 color;

void main()
{
	gl_Position = u_ViewProjection * u_Transform * vec4(inPos, 1.0f);
	color = inColor;
}

#type fragment
#version 330 core
			
layout(location = 0) out vec4 fragColor;

in vec3 color;

void main()
{
	fragColor = vec4(color, 1.0f);
}