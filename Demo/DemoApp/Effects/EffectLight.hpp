#pragma once

#include <glm/glm.hpp>

#define MAX_LIGHTS 16
#define MAX_MATERIALS MAX_LIGHTS

namespace CodeRed {

	struct Material {
		glm::vec4 DiffuseAlbedo = glm::vec4(1.0f);
		glm::vec3 FreshelR0 = glm::vec3(0.01f);
		glm::vec1 Roughness = glm::vec1(0.25f);

		Material() = default;

		Material(
			const glm::vec4& diffuseAlbedo,
			const glm::vec3& freshelR0,
			const glm::vec1& roughness) :
			DiffuseAlbedo(diffuseAlbedo),
			FreshelR0(freshelR0),
			Roughness(roughness) {}
	};

	struct Light {
		glm::vec3 Strength = glm::vec3(0);
		glm::vec1 FalloffStart = glm::vec1(0);
		glm::vec3 Direction = glm::vec3(0);
		glm::vec1 FalloffEnd = glm::vec1(0);
		glm::vec3 Position = glm::vec3(0);
		glm::vec1 SpotPower = glm::vec1(0);

		Light() = default;

		Light(
			const glm::vec3& strength,
			const glm::vec3& position,
			const glm::vec3& direction,
			const glm::vec1& falloffStart,
			const glm::vec1& falloffEnd,
			const glm::vec1& spotPower) :
			Strength(strength),
			FalloffStart(falloffStart),
			Direction(direction),
			FalloffEnd(falloffEnd),
			Position(position),
			SpotPower(spotPower) {}

		static auto DirectionalLight(
			const glm::vec3& strength,
			const glm::vec3& direction) -> Light
		{
			return Light(strength, glm::vec3(0), direction,
				glm::vec1(0),
				glm::vec1(0),
				glm::vec1(0));
		}

		static auto PointLight(
			const glm::vec3& strength,
			const glm::vec3& position,
			const glm::vec1& falloffStart,
			const glm::vec1& falloffEnd) -> Light
		{
			return Light(
				strength, position, glm::vec3(0),
				falloffStart, falloffEnd, glm::vec1(0)
			);
		}

		static auto SpotLight(
			const glm::vec3& strength,
			const glm::vec3& direction,
			const glm::vec1& falloffStart,
			const glm::vec1& falloffEnd,
			const glm::vec1& spotPower) -> Light
		{
			return Light(
				strength, glm::vec3(0), direction,
				falloffStart, falloffEnd, spotPower
			);
		}
	};

}