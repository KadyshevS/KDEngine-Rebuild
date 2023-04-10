#type vertex
#version 450 core
			
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in float inTexIndex;
layout(location = 4) in float inTexScalingFactor;
layout(location = 5) in int inEntityID;

uniform mat4 u_ViewProjection;

out vec2 texCoord;
out vec4 color;
out flat float texIndex;
out float scalingFactor;
out int entityID;

void main()
{
	gl_Position = u_ViewProjection * vec4(inPos, 1.0f);
	texCoord = inTexCoord;
	color = inColor;
	texIndex = inTexIndex;
	scalingFactor = inTexScalingFactor;
	entityID = inEntityID;
}

#type fragment
#version 450 core
			
layout(location = 0) out vec4 fragColor;
layout(location = 1) out int fragColor2;

in vec2 texCoord;
in vec4 color;
in flat float texIndex;
in float scalingFactor;
in flat int entityID;

uniform sampler2D u_Textures[32];

void main()
{
	vec4 l_Texture;
	int l_TexIndex = int(texIndex);

	switch (l_TexIndex) 
	{
		case 0:  l_Texture = texture(u_Textures[0],  texCoord * scalingFactor); break;
		case 1:  l_Texture = texture(u_Textures[1],  texCoord * scalingFactor); break;
		case 2:  l_Texture = texture(u_Textures[2],  texCoord * scalingFactor); break;
		case 3:  l_Texture = texture(u_Textures[3],  texCoord * scalingFactor); break;
		case 4:  l_Texture = texture(u_Textures[4],  texCoord * scalingFactor); break;
		case 5:  l_Texture = texture(u_Textures[5],  texCoord * scalingFactor); break;
		case 6:  l_Texture = texture(u_Textures[6],  texCoord * scalingFactor); break;
		case 7:  l_Texture = texture(u_Textures[7],  texCoord * scalingFactor); break;
		case 8:  l_Texture = texture(u_Textures[8],  texCoord * scalingFactor); break;
		case 9:  l_Texture = texture(u_Textures[9],  texCoord * scalingFactor); break;
		case 10: l_Texture = texture(u_Textures[10], texCoord * scalingFactor); break;
		case 11: l_Texture = texture(u_Textures[11], texCoord * scalingFactor); break;
		case 12: l_Texture = texture(u_Textures[12], texCoord * scalingFactor); break;
		case 13: l_Texture = texture(u_Textures[13], texCoord * scalingFactor); break;
		case 14: l_Texture = texture(u_Textures[14], texCoord * scalingFactor); break;
		case 15: l_Texture = texture(u_Textures[15], texCoord * scalingFactor); break;
		case 16: l_Texture = texture(u_Textures[16], texCoord * scalingFactor); break;
		case 17: l_Texture = texture(u_Textures[17], texCoord * scalingFactor); break;
		case 18: l_Texture = texture(u_Textures[18], texCoord * scalingFactor); break;
		case 19: l_Texture = texture(u_Textures[19], texCoord * scalingFactor); break;
		case 20: l_Texture = texture(u_Textures[20], texCoord * scalingFactor); break;
		case 21: l_Texture = texture(u_Textures[21], texCoord * scalingFactor); break;
		case 22: l_Texture = texture(u_Textures[22], texCoord * scalingFactor); break;
		case 23: l_Texture = texture(u_Textures[23], texCoord * scalingFactor); break;
		case 24: l_Texture = texture(u_Textures[24], texCoord * scalingFactor); break;
		case 25: l_Texture = texture(u_Textures[25], texCoord * scalingFactor); break;
		case 26: l_Texture = texture(u_Textures[26], texCoord * scalingFactor); break;
		case 27: l_Texture = texture(u_Textures[27], texCoord * scalingFactor); break;
		case 28: l_Texture = texture(u_Textures[28], texCoord * scalingFactor); break;
		case 29: l_Texture = texture(u_Textures[29], texCoord * scalingFactor); break;
		case 30: l_Texture = texture(u_Textures[30], texCoord * scalingFactor); break;
		case 31: l_Texture = texture(u_Textures[31], texCoord * scalingFactor); break;
	}

	fragColor = l_Texture * color;
	fragColor2 = entityID;

//	fragColor = texture(u_Textures[int(texIndex)], texCoord * scalingFactor) * color;
//	fragColor = color;
}