#pragma once

#include <CodeRed/Interface/GpuGraphicsCommandList.hpp>
#include <CodeRed/Interface/GpuResource/GpuSampler.hpp>
#include <CodeRed/Interface/GpuResource/GpuTexture.hpp>
#include "CodeRed/Interface/GpuResource/GpuBuffer.hpp"
#include <CodeRed/Interface/GpuGraphicsPipeline.hpp>
#include <CodeRed/Interface/GpuDescriptorHeap.hpp>
#include <CodeRed/Interface/GpuLogicalDevice.hpp>

#include <CodeRed/Shared/Noncopyable.hpp>

#include <ThirdParties/ImGui/imgui.h>

#include <unordered_map>
#include <memory>

namespace CodeRed {

	class ImGuiFrameResources {
	public:
		explicit ImGuiFrameResources(
			const std::shared_ptr<GpuLogicalDevice>& device);

		~ImGuiFrameResources() = default;
		
		void update(ImDrawData* drawData);
		
		auto vertex() const -> std::shared_ptr<GpuBuffer> { return mVertexBuffer; }

		auto index() const -> std::shared_ptr<GpuBuffer> { return mIndexBuffer; }
	private:
		const size_t mVertexBufferIncrease = 500;
		const size_t mIndexBufferIncrease = 5000;

		std::shared_ptr<GpuLogicalDevice> mDevice;
		
		std::shared_ptr<GpuBuffer> mVertexBuffer;
		std::shared_ptr<GpuBuffer> mIndexBuffer;
	};
	
	class ImGuiContext final : public Noncopyable {
	public:
		explicit ImGuiContext(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuRenderPass>& renderPass,
			const size_t numFrameResources = 2);

		explicit ImGuiContext(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuRenderPass>& renderPass,
			const std::shared_ptr<GpuCommandAllocator>& allocator,
			const std::shared_ptr<GpuCommandQueue>& queue,
			const size_t numFrameResources = 2);
		
		~ImGuiContext() = default;
		
		void setRenderState(
			const std::shared_ptr<GpuGraphicsCommandList>& ctx,
			const ImGuiFrameResources& resources,
			ImDrawData* drawData) const;

		void draw(
			const std::shared_ptr<GpuGraphicsCommandList>& ctx,
			ImDrawData* drawData);
	private:
		void initializeFontsTexture(
			const std::shared_ptr<GpuCommandAllocator>& allocator,
			const std::shared_ptr<GpuCommandQueue>& queue);
	private:
		std::shared_ptr<GpuLogicalDevice> mDevice;

		std::unordered_map<
			std::shared_ptr<GpuTexture>,
			std::shared_ptr<GpuDescriptorHeap>> mCurrentDescriptorHeaps;

		std::vector<std::shared_ptr<GpuDescriptorHeap>> mDescriptorHeapPool;
		
		std::shared_ptr<GpuResourceLayout> mResourceLayout;
		std::shared_ptr<GpuTexture> mFontsTexture;
		std::shared_ptr<GpuSampler> mSampler;

		std::shared_ptr<GpuGraphicsPipeline> mPipeline;
		std::shared_ptr<GpuRenderPass> mRenderPass;

		std::vector<Byte> mVertexShaderCode;
		std::vector<Byte> mPixelShaderCode;
		
		std::vector<ImGuiFrameResources> mFrameResources;
		
		size_t mCurrentFrameIndex = 0;
	};
	
}