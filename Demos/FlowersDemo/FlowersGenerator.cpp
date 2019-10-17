#include "FlowersGenerator.hpp"

#include <random>

auto generateColor(
	std::default_random_engine& random,
	std::uniform_real_distribution<float>& range
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
	mSpeeds.resize(mCount);
	
	std::default_random_engine random(0);

	std::uniform_real_distribution<float> xRange(0.0f, static_cast<float>(mWidth));
	std::uniform_real_distribution<float> yRange(0.0f, static_cast<float>(mHeight));
	std::uniform_real_distribution<float> rRange(minRadius, maxRadius);
	std::uniform_real_distribution<float> cRange(0.0f, 1.0f);
	std::uniform_real_distribution<float> sRange(-0.3f * glm::two_pi<float>(), 0.3f * glm::two_pi<float>());
	
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
		mSpeeds[index] = sRange(random);
	}
}

void FlowersGenerator::update(float delta)
{	
	for (size_t index = 0; index < mTransforms.size(); index++) {
		const auto speed = mSpeeds[index];
		const auto angle = delta * speed;

		mTransforms[index] = glm::rotate(mTransforms[index], angle, glm::vec3(0, 0, 1));
	}

	for (size_t index = 0; index < mPositions.size(); index++) {
		auto& transform = mTransforms[index / 8];

		for (size_t offset = 0; offset < 3; offset++) {
			mTransformedPositions[index].Point[offset] =
				transform * glm::vec4(mPositions[index].Point[offset], 0.0f, 1.0f);
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
