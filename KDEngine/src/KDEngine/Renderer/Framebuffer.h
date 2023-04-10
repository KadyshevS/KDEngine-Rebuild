#pragma once

#include <KDEngine/Core.h>

namespace KDE
{
	enum class FramebufferTextureFormat
	{
		None = 0,

	//	Color
		RGBA8,
		RED_INTEGER,

	//	Depth/Stencil
		DEPTH24STENCIL8,

	//	Default
		Depth = DEPTH24STENCIL8,
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			: TextureFormat(format) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: Attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> Attachments;
	};

	struct FramebufferSpecification
	{
		FramebufferAttachmentSpecification Attachments;
		uint32_t Width = 0, Height = 0;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class KD_API Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;

		virtual uint32_t GetColorAttachment(uint32_t index = 0) const = 0;
		
		virtual const FramebufferSpecification& GetSpecification() const = 0;
	private:
	};
}