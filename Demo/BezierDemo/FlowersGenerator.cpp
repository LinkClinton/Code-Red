#include "FlowersGenerator.hpp"

#include <random>

auto generateColor(
	const std::default_random_engine& random,
	const std::uniform_real_distribution<float>& range
) -> glm::vec4
{
	return glm::vec4(range(random), range(random), range(random), range(random));
}

FlowersGenerator::FlowersGenerator(
	const size_t width, 
	const size_t height, 
	const size_t count) :
	mWidth(width), mHeight(height), mCount(count)
{
	mTransformedPositions.resize(mCount * 8);
	mPositions.resize(mCount * 8);
	mColors.resize(mCount * 8);

	mTransforms.resize(mCount);

	std::default_random_engine random(0);

	const std::uniform_real_distribution<float> xRange(0.0f, static_cast<float>(mWidth));
	const std::uniform_real_distribution<float> yRange(0.0f, static_cast<float>(mHeight));
	const std::uniform_real_distribution<float> rRange(minRadius, maxRadius);
	const std::uniform_real_distribution<float> cRange(0.0f, 1.0f);
	
	//build each flwoer
	for (size_t index = 0; index < mCount; index++) {
		const auto position = glm::vec2(xRange(random), yRange(random));
		const auto radius = rRange(random);

		const auto halfRadius = radius * 0.5f;
		const auto halfHeight = radius * heightFactor;

		std::vector<glm::vec2> midOffset =
		{
			glm::vec2(halfRadius,halfHeight),
			glm::vec2(halfRadius,-halfHeight),
			glm::vec2(halfHeight, -halfRadius),
			glm::vec2(-halfHeight, -halfRadius),
			glm::vec2(-halfRadius, -halfHeight),
			glm::vec2(-halfRadius, halfHeight),
			glm::vec2(-halfHeight, halfRadius),
			glm::vec2(halfHeight, halfRadius)
		};
		
		std::vector<glm::vec2> endOffset =
		{
			glm::vec2(radius,0),
			glm::vec2(radius,0),
			glm::vec2(0,-radius),
			glm::vec2(0,-radius),
			glm::vec2(-radius,0),
			glm::vec2(-radius,0),
			glm::vec2(0,radius),
			glm::vec2(0,radius)
		};

		const auto centerColor = generateColor(random, cRange);
		const auto endColor = generateColor(random, cRange);

		for (size_t offset = 0; offset < 8; offset++) {
			mPositions[index * 8 + offset] = {
				glm::vec2(0, 0),
				midOffset[offset],
				endOffset[offset]
			};

			mColors[index * 8 + offset] = {
				centerColor,
				generateColor(random, cRange),
				endColor
			};
		}

		mTransforms[index] = glm::translate(glm::mat4(1), glm::vec3(position, 0.0f));
	}
}

void FlowersGenerator::update(float delta, const glm::mat4x4 &view)
{
	const auto speed = glm::two_pi<float>() * 0.25f;
	const auto angle = delta * speed;

	for (auto& transform : mTransforms)
		transform = glm::rotate(transform, angle, glm::vec3(0, 0, 1));

	for (size_t index = 0; index < mPositions.size(); index++) {
		auto& transform = mTransforms[index / 8];

		for (size_t offset = 0; offset < 3; offset++) {
			mTransformedPositions[index].Point[offset] =
				glm::vec4(mPositions[index].Point[offset], 0.0f, 1.0f) *
				transform * view;
		}
	}
}

auto FlowersGenerator::positions() noexcept -> void* 
{
	return mTransformedPositions.data();
}

auto FlowersGenerator::colors() noexcept -> void* 
{
	return mColors.data();
}
