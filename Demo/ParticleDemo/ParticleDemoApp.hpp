#pragma once
#include "ParticleTextureGenerator.hpp"

#include <ResourceHelper.hpp>
#include <DemoApp.hpp>

class ParticleDemoApp final : public Demo::DemoApp {
public:
	ParticleDemoApp(
		const std::string& name,
		const size_t width,
		const size_t height) :
		Demo::DemoApp(name, width, height)
	{
		initialize();
	}
private:
	void update() override;
	void render() override;
	void initialize() override;

	void initializeCommands();

	void initializeSwapChain();
	
	void initializeBuffers();

	void initializeShaders();

	void initializeSamplers();

	void initializeTextures();

	void initializePipeline();
private:
	const size_t maxFrameResources = 2;
	const size_t particleCount = 1000;

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

	std::shared_ptr<CodeRed::GpuSampler> mSampler;

	std::shared_ptr<CodeRed::GpuPipelineFactory> mPipelineFactory;
	std::shared_ptr<CodeRed::PipelineInfo> mPipelineInfo;

	std::shared_ptr<ParticleTextureGenerator> mParticleTextureGenerator;
	
	std::vector<CodeRed::Byte> mVertexShaderCode;
	std::vector<CodeRed::Byte> mPixelShaderCode;
};
