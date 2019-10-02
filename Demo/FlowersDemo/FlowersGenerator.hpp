#pragma once

#include "glm/gtc/matrix_transform.hpp"
#include <glm/glm.hpp>
#include <vector>

template<typename Property>
struct Leaf {
	Property Point[3];
};

using LeafPosition2 = Leaf<glm::vec2>;
using LeafColor = Leaf<glm::vec4>;

class FlowersGenerator final {
public:
	FlowersGenerator(
		const size_t width,
		const size_t height,
		const size_t count);

	void update(float delta);

	auto positions() noexcept -> void*;

	auto colors() noexcept -> void*;
private:
	const float minRadius = 100.0f;
	const float maxRadius = 200.0f;
	const float heightFactor = 0.5f;
	
	size_t mWidth;
	size_t mHeight;
	size_t mCount;

	std::vector<glm::mat4x4> mTransforms;
	
	std::vector<LeafPosition2> mTransformedPositions;
	std::vector<LeafPosition2> mPositions;
	std::vector<LeafColor> mColors;
};