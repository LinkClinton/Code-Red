#pragma once

#include <Shaders/ShaderCompiler.hpp>
#include <Resources/FrameResources.hpp>
#include <Resources/ResourceHelper.hpp>
#include <Pipelines/PipelineInfo.hpp>
#include <Effects/EffectPass.hpp>
#include <DemoApp.hpp>

#define __DIRECTX12__MODE__
#define __VULKAN__MODE__

class EffectPassDemoApp final : public Demo::DemoApp {
public:
	EffectPassDemoApp(
		const std::string& name,
		const size_t width,
		const size_t height);

	~EffectPassDemoApp();
private:
	void update(float delta) override;
	void render(float delta) override;

	void initialize();

	void initializeCommands();

	void initializeSwapChain();

	void initializeBuffers();

	void initializeShaders();

	void initializeSamplers();

	void initializeTextures();

	void initializePipeline();

	void initializeDescriptorHeaps();
private:
	const size_t maxFrameResources = 2;
	const size_t sphereCount = 1;

	size_t mCurrentFrameIndex = 0;

	std::shared_ptr<CodeRed::GpuLogicalDevice> mDevice;
	std::shared_ptr<CodeRed::GpuSwapChain> mSwapChain;

	std::shared_ptr<CodeRed::GpuCommandAllocator> mCommandAllocator;
	std::shared_ptr<CodeRed::GpuGraphicsCommandList> mCommandList;
	std::shared_ptr<CodeRed::GpuCommandQueue> mCommandQueue;

	std::vector<CodeRed::FrameResources> mFrameResources =
		std::vector<CodeRed::FrameResources>(maxFrameResources);

	std::shared_ptr<CodeRed::GpuTexture> mDepthBuffer;
	
	std::shared_ptr<CodeRed::GpuBuffer> mVertexBuffer;
	std::shared_ptr<CodeRed::GpuBuffer> mIndexBuffer;

	std::shared_ptr<CodeRed::GpuRenderPass> mRenderPass;

	std::vector<CodeRed::Transform3D> mTransforms = std::vector<CodeRed::Transform3D>(sphereCount);
};