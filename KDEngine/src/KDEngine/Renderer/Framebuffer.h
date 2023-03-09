#pragma once

#include <KDEngine/Core.h>

namespace KDE
{
	struct FramebufferSpecification
	{
		uint32_t Width = 1280, Height = 720;
		uint32_t Samples = 1;
	// TODO: FramebufferFormat

		bool SwapChainTarget = false;
	};

	class KD_API Framebuffer
	{
	public:
		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttachment() const = 0;
		
		virtual const FramebufferSpecification& GetSpecification() const = 0;
	private:
	};
}