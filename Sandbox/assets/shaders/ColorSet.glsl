#type vertex
#version 330 core
			
layout(location = 0) in vec3 inPos;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

uniform vec3 u_Color;

out vec3 color;

void main()
{
	gl_Position = u_ViewProjection * u_Transform * vec4(inPos, 1.0f);
	color = u_Color;
}

#type fragment
#version 330 core
			
layout(location = 0) out vec4 fragColor;

in vec3 color;

void main()
{
	fragColor = vec4(color, 1.0f);
}