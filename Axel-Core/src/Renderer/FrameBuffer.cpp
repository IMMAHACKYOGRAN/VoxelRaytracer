#include "pch.h"
#include "Renderer/FrameBuffer.h"
#include "glad/glad.h"

namespace Axel
{
	FrameBuffer::FrameBuffer(const FrameBufferSpecification& spec)
		: m_Specification(spec)
	{
		for (const auto spec : m_Specification.Attachments)
			if (spec == AttachmentFormat::DEPTH)
				m_UseDepth = true;

		Resize(m_Specification.Width, m_Specification.Height);
	}

	FrameBuffer::~FrameBuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(m_ColourAttachments.size(), m_ColourAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);

	}

	std::shared_ptr<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
	{
		return std::make_shared<FrameBuffer>(spec);
	}

	void FrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(m_ColourAttachments.size(), m_ColourAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);

			m_ColourAttachments.clear();
			m_DepthAttachment = 0;
		}

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		if (m_ColourAttachments.size())
		{
			glCreateTextures(GL_TEXTURE_2D, m_ColourAttachments.size(), m_ColourAttachments.data());

			for (size_t i = 0; i < m_ColourAttachments.size(); i++)
			{
				glBindTexture(GL_TEXTURE_2D, m_ColourAttachments[i]);

				switch (m_Specification.Attachments[i])
				{
				case AttachmentFormat::RGBA:
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
					break;
				case AttachmentFormat::RED_INTEGER:
					glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, m_Specification.Width, m_Specification.Height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, nullptr);
					break;

				}

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_ColourAttachments[i], 0);
			}
		}

		if (m_UseDepth)
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
			glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);

			glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH24_STENCIL8, GL_TEXTURE_2D, m_DepthAttachment, 0);
		}

		if (m_ColourAttachments.size() > 1)
		{
			AX_ASSERT(m_ColourAttachments.size() <= 4, "You dont need that many colours.");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_ColourAttachments.size(), buffers);
		}
		else if (m_ColourAttachments.empty())
		{
			// Only depth-pass
			glDrawBuffer(GL_NONE);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	uint32_t FrameBuffer::GetAttachmentRendererID(uint32_t attachmentIndex)
	{
		AX_ASSERT(attachmentIndex < m_ColourAttachments.size(), "Attachment out of range");
		return m_ColourAttachments[attachmentIndex];
	}

	void FrameBuffer::ResetAttachment(uint32_t attachmentIndex, int value)
	{
		AX_ASSERT(attachmentIndex < m_ColourAttachments.size(), "Attachment out of range");

		GLenum attchment = 0;
		switch (m_Specification.Attachments[attachmentIndex])
		{
		case AttachmentFormat::RGBA:
			attchment = GL_RGBA8;
			break;
		case AttachmentFormat::RED_INTEGER:
			attchment = GL_RED_INTEGER;
			break;
		}

		glClearTexImage(m_ColourAttachments[attachmentIndex], 0, attchment, GL_INT, &value);
	}

	// MUST READ FROM THE "GL_RED_INTEGER" CHANNEL
	int FrameBuffer::GetPixel(uint32_t attachmentIndex, int x, int y)
	{
		AX_ASSERT(attachmentIndex < m_ColourAttachments.size(), "Attachment out of range");

		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int value;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &value);
		
		return value;
	}

	void FrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
	}

	void FrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}