#include "ResourceHelper.hpp"

void CodeRed::ResourceHelper::updateBuffer(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::shared_ptr<GpuCommandAllocator>& allocator,
	const std::shared_ptr<GpuBuffer>& buffer, 
	const void* data)
{
	//first, we create the command list and command queue for copy resource
	auto commandList = device->createGraphicsCommandList(allocator);
	auto commandQueue = device->createCommandQueue();

	//create the upload buffer
	const auto uploadBuffer = device->createBuffer(
		ResourceInfo::UploadBuffer(buffer->stride(), buffer->count())
	);

	const auto memory = uploadBuffer->mapMemory();
	std::memcpy(memory, data, uploadBuffer->size());
	uploadBuffer->unmapMemory();

	const auto oldLayout = buffer->layout();

	//start recoding the copy commands
	commandList->beginRecoding();

	//we need translate the buffer layout to copy-destination
	commandList->layoutTransition(buffer, ResourceLayout::CopyDestination);

	//copy upload buffer to buffer
	commandList->copyBuffer(uploadBuffer, buffer, buffer->size());

	//translate back the layout of buffer
	commandList->layoutTransition(buffer, oldLayout);

	//end recoding, execute and wait finish
	commandList->endRecoding();
	commandQueue->execute({ commandList });
	commandQueue->waitIdle();
}