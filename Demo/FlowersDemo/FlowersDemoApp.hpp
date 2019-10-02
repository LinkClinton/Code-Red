#pragma once

#include <ShaderCompiler.hpp>
#include <FrameResources.hpp>
#include <ResourceHelper.hpp>
#include <PipelineInfo.hpp>
#include <DemoApp.hpp>

#include "FlowersGenerator.hpp"

#define __DIRECTX12__MODE__
#define __VULKAN__MODE__

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

class FlowersDemoApp final : public Demo::DemoApp {
public:
	FlowersDemoApp(
		const std::string& name,
		const size_t width,
		const size_t height);

	~FlowersDemoApp();
private:
	void update(float delta) override;
	void render(float delta) override;

	void initialize() override;

	void initializeFlowers();
	
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
	const size_t flowersCount = 500;

	size_t mCurrentFrameIndex = 0;
	
	std::shared_ptr<CodeRed::GpuLogicalDevice> mDevice;
	std::shared_ptr<CodeRed::GpuSwapChain> mSwapChain;
	
	std::shared_ptr<CodeRed::GpuCommandAllocator> mCommandAllocator;
	std::shared_ptr<CodeRed::GpuGraphicsCommandList> mCommandList;
	std::shared_ptr<CodeRed::GpuCommandQueue> mCommandQueue;

	std::vector<CodeRed::FrameResources> mFrameResources =
		std::vector<CodeRed::FrameResources>(maxFrameResources);
	
	std::shared_ptr<CodeRed::GpuBuffer> mVertexBuffer;
	std::shared_ptr<CodeRed::GpuBuffer> mIndexBuffer;
	std::shared_ptr<CodeRed::GpuBuffer> mViewBuffer;
	
	std::shared_ptr<CodeRed::GpuPipelineFactory> mPipelineFactory;
	std::shared_ptr<CodeRed::PipelineInfo> mPipelineInfo;

	std::shared_ptr<FlowersGenerator> mFlowersGenerator;
	
	std::vector<CodeRed::Byte> mVertexShaderCode;
	std::vector<CodeRed::Byte> mPixelShaderCode;
};