#pragma once

#include <Interface/GpuGraphicsCommandList.hpp>
#include <Interface/GpuResource/GpuSampler.hpp>
#include <Interface/GpuResource/GpuTexture.hpp>
#include "Interface/GpuResource/GpuBuffer.hpp"
#include <Interface/GpuGraphicsPipeline.hpp>
#include <Interface/GpuDescriptorHeap.hpp>
#include <Interface/GpuLogicalDevice.hpp>

#include <Shared/Noncopyable.hpp>

#include <ImGui/imgui.h>

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

		~ImGuiContext() = default;
		
		void setRenderState(
			const std::shared_ptr<GpuGraphicsCommandList>& ctx,
			const ImGuiFrameResources& resources,
			ImDrawData* drawData) const;

		void draw(
			const std::shared_ptr<GpuGraphicsCommandList>& ctx,
			ImDrawData* drawData);
	private:
		void initializeFontsTexture();
	private:
		std::shared_ptr<GpuLogicalDevice> mDevice;

		std::shared_ptr<GpuDescriptorHeap> mDescriptorHeap;
		std::shared_ptr<GpuResourceLayout> mResourceLayout;
		std::shared_ptr<GpuTexture> mFontsTexture;
		std::shared_ptr<GpuSampler> mSampler;

		std::shared_ptr<GpuGraphicsPipeline> mPipeline;
		std::shared_ptr<GpuRenderPass> mRenderPass;

		std::shared_ptr<Byte> mVertexShaderCode;
		std::shared_ptr<Byte> mPixelShaderCode;

		std::vector<ImGuiFrameResources> mFrameResouces;
		
		size_t mCurrentFrameIndex = 0;
	};
	
}