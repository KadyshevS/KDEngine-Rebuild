#pragma once

#include <KDEngine/Renderer/Framebuffer.h>

namespace KDE
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& specification);
		virtual ~OpenGLFramebuffer();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

		virtual uint32_t GetColorAttachment(uint32_t index = 0) const override 
		{  
			KD_CORE_ASSERT(index < m_ColorAttachments.size(), "Index of color attachments out of range");
			return m_ColorAttachments[index];
		}

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
		uint32_t GetRendererID() const { return m_RendererID; }

		void Invalidate();
	private:
		uint32_t m_RendererID = 0;
		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecs;
		FramebufferTextureSpecification m_DepthAttachmentSpec;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;
	};
}