#include <ShaderCompiler.hpp>
#include <PipelineInfo.hpp>
#include <DemoApp.hpp>

//#define __DIRECTX12__MODE__
#define __VULKAN__MODE__

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

class TriangleDemo final : public Demo::DemoApp {
public:
	TriangleDemo(
		const std::string &name, 
		const size_t width, 
		const size_t height) :
#ifdef __DIRECTX12__MODE__
		Demo::DemoApp(name + "[DirectX12]", width, height)
#else
#ifdef __VULKAN__MODE__
		Demo::DemoApp(name + "[Vulkan]", width, height)
#else
		Demo::DemoApp(name, width, height)
#endif
#endif
	{
		initialize();
	}
private:
	void update(float delta) override {}
	void render(float delta) override
	{
		//get current back buffer we can render
		const auto index = mSwapChain->currentBufferIndex();
		
		//reset the command allocator and command list
		//we can reuse the memory that we used in the previous frame
		mCommandAllocator->reset();

		//begin recoding with command list
		mCommandList->beginRecoding();

		//set graphics pipeline and resource layout
		mCommandList->setGraphicsPipeline(mPipelineInfo->graphicsPipeline());
		mCommandList->setResourceLayout(mPipelineInfo->graphicsPipeline()->layout());

		//set frame buffer, viewport and scissor rect
		mCommandList->setViewPort(mFrameBuffers[index]->fullViewPort());
		mCommandList->setScissorRect(mFrameBuffers[index]->fullScissorRect());

		//set vertex buffer and view buffer
		mCommandList->setVertexBuffer(mVertexBuffer);

		mCommandList->setDescriptorHeap(mDescriptorHeap);
		
		mPipelineInfo->graphicsPipeline()->renderPass()->setClear(
			CodeRed::ClearValue(1.0f, 1.0f, 1.0f, 1.0f)
		);
		
		mCommandList->beginRenderPass(mPipelineInfo->graphicsPipeline()->renderPass(),
			mFrameBuffers[index]);
		
		mCommandList->draw(3);

		mCommandList->endRenderPass();
		
		//we finish the draw commands, so we need end recoding
		mCommandList->endRecoding();

		//submit the command list to GPU using queue
		//NOTICE: if you want to write to back buffer of swap chain,
		//NOTICE: you must use the queue that create the swap chain to submit command list
		mCommandQueue->execute({ mCommandList });

		//present current back buffer
		mSwapChain->present();

		//wait the commands finish
		mCommandQueue->waitIdle();

	}
	void initialize() override
	{
		
#ifdef __DIRECTX12__MODE__
		//find all adapters we can use
		const auto systemInfo = std::make_shared<CodeRed::DirectX12SystemInfo>();
		const auto adapters = systemInfo->selectDisplayAdapter();

		//create device with adapter we choose
		//for this demo, we choose the first adapter
		
		mDevice = std::static_pointer_cast<CodeRed::GpuLogicalDevice>(
			std::make_shared<CodeRed::DirectX12LogicalDevice>(adapters[0])
			);
#else
#ifdef 		__VULKAN__MODE__
		//find all adapters we can use
		const auto systemInfo = std::make_shared<CodeRed::VulkanSystemInfo>();
		const auto adapters = systemInfo->selectDisplayAdapter();

		//create device with adapter we choose
		//for this demo, we choose the first adapter

		mDevice = std::static_pointer_cast<CodeRed::GpuLogicalDevice>(
			std::make_shared<CodeRed::VulkanLogicalDevice>(adapters[0])
			);
#endif
#endif
		
		//create the command allocator, queue and list
		//command allocator is memory pool for command list
		//command queue is a queue to submit the command list
		//command list is a recoder of draw, copy and so on
		mCommandAllocator = mDevice->createCommandAllocator();
		mCommandQueue = mDevice->createCommandQueue();
		mCommandList = mDevice->createGraphicsCommandList(mCommandAllocator);

		//create the swap chain
		//if we want to write the back buffer(to window)
		//we need use the queue that create the swap chain to submit the draw commands
		mSwapChain = mDevice->createSwapChain(
			mCommandQueue,
			{ width(), height(), handle() },
			CodeRed::PixelFormat::RedGreenBlueAlpha8BitUnknown
		);

		//create frame buffers, we can ignore the render target and depth stencil
		for (size_t index = 0; index < mSwapChain->bufferCount(); index++) {

			const auto frameBuffer = mDevice->createFrameBuffer(
				mSwapChain->buffer(index),
				nullptr);

			mFrameBuffers.push_back(frameBuffer);
		}
	
		//pipeline factory is used to create pipeline state
		mPipelineFactory = mDevice->createPipelineFactory();
		
		//we use pipeline info to help us to create graphics pipeline
		//the pipeline info is a helper class in demo app
		mPipelineInfo = std::make_shared<CodeRed::PipelineInfo>(mDevice);

		initializePipeline();
		initializeShader();
		initializeBuffer();

		mPipelineInfo->updateState();

		mDescriptorHeap = mDevice->createDescriptorHeap(
			mPipelineInfo->graphicsPipeline()->layout()
		);

		mDescriptorHeap->bindBuffer(0, mViewBuffer);
	}

	void initializeBuffer() {
		//create the vertex buffer of triangle
		//the upload memory hepa means we can map the memory directly
		//so we do not need a upload buffer to copy the vertices data to buffer
		//but the performance of gpu read is not the best
		mVertexBuffer = mDevice->createBuffer(
			CodeRed::ResourceInfo::VertexBuffer(
				sizeof(glm::vec3),
				3,
				CodeRed::MemoryHeap::Upload
			)
		);

		//create the constant buffer for matrix transform
		//default memory heap of constant buffer is upload
		mViewBuffer = mDevice->createBuffer(
			CodeRed::ResourceInfo::ConstantBuffer(
				sizeof(glm::mat4x4)
			)
		);
		
		glm::vec3 triangleVertices[] = {
			glm::vec3(0.5f, 0.25f, 0.0f),
			glm::vec3(0.70f, 0.5f, 0.0f),
			glm::vec3(0.30f,0.5f,0.0f)
		};

		for (auto& vertex : triangleVertices) 
			vertex = vertex * glm::vec3(width(), height(), 1.0f);

		//project matrix we will use
		auto viewMatrix = glm::orthoLH_ZO(0.0f,
			static_cast<float>(width()),
			static_cast<float>(height()), 0.0f, 0.0f, 1.0f);

		//we map buffer memory and copy the data
		const auto triangleMemory = mVertexBuffer->mapMemory();
		const auto viewMemory = mViewBuffer->mapMemory();

		std::memcpy(triangleMemory, triangleVertices, sizeof(triangleVertices));
		std::memcpy(viewMemory, &viewMatrix, sizeof(viewMatrix));

		//we unmap memory
		mVertexBuffer->unmapMemory();
		mViewBuffer->unmapMemory();
	}
	
	void initializeShader() const {
		
#ifdef __DIRECTX12__MODE__
		//in DirectX 12 mode, we use HLSL and compile them with D3DCompile
		static const auto vertShaderText =
			"#pragma pack_matrix(row_major)\n"
			"cbuffer transform : register(b0)\n"
			"{\n"
			"    float4x4 project;\n"
			"}\n"
			"float4 main(float3 position : POSITION) : SV_POSITION\n"
			"{\n"
			"    return mul(float4(position, 1.0f), project);\n"
			"}\n";

		static const auto pixelShaderText =
			"float4 main(float4 position : SV_POSITION) : SV_TARGET\n"
			"{\n"
			"    return float4(1.0, 0, 0, 1);\n"
			"}\n";

		WRL::ComPtr<ID3DBlob> error;
		WRL::ComPtr<ID3DBlob> vertex;
		WRL::ComPtr<ID3DBlob> pixel;

		CODE_RED_THROW_IF_FAILED(
			D3DCompile(vertShaderText,
				std::strlen(vertShaderText),
				nullptr, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
				"main", "vs_5_0", D3DCOMPILE_DEBUG, 0,
				vertex.GetAddressOf(), error.GetAddressOf()),
			CodeRed::FailedException({"Vertex Shader of HLSL"}, CodeRed::DebugType::Create)
		);

		CODE_RED_THROW_IF_FAILED(
			D3DCompile(pixelShaderText,
				std::strlen(pixelShaderText),
				nullptr, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
				"main", "ps_5_0", D3DCOMPILE_DEBUG, 0,
				pixel.GetAddressOf(), error.GetAddressOf()),
			CodeRed::FailedException({ "Pixel Shader of HLSL" }, CodeRed::DebugType::Create)
		);

		std::vector<CodeRed::Byte> vsCode(vertex->GetBufferSize());
		std::vector<CodeRed::Byte> psCode(pixel->GetBufferSize());

		std::memcpy(vsCode.data(), vertex->GetBufferPointer(), vertex->GetBufferSize());
		std::memcpy(psCode.data(), pixel->GetBufferPointer(), pixel->GetBufferSize());
#else
#ifdef __VULKAN__MODE__
		static const auto vertShaderText =
			"#version 450\n"
			"#extension GL_ARB_separate_shader_objects : enable\n"
			"layout (set = 0, binding = 0) uniform bufferVals {\n"
			"    mat4 project;\n"
			"} myBufferVals;\n"
			"layout (location = 0) in vec4 pos;\n"
			"void main() {\n"
			"   gl_Position = pos * transpose(myBufferVals.project);\n"
			"	gl_Position.y = - gl_Position.y;\n"
			"}\n";

		static const auto pixelShaderText =
			"#version 450\n"
			"#extension GL_ARB_separate_shader_objects : enable\n"
			"layout (location = 0) out vec4 outColor;\n"
			"void main() {\n"
			"   outColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
			"}\n";

		const auto vertex = CodeRed::ShaderCompiler::compileToSpv(CodeRed::ShaderType::Vertex, vertShaderText);
		const auto pixel = CodeRed::ShaderCompiler::compileToSpv(CodeRed::ShaderType::Pixel, pixelShaderText);

		std::vector<CodeRed::Byte> vsCode((vertex.end() - vertex.begin()) * sizeof(uint32_t));
		std::vector<CodeRed::Byte> psCode((pixel.end() - pixel.begin()) * sizeof(uint32_t));

		std::memcpy(vsCode.data(), &vertex.begin()[0], vsCode.size());
		std::memcpy(psCode.data(), &pixel.begin()[0], psCode.size());
#endif
#endif
		mPipelineInfo->setVertexShaderState(
			mPipelineFactory->createShaderState(
				CodeRed::ShaderType::Vertex,
				vsCode
			)
		);

		mPipelineInfo->setPixelShaderState(
			mPipelineFactory->createShaderState(
				CodeRed::ShaderType::Pixel,
				psCode
			)
		);
	}

	void initializePipeline() const {
		//set input assembly state, we only use the position with vec3
		mPipelineInfo->setInputAssemblyState(
			mPipelineFactory->createInputAssemblyState(
				{ CodeRed::InputLayoutElement("POSITION", CodeRed::PixelFormat::RedGreenBlue32BitFloat) },
				CodeRed::PrimitiveTopology::TriangleList
			)
		);

		//set depth stencil state, we disable the depth test
		mPipelineInfo->setDepthStencilState(
			mPipelineFactory->createDetphStencilState(
				false
			)
		);

		mPipelineInfo->setRasterizationState(
			mPipelineFactory->createRasterizationState()
		);

		mPipelineInfo->setBlendState(
			mPipelineFactory->createBlendState()
		);

		//set resource layout, we only set a buffer to (Binding = 0, Space = 0)
		mPipelineInfo->setResourceLayout(
			mDevice->createResourceLayout(
				{ CodeRed::ResourceLayoutElement(CodeRed::ResourceType::Buffer, 0, 0) },
				{}
			)
		);
		
		mPipelineInfo->setRenderPass(
			mDevice->createRenderPass(
				CodeRed::Attachment::RenderTarget(mSwapChain->format())
			)
		);
	}
private:
	std::shared_ptr<CodeRed::GpuLogicalDevice> mDevice;
	std::shared_ptr<CodeRed::GpuSwapChain> mSwapChain;
	
	std::shared_ptr<CodeRed::GpuCommandAllocator> mCommandAllocator;
	std::shared_ptr<CodeRed::GpuGraphicsCommandList> mCommandList;
	std::shared_ptr<CodeRed::GpuCommandQueue> mCommandQueue;

	std::shared_ptr<CodeRed::GpuPipelineFactory> mPipelineFactory;
	
	std::shared_ptr<CodeRed::GpuBuffer> mVertexBuffer;
	std::shared_ptr<CodeRed::GpuBuffer> mViewBuffer;

	std::shared_ptr<CodeRed::PipelineInfo> mPipelineInfo;
	std::shared_ptr<CodeRed::GpuDescriptorHeap> mDescriptorHeap;
	
	std::vector<std::shared_ptr<CodeRed::GpuFrameBuffer>> mFrameBuffers;
};

int main() {
	auto app = TriangleDemo("TriangleDemo", 1920, 1080);
	
	app.show();
	app.runLoop();
}