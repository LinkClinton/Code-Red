#include "FrameResources.hpp"

CodeRed::FrameResources::FrameResources(const std::vector<std::string>& names)
{
	for (auto &name : names) mResources.insert({ name, nullptr });
}

CodeRed::FrameResources::FrameResources(const std::initializer_list<std::string>& names)
{
	for (auto& name : names) mResources.insert({ name, nullptr });
}
