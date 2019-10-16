#include "EffectPassDemoApp.hpp"

#include <random>

EffectPassDemoApp::EffectPassDemoApp(
	const std::string& name,
	const size_t width,
	const size_t height) :
#ifdef __DIRECTX12__MODE__
	DemoApp(name + "[DirectX12]", width, height)
#else
#ifdef __VULKAN__MODE__
	DemoApp(name + "[Vulkan]", width, height)
#else
	DemoApp(name, width, height)
#endif
#endif	
{
	initialize();
}

EffectPassDemoApp::~EffectPassDemoApp()
{
	//if we want to destroy the demo app, device and so on
	//we need wait for command queue to idle
	mCommandQueue->waitIdle();
}

void EffectPassDemoApp::update(float delta)
{
	static std::default_random_engine random(0);
	static const std::uniform_real_distribution<float> dRange(0.2f, 0.75f);
	static const std::uniform_real_distribution<float> fRange(0.0005f, 0.1f);
	
	//user can update some resource or state in this function
	//such as, update buffer, copy texture and so on
	//the function call is before render()

	auto effectPass = mFrameResources[mCurrentFrameIndex].get<EffectPass>("EffectPass");

	for (size_t index = 0; index < mTransforms.size(); index++) {
		const auto speed = glm::pi<float>() * 0.00f;
		const auto angle = delta * speed;

		auto& transform = mTransforms[index];

		transform.Transform = glm::rotate(transform.Transform, angle, glm::vec3(0, 1, 0));
		transform.NormalTransform = glm::transpose(glm::inverse(transform.Transform));

	}
	
	effectPass->setTransforms(mTransforms);
	effectPass->setMaterials(mMaterials);

	effectPass->updateToGpu(mCommandAllocator, mCommandQueue);
}

void EffectPassDemoApp::render(float delta)
{
	//get the frame buffer we render to
	//we use a frame resource to record the resource we will use in this frame
	//with frame resource, we can try to avoid the synchronization of CPU and GPU
	const auto frameBuffer =
		mFrameResources[mCurrentFrameIndex].get<CodeRed::GpuFrameBuffer>("FrameBuffer");
	const auto effectPass =
		mFrameResources[mCurrentFrameIndex].get<EffectPass>("EffectPass");

	//wait for GPU and reset the command allocator
	mCommandQueue->waitIdle();
	mCommandAllocator->reset();

	//begin to recording commands
	mCommandList->beginRecording();

	//set view port and scissor rect
	mCommandList->setViewPort(frameBuffer->fullViewPort());
	mCommandList->setScissorRect(frameBuffer->fullScissorRect());

	//set vertex buffer and index buffer
	mCommandList->setVertexBuffer(mVertexBuffer);
	mCommandList->setIndexBuffer(mIndexBuffer);

	effectPass->beginEffect(mCommandList);
	
	//begin render pass and set frame buffer
	mCommandList->beginRenderPass(
		mRenderPass,
		frameBuffer);

	//add some draw commands
	//the draw commands must between the render pass
#ifdef __TEXTURE__MATERIAL__MODE__
	effectPass->drawIndexedWithTextureMaterial(mIndexBuffer->count(), sphereCount);
#else
	effectPass->drawIndexed(mIndexBuffer->count(), sphereCount);
#endif

	mCommandList->endRenderPass();

	effectPass->endEffect();
	
	mCommandList->endRecording();

	//execute the commands recording by command list
	mCommandQueue->execute({ mCommandList });

	mSwapChain->present();

	mCurrentFrameIndex = (mCurrentFrameIndex + 1) % maxFrameResources;
}

void EffectPassDemoApp::initialize()
{
#ifdef __DIRECTX12__MODE__
	const auto systemInfo = std::make_shared<CodeRed::DirectX12SystemInfo>();
	const auto adapters = systemInfo->selectDisplayAdapter();

	mDevice = std::static_pointer_cast<CodeRed::GpuLogicalDevice>(
		std::make_shared<CodeRed::DirectX12LogicalDevice>(adapters[0])
		);
#else
#ifdef __VULKAN__MODE__
	const auto systemInfo = std::make_shared<CodeRed::VulkanSystemInfo>();
	const auto adapters = systemInfo->selectDisplayAdapter();

	mDevice = std::static_pointer_cast<CodeRed::GpuLogicalDevice>(
		std::make_shared<CodeRed::VulkanLogicalDevice>(adapters[0])
		);
#endif
#endif

	initializeSpheres();
	initializeCommands();
	initializeSwapChain();
	initializeBuffers();
	initializeShaders();
	initializeSamplers();
	initializeTextures();
	initializePipeline();
	initializeDescriptorHeaps();
}

void EffectPassDemoApp::initializeSpheres()
{
	const auto projection = glm::perspectiveFovLH(
		glm::pi<float>() * 0.25f,
		static_cast<float>(width()),
		static_cast<float>(height()),
		1.0f, 1000.0f);

#ifdef __TEXTURE__MATERIAL__MODE__
	const auto eyePosition = glm::vec4(0, 0, -20, 0);
#else
	const auto eyePosition = glm::vec4(0, 0, -100, 0);
#endif
	
	const auto view = glm::lookAtLH(
		glm::vec3(eyePosition),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0));

	const auto limitRadius = 5.0f;

	std::default_random_engine random(0);
	const std::uniform_real_distribution<float> dRange(0.2f, 0.75f);
	const std::uniform_real_distribution<float> fRange(0.0005f, 0.1f);
	const std::uniform_real_distribution<float> mRange(0.2f, 0.7f);
	const std::uniform_real_distribution<float> rRange(0.05f, 0.3f);
	const std::uniform_real_distribution<float> aRange(0.5f, 1.0f);

	const float distance = 2.0f;

	const auto startX = 0.0f - 0.5f * (columnCount * limitRadius * 2 + (columnCount - 1) * distance) + limitRadius;
	const auto startY = 0.0f - 0.5f * (rowCount * limitRadius * 2 + (rowCount - 1) * distance) + limitRadius;
	
	for (size_t index = 0; index < sphereCount; index++) {
		auto& transform = mTransforms[index];
		auto& material = mMaterials[index];
		auto& sphere = mSpheres[index];

		const auto positionX = index % columnCount;
		const auto positionY = index / columnCount;
		
		sphere.Position = glm::vec3(
			startX + positionX * (limitRadius * 2 + distance),
			startY + positionY * (limitRadius * 2 + distance),
			0.0f
		);
	
		sphere.Radius = glm::vec1(limitRadius);

		transform.EyePosition = eyePosition;
		transform.Projection = projection;
		transform.View = view;

		transform.Transform = glm::translate(transform.Transform, sphere.Position);
		transform.Transform = glm::scale(transform.Transform, glm::vec3(sphere.Radius));
		
		transform.NormalTransform = glm::transpose(glm::inverse(transform.Transform));

#ifdef __PBR__MODE__
		material.DiffuseAlbedo = glm::vec4(0.5f, 0.0f, 0.0f, 1.0f);
		material.Metallic = glm::vec1(positionY * (1.0f / (rowCount - 1)));
		material.Roughness = glm::vec1(0.1f + positionX * (0.9f / (columnCount - 1)));
		material.AmbientOcclusion = glm::vec1(1.0f);
#else
		material.DiffuseAlbedo = glm::vec4(0.5f, 0.0f, 0.0f, 1.0f);
		material.FreshelR0 = glm::vec3(positionY * (1.0f / (rowCount - 1)));
		material.Roughness = glm::vec1(0.1f + positionX * (0.9f / (columnCount - 1)));
#endif	
	}
}

void EffectPassDemoApp::initializeCommands()
{
	//create command allocator, queue and list
	//command allocator is a memory pool to allocator memory of command list
	//the commands recoding by command list are store in the command allocator
	//the command queue is a queue that store the commands are submitted to GPU
	mCommandAllocator = mDevice->createCommandAllocator();
	mCommandQueue = mDevice->createCommandQueue();
	mCommandList = mDevice->createGraphicsCommandList(mCommandAllocator);
}

void EffectPassDemoApp::initializeSwapChain()
{
	//create the swap chain
	//if we want to write the back buffer(to window)
	//we need use the queue that create the swap chain to submit the draw commands
	mSwapChain = mDevice->createSwapChain(
		mCommandQueue,
		{ width(), height(), handle() },
		CodeRed::PixelFormat::BlueGreenRedAlpha8BitUnknown,
		maxFrameResources
	);

	mDepthBuffer = mDevice->createTexture(
		CodeRed::ResourceInfo::Texture2D(
			mSwapChain->width(), mSwapChain->height(),
			CodeRed::PixelFormat::Depth32BitFloat,
			CodeRed::ResourceUsage::DepthStencil
		)
	);
	
	for (size_t index = 0; index < maxFrameResources; index++) {
		mFrameResources[index].set(
			"FrameBuffer",
			mDevice->createFrameBuffer(
				mSwapChain->buffer(index),
				mDepthBuffer
			)
		);
	}
}

void EffectPassDemoApp::initializeBuffers()
{
	//we initialize buffers in this
	//such as vertex buffer, index buffer, constant buffer
	//the buffers in the frame resource are created in this, too.

	const float radius = 1.0f;
	const size_t sliceCount = 64;
	const size_t stackCount = 64;

	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 Texcoord;
		glm::vec3 Tangent;
	};

	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;

	const Vertex topVertex = {
		{0.0f, +radius, 0.0f},
		{0.0f, +1.0f, 0.0f},
		{0.0f,0.0f} };
	const Vertex bottomVertex = {
		{0.0f, -radius, 0.0f},
		{0.0f, -1.0f, 0.0f},
		{0.0f,1.0f} };

	vertices.push_back(topVertex);

	const auto phiStep = glm::pi<float>() / stackCount;
	const auto thetaStep = glm::two_pi<float>() / sliceCount;

	for (size_t index0 = 1; index0 < stackCount; index0++) {
		const auto phi = index0 * phiStep;

		for (size_t index1 = 0; index1 <= sliceCount; index1++) {
			const auto theta = index1 * thetaStep;

			Vertex vertex;

			vertex.Position.x = radius * glm::sin(phi) * glm::cos(theta);
			vertex.Position.y = radius * glm::cos(phi);
			vertex.Position.z = radius * glm::sin(phi) * glm::sin(theta);

			vertex.Tangent.x = -radius * glm::sin(phi) * glm::sin(theta);
			vertex.Tangent.y = 0.0f;
			vertex.Tangent.z = +radius * glm::sin(phi) * glm::cos(theta);

			vertex.Tangent = glm::normalize(vertex.Tangent);
			
			vertex.Normal = glm::normalize(vertex.Position);

			vertex.Texcoord.x = theta / glm::two_pi<float>() * 4;
			vertex.Texcoord.y = phi / glm::pi<float>() * 2;
			
			vertices.push_back(vertex);
		}
	}

	vertices.push_back(bottomVertex);

	for (size_t index = 1; index <= sliceCount; index++) {
		indices.push_back(0);
		indices.push_back(static_cast<unsigned>(index + 1));
		indices.push_back(static_cast<unsigned>(index));
	}

	size_t baseIndex = 1;
	size_t ringVertexCount = sliceCount + 1;

	for (size_t index0 = 0; index0 < stackCount - 2; index0++) {
		for (size_t index1 = 0; index1 < sliceCount; index1++) {
			indices.push_back(static_cast<unsigned>(baseIndex + index0 * ringVertexCount + index1));
			indices.push_back(static_cast<unsigned>(baseIndex + index0 * ringVertexCount + index1 + 1));
			indices.push_back(static_cast<unsigned>(baseIndex + (index0 + 1) * ringVertexCount + index1));

			indices.push_back(static_cast<unsigned>(baseIndex + (index0 + 1) * ringVertexCount + index1));
			indices.push_back(static_cast<unsigned>(baseIndex + index0 * ringVertexCount + index1 + 1));
			indices.push_back(static_cast<unsigned>(baseIndex + (index0 + 1) * ringVertexCount + index1 + 1));
		}
	}

	size_t southPoleIndex = vertices.size() - 1;

	baseIndex = southPoleIndex - ringVertexCount;

	for (size_t index = 0; index < sliceCount; index++) {
		indices.push_back(static_cast<unsigned>(southPoleIndex));
		indices.push_back(static_cast<unsigned>(baseIndex + index));
		indices.push_back(static_cast<unsigned>(baseIndex + index + 1));
	}

	mVertexBuffer = mDevice->createBuffer(
		CodeRed::ResourceInfo::VertexBuffer(
			sizeof(Vertex),
			vertices.size()
		)
	);

	mIndexBuffer = mDevice->createBuffer(
		CodeRed::ResourceInfo::IndexBuffer(
			sizeof(unsigned),
			indices.size()
		)
	);

	CodeRed::ResourceHelper::updateBuffer(mDevice, mCommandAllocator, mCommandQueue, mVertexBuffer, vertices.data());
	CodeRed::ResourceHelper::updateBuffer(mDevice, mCommandAllocator, mCommandQueue, mIndexBuffer, indices.data());
}

void EffectPassDemoApp::initializeShaders()
{
	//we initialize shaders in this
	//we will read shader from file and compile them
	//there are some help function in class CodeRed::ShaderCompiler in DemoApp.
#ifdef __DIRECTX12__MODE__
#else
#ifdef __VULKAN__MODE__
#endif
#endif
}

void EffectPassDemoApp::initializeSamplers()
{
	//we initialize samplers in this
}

void EffectPassDemoApp::initializeTextures()
{
	//we initialize textures in this
}

void EffectPassDemoApp::initializePipeline()
{
	//we initialize the graphics pipeline in this
	//the pipeline info is a help class that we can initialize pipeline easier
	//you can use setXXX to set the graphics pipeline state
	//but you should use "updateState()" to create graphics pipeline
	mRenderPass = mDevice->createRenderPass(
		CodeRed::Attachment::RenderTarget(mSwapChain->format()),
		CodeRed::Attachment::DepthStencil(mDepthBuffer->format())
	);

	mRenderPass->setClear(CodeRed::ClearValue(0.27f, 0.27f, 0.27f, 1.0f));

	auto pipelineFactory = mDevice->createPipelineFactory();
	
	for (auto& frameResource : mFrameResources) {
		frameResource.set("EffectPass",
			std::make_shared<EffectPass>(
				mDevice,
				mRenderPass,
				sphereCount)
		);

		auto effectPass = frameResource.get<EffectPass>("EffectPass");

#ifdef __PBR__MODE__
		const auto lightFactor = 2.0f;
#else
		const auto lightFactor = 2.0f;
#endif

		effectPass->setLight(CodeRed::LightType::Point, 0,
			CodeRed::Light::PointLight(
				glm::vec3(0.5f * lightFactor),
				glm::vec3(0, 0, -150),
				glm::vec1(200),
				glm::vec1(300)
			));

		effectPass->setAmbientLight(glm::vec4(0.2f));

#ifdef __TEXTURE__MATERIAL__MODE__
		effectPass->setTextureMaterial(getTextureMaterial("wornpaintedcement"));
#endif
		
		effectPass->updateToGpu(mCommandAllocator, mCommandQueue);
	}
}

void EffectPassDemoApp::initializeDescriptorHeaps()
{
	//we initialize descriptor heap in this
	
}

auto EffectPassDemoApp::getTextureMaterial(const std::string& name) -> TextureMaterial
{
	if (mTextureMaterials.find(name) != mTextureMaterials.end()) return mTextureMaterials[name];

	TextureMaterial material;

	material.DiffuseAlbedo = CodeRed::ResourceHelper::loadTexture(
		mDevice, mCommandAllocator, mCommandQueue, "./Resources/" + name + "/albedo.png"
	);

	material.Metallic = CodeRed::ResourceHelper::loadTexture(
		mDevice, mCommandAllocator, mCommandQueue, "./Resources/" + name + "/metalness.png"
	);

	material.Normal = CodeRed::ResourceHelper::loadTexture(
		mDevice, mCommandAllocator, mCommandQueue, "./Resources/" + name + "/normal.png");

	material.Roughness = CodeRed::ResourceHelper::loadTexture(
		mDevice, mCommandAllocator, mCommandQueue, "./Resources/" + name + "/roughness.png"
	);

	material.AmbientOcclusion = CodeRed::ResourceHelper::loadTexture(
		mDevice, mCommandAllocator, mCommandQueue, "./Resources/" + name + "/ao.png"
	);

	mTextureMaterials.insert({ name, material });

	return material;
}

