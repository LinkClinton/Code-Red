#pragma once

#include <ShaderCompiler.hpp>
#include <FrameResources.hpp>
#include <PipelineInfo.hpp>

//#define __DIRECTX12__MODE__
#define __VULKAN__MODE

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

struct ParticleVertex {
	glm::vec2 Position = glm::vec2(0);
	glm::vec2 TexCoord = glm::vec2(0);
	
	ParticleVertex() = default;

	explicit ParticleVertex(
		const glm::vec2 &position,
		const glm::vec2 &texcoord) :
		Position(position), TexCoord(texcoord) {}
};

class ParticleTextureGenerator final {
public:
	explicit ParticleTextureGenerator(
		const std::shared_ptr<CodeRed::GpuLogicalDevice>& device,
		const std::shared_ptr<CodeRed::GpuCommandAllocator>& allocator,
		const size_t detailLevel,
		const size_t detailSize);

	void run() const;

	void waitFinish() const;

	auto texture() const noexcept -> std::shared_ptr<CodeRed::GpuTexture> { return mTexture; }
private:
	void initializeCommands();

	void initializeBuffers();

	void initializeShaders();

	void initializeTextures();

	void initializePipeline();
private:
	size_t mDetailLevel = 0;
	size_t mDetailSize = 0;

	std::shared_ptr<CodeRed::GpuLogicalDevice> mDevice;
	std::shared_ptr<CodeRed::GpuCommandAllocator> mCommandAllocator;

	std::shared_ptr<CodeRed::GpuGraphicsCommandList> mCommandList;
	std::shared_ptr<CodeRed::GpuCommandQueue> mCommandQueue;

	std::shared_ptr<CodeRed::GpuPipelineFactory> mPipelineFactory;
	std::shared_ptr<CodeRed::PipelineInfo> mPipelineInfo;

	std::shared_ptr<CodeRed::GpuBuffer> mVertexBuffer;
	std::shared_ptr<CodeRed::GpuBuffer> mIndexBuffer;
	std::shared_ptr<CodeRed::GpuBuffer> mViewBuffer;

	std::shared_ptr<CodeRed::GpuFrameBuffer> mFrameBuffer;
	
	std::shared_ptr<CodeRed::GpuTexture> mTexture;

	std::vector<CodeRed::Byte> mVertexShaderCode;
	std::vector<CodeRed::Byte> mPixelShaderCode;
};