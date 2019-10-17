#include "ImGuiContext.hpp"

#include <Interface/GpuPipelineState/GpuPipelineFactory.hpp>
#include <Interface/GpuCommandAllocator.hpp>
#include <Interface/GpuCommandQueue.hpp>

CodeRed::ImGuiFrameResources::ImGuiFrameResources(
	const std::shared_ptr<GpuLogicalDevice>& device) :
	mDevice(device)
{
	mVertexBuffer = mDevice->createBuffer(
		ResourceInfo::VertexBuffer(sizeof(ImDrawVert), mVertexBufferIncrease,
			MemoryHeap::Upload)
	);

	mIndexBuffer = mDevice->createBuffer(
		ResourceInfo::IndexBuffer(sizeof(ImDrawIdx), mIndexBufferIncrease,
			MemoryHeap::Upload)
	);
}

void CodeRed::ImGuiFrameResources::update(ImDrawData* drawData)
{
	CODE_RED_TRY_EXECUTE(
		mVertexBuffer->size() < drawData->TotalVtxCount,
		mVertexBuffer = mDevice->createBuffer(
			ResourceInfo::VertexBuffer(sizeof(ImDrawVert), mVertexBuffer->count() + mVertexBufferIncrease,
				MemoryHeap::Upload)
		)
	);

	CODE_RED_TRY_EXECUTE(
		mIndexBuffer->size() < drawData->TotalIdxCount,
		mIndexBuffer = mDevice->createBuffer(
			ResourceInfo::IndexBuffer(sizeof(ImDrawIdx), mIndexBuffer->count() + mIndexBufferIncrease,
				MemoryHeap::Upload)
		)
	);

	auto vtxMemory = static_cast<ImDrawVert*>(mVertexBuffer->mapMemory());
	auto idxMemory = static_cast<ImDrawIdx*>(mIndexBuffer->mapMemory());
	
	for (auto index = 0; index < drawData->CmdListsCount; index++) {
		const auto commandList = drawData->CmdLists[index];

		std::memcpy(vtxMemory, commandList->VtxBuffer.Data, commandList->VtxBuffer.size_in_bytes());
		std::memcpy(idxMemory, commandList->IdxBuffer.Data, commandList->IdxBuffer.size_in_bytes());

		vtxMemory = vtxMemory + commandList->VtxBuffer.Size;
		idxMemory = idxMemory + commandList->IdxBuffer.Size;
	}

	mVertexBuffer->unmapMemory();
	mIndexBuffer->unmapMemory();
}

CodeRed::ImGuiContext::ImGuiContext(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::shared_ptr<GpuRenderPass>& renderPass, 
	const size_t numFrameResources) :
	mDevice(device), mRenderPass(renderPass)
{
	mSampler = mDevice->createSampler(
		SamplerInfo(FilterOptions::MinLinearMagLinearMipLinear)
	);
	
	mResourceLayout = mDevice->createResourceLayout(
		{
			ResourceLayoutElement(ResourceType::Texture, 0)
		},
		{
			SamplerLayoutElement(mSampler, 1)
		},
		Constant32Bits(16, 2)
	);

	auto pipelineFactory = mDevice->createPipelineFactory();
	
	mPipeline = mDevice->createGraphicsPipeline(
		mRenderPass,
		mResourceLayout,
		pipelineFactory->createInputAssemblyState(
			{
				InputLayoutElement("POSITION", PixelFormat::RedGreen32BitFloat),
				InputLayoutElement("TEXCOORD", PixelFormat::RedGreen32BitFloat),
				InputLayoutElement("COLOR", PixelFormat::RedGreenBlueAlpha8BitUnknown)
			},
			PrimitiveTopology::TriangleList
		),
		nullptr, //todo
		nullptr, //todo
		pipelineFactory->createDetphStencilState(false, false),
		pipelineFactory->createBlendState(BlendProperty(
			true,
			BlendOperator::Add,
			BlendOperator::Add,
			BlendFactor::Zero,
			BlendFactor::InvSrcAlpha,
			BlendFactor::InvSrcAlpha,
			BlendFactor::SrcAlpha
		)),
		pipelineFactory->createRasterizationState(
			FrontFace::Clockwise,
			CullMode::None,
			FillMode::Solid
		)
	);

	for (size_t index = 0; index < numFrameResources; index++) 
		mFrameResouces.push_back(ImGuiFrameResources(mDevice));

	mDescriptorHeap = mDevice->createDescriptorHeap(mResourceLayout);
	
	initializeFontsTexture();
}

void CodeRed::ImGuiContext::setRenderState(
	const std::shared_ptr<GpuGraphicsCommandList>& ctx,
	const ImGuiFrameResources& resources, 
	ImDrawData* drawData) const
{
	//copy from https://github.com/ocornut/imgui/blob/master/examples/imgui_impl_dx12.cpp#L68
	const auto L = drawData->DisplayPos.x;
	const auto R = drawData->DisplayPos.x + drawData->DisplaySize.x;
	const auto T = drawData->DisplayPos.y;
	const auto B = drawData->DisplayPos.y + drawData->DisplaySize.y;
	
	float mvp[4][4] =
	{
		{ 2.0f / (R - L),   0.0f,           0.0f,       0.0f },
		{ 0.0f,         2.0f / (T - B),     0.0f,       0.0f },
		{ 0.0f,         0.0f,           0.5f,       0.0f },
		{ (R + L) / (L - R),  (T + B) / (B - T),    0.5f,       1.0f },
	};

	std::vector<Value32Bit> value32Bits(16);

	std::memcpy(value32Bits.data(), mvp, sizeof(mvp));
	
	ctx->setViewPort({
		0,0,
		drawData->DisplaySize.x,
		drawData->DisplaySize.y,
		0.0f,1.0f
		});

	ctx->setVertexBuffer(resources.vertex());
	ctx->setIndexBuffer(resources.index(),
		sizeof(ImDrawIdx) == 2 ? IndexType::UInt16 : IndexType::UInt32);

	ctx->setGraphicsPipeline(mPipeline);
	ctx->setResourceLayout(mResourceLayout);
	ctx->setDescriptorHeap(mDescriptorHeap);
	ctx->setConstant32Bits(value32Bits);
}

void CodeRed::ImGuiContext::draw(
	const std::shared_ptr<GpuGraphicsCommandList>& ctx, 
	ImDrawData* drawData)
{
	//copy from https://github.com/ocornut/imgui/blob/master/examples/imgui_impl_dx12.cpp#L123
	// Avoid rendering when minimized
	if (drawData->DisplaySize.x <= 0.0f || drawData->DisplaySize.y <= 0.0f)
		return;

	auto &frameResources = mFrameResouces[mCurrentFrameIndex];

	frameResources.update(drawData);

	setRenderState(ctx, frameResources, drawData);

	size_t globalVtxOffset = 0;
	size_t globalIdxOffset = 0;

	const auto clipOff = drawData->DisplayPos;

	for (int nCommnadList = 0; nCommnadList < drawData->CmdListsCount; nCommnadList++) {
		const ImDrawList* commandList = drawData->CmdLists[nCommnadList];

		for (int nCommand = 0; nCommand < commandList->CmdBuffer.Size; nCommand++) {
			const auto command = &commandList->CmdBuffer[nCommand];

			if (command->UserCallback != nullptr) {
				if (command->UserCallback == ImDrawCallback_ResetRenderState)
					setRenderState(ctx, frameResources, drawData);
				else
					command->UserCallback(commandList, command);
			}else {
				//draw call commands
				ScissorRect rect = {
					static_cast<size_t>(command->ClipRect.x - clipOff.x),
					static_cast<size_t>(command->ClipRect.y - clipOff.y),
					static_cast<size_t>(command->ClipRect.z - clipOff.x),
					static_cast<size_t>(command->ClipRect.w - clipOff.y)
				};

				ctx->setScissorRect(rect);
				ctx->drawIndexed(command->ElemCount, 1,
					command->IdxOffset + globalIdxOffset,
					command->VtxOffset + globalVtxOffset, 0);
			}
		}
		
		globalIdxOffset = globalIdxOffset + commandList->IdxBuffer.Size;
		globalVtxOffset = globalVtxOffset + commandList->VtxBuffer.Size;
	}
}

void CodeRed::ImGuiContext::initializeFontsTexture()
{
	auto& io = ImGui::GetIO();

	Byte* pixels = nullptr;

	auto width = 0;
	auto height = 0;

	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

	mFontsTexture = mDevice->createTexture(
		ResourceInfo::Texture2D(width, height, PixelFormat::RedGreenBlueAlpha8BitUnknown)
	);

	const auto commandAllocator = mDevice->createCommandAllocator();
	const auto commandQueue = mDevice->createCommandQueue();
	const auto commandList = mDevice->createGraphicsCommandList(commandAllocator);

	commandList->layoutTransition(mFontsTexture, mFontsTexture->layout(), ResourceLayout::CopyDestination);
	
	commandList->copyMemoryToTexture(mFontsTexture, pixels);

	commandList->layoutTransition(mFontsTexture, mFontsTexture->layout(), ResourceLayout::GeneralRead);

	commandQueue->execute({ commandList });
	commandQueue->waitIdle();

	mDescriptorHeap->bindTexture(mFontsTexture, 0);

	//we set it to nullptr, because we only need one font texture
	io.Fonts->TexID = nullptr;
}
