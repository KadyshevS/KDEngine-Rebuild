#type vertex
#version 410 core
			
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in float inTexIndex;
layout(location = 4) in float inTexScalingFactor;

uniform mat4 u_ViewProjection;

out vec2 texCoord;
out vec4 color;
out float texIndex;
out float scalingFactor;

void main()
{
	gl_Position = u_ViewProjection * vec4(inPos, 1.0f);
	texCoord = inTexCoord;
	color = inColor;
	texIndex = inTexIndex;
	scalingFactor = inTexScalingFactor;
}

#type fragment
#version 410 core
			
layout(location = 0) out vec4 fragColor;

in vec2 texCoord;
in vec4 color;
in float texIndex;
in float scalingFactor;

uniform sampler2D u_Textures[32];

void main()
{
	fragColor = texture(u_Textures[int(texIndex)], texCoord * scalingFactor) * color;
//	fragColor = color;
}