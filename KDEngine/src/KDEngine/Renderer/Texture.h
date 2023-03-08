#pragma once

#include <string>

#include <KDEngine/Core.h>

namespace KDE
{
	class KD_API Texture
	{
	public:
		virtual ~Texture() {}

		virtual void Bind(uint32_t slot = 0) = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual uint32_t GetRendererID() const = 0;

		virtual bool operator == (const Texture& oth) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& path);
		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
	};
}