#pragma once
#include "ParticleTextureGenerator.hpp"

#include <ResourceHelper.hpp>
#include <DemoApp.hpp>

struct Particle {
	glm::vec2 Position = glm::vec2(0);
	glm::vec2 Forward = glm::vec2(0);
	glm::vec2 Size = glm::vec2(1);

	Particle() = default;

	Particle(
		const glm::vec2 &position,
		const glm::vec2 &forward,
		const glm::vec2 &size) :
		Position(position), Forward(forward), Size(size) {}

	void reverseIfOut(
		glm::vec2& offset,
		const size_t width,
		const size_t height)
	{
		const auto target = Position + offset;
		
		if (target.x - Size.x < 0 || target.x + Size.x > width) {
			offset.x = -offset.x;
			Forward.x = -Forward.x;
		}
		
		if (target.y - Size.y < 0 || target.y + Size.y > height) {
			offset.y = -offset.y;
			Forward.y = -Forward.y;
		}

		Position = Position + offset;
	}
};

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

	~ParticleDemoApp();
private:
	void update(float delta) override;
	void render(float delta) override;
	void initialize() override;
	
	void initializeParticles();
	
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
	const size_t maxParticleSize = 20;
	const size_t particleCount = 2000;
	
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

	std::shared_ptr<CodeRed::GpuSampler> mSampler;

	std::shared_ptr<CodeRed::GpuPipelineFactory> mPipelineFactory;
	std::shared_ptr<CodeRed::PipelineInfo> mPipelineInfo;

	std::shared_ptr<ParticleTextureGenerator> mParticleTextureGenerator;
	
	std::vector<CodeRed::Byte> mVertexShaderCode;
	std::vector<CodeRed::Byte> mPixelShaderCode;

	std::vector<Particle> mParticles = std::vector<Particle>(particleCount);
	std::vector<glm::mat4x4> mTransform = std::vector<glm::mat4x4>(particleCount);
};
