#include "Vulkan/VulkanLogicalDevice.hpp"
#include "Vulkan/VulkanSystemInfo.hpp"

using namespace CodeRed;

int main() {

	auto systemInfo = std::make_shared<VulkanSystemInfo>();
	auto adapters = systemInfo->selectDisplayAdapter();

	auto device = std::make_shared<VulkanLogicalDevice>(adapters[0]);
}