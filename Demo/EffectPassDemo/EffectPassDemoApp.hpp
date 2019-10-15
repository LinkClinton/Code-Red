#pragma once

#include <Effects/PhysicallyBasedEffectPass.hpp>
#include <Effects/GeneralEffectPass.hpp>
#include <Resources/FrameResources.hpp>
#include <Resources/ResourceHelper.hpp>
#include <Pipelines/PipelineInfo.hpp>
#include <Shaders/ShaderCompiler.hpp>

#include <DemoApp.hpp>

#define __DIRECTX12__MODE__
#define __VULKAN__MODE__
#define __PBR__MODE__

struct Sphere {
	glm::vec3 Position = glm::vec3(0);
	glm::vec3 Forward = glm::vec3(0, 0, 1);
	glm::vec1 Radius = glm::vec1(1);

	Sphere() = default;
	
	auto update(float length, const glm::vec3& limitBound) -> glm::vec3;

	static void reverse(float& forward, float position, float limit);

	static bool intersect(const Sphere& sphere0, const Sphere& sphere1);
};

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

	void initializeSpheres();
	
	void initializeCommands();

	void initializeSwapChain();

	void initializeBuffers();

	void initializeShaders();

	void initializeSamplers();

	void initializeTextures();

	void initializePipeline();

	void initializeDescriptorHeaps();
private:
#ifdef __PBR__MODE__
	using EffectPass = CodeRed::PhysicallyBasedEffectPass;
	using Material = CodeRed::PhysicallyBasedMaterial;
#else
	using EffectPass = CodeRed::GeneralEffectPass;
	using Material = CodeRed::Material;
#endif
	const size_t maxFrameResources = 2;
	const size_t rowCount = 6;
	const size_t columnCount = 10;
	const size_t sphereCount = rowCount * columnCount;

	const glm::vec3 limitBound = glm::vec3(110, 60, 30);
	
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

	std::vector<Material> mMaterials = std::vector<Material>(sphereCount);
	std::vector<Sphere> mSpheres = std::vector<Sphere>(sphereCount);
};