#pragma once
#include <memory>

namespace Axel
{
	enum class AttachmentFormat
	{
		None = 0,
		RGBA,
		RED_INTEGER,
		DEPTH,
	};

	struct FrameBufferSpecification
	{
		uint32_t Width, Height;
		std::vector<AttachmentFormat> Attachments;
		bool SwapChainTarget = false;
	};

	class FrameBuffer
	{
	public:
		FrameBuffer(const FrameBufferSpecification& spec);
		~FrameBuffer();

		static std::shared_ptr<FrameBuffer> Create(const FrameBufferSpecification& spec);

		void Resize(uint32_t width, uint32_t height);
		int GetPixel(uint32_t attachmentIndex, int x, int y);

		void ResetAttachment(uint32_t attachmentIndex, int value);

		uint32_t GetAttachmentRendererID(uint32_t attachmentIndex = 0);
		FrameBufferSpecification GetSpecification() const { return m_Specification; }

		void Bind();
		void Unbind();

	private:
		uint32_t m_RendererID; 
		FrameBufferSpecification m_Specification;
		bool m_UseDepth = false;

		std::vector<uint32_t> m_ColourAttachments;
		uint32_t m_DepthAttachment;
	};
}