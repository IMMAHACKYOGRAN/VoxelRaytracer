#pragma once
#include <memory>

namespace Axel
{
	struct FrameBufferSpecification
	{
		uint32_t Width, Height;

		bool SwapChainTarget = false;
	};


	class FrameBuffer
	{
	public:
		FrameBuffer(const FrameBufferSpecification& spec);
		~FrameBuffer();

		static std::shared_ptr<FrameBuffer> Create(const FrameBufferSpecification& spec);

		void Resize();

		uint32_t GetColourAttachment() const { return m_ColourAttachment; }
		FrameBufferSpecification GetSpecification() const { return m_Specification; }

		void Bind();
		void Unbind();

	private:
		uint32_t m_RendererID;
		uint32_t m_ColourAttachment, m_DepthAttachment;
		FrameBufferSpecification m_Specification;
	};
}