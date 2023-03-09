#pragma once

#include <KDEngine/Renderer/Framebuffer.h>

namespace KDE
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetColorAttachment() const override { return m_ColorAttachment; }

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
		uint32_t GetRendererID() const { return m_RendererID; }

		void Invalidate();
	private:
		uint32_t m_RendererID;
		uint32_t m_ColorAttachment;
		uint32_t m_DepthAttachment;
		FramebufferSpecification m_Specification;
	};
}