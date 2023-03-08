#include <kdpch.h>
#include "SubTexture2D.h"

namespace KDE
{
	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
		: m_Texture(texture)
	{
		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };
	}

	Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& size, const glm::vec2& tileSize)
	{
		const glm::vec2 min = 
		{ 
			coords.x * size.x / texture->GetWidth(), 
			coords.y * size.y / texture->GetHeight() 
		};
		const glm::vec2 max = 
		{ 
			(coords.x + tileSize.x) * size.x / texture->GetWidth(), 
			(coords.y + tileSize.y) * size.y / texture->GetHeight()
		};

		return MakeRef<SubTexture2D>(texture, min, max);
	}
}