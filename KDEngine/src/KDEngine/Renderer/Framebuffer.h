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

		virtual uint32_t GetColorAttachment() const = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;
	private:
	};
}