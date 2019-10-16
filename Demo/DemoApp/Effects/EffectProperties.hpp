#pragma once

#include <Core/CodeRedGraphics.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

#define MAX_LIGHTS_PER_TYPE 16
#define MAX_ALL_LIGHTS MAX_LIGHTS_PER_TYPE * 3

namespace CodeRed {

	enum class LightType : UInt32 {
		Directional = 0,
		Point = 1,
		Spot = 2
	};

	enum class MaterialType : UInt32 {
		Buffer = 0,
		Texture = 1
	};
	
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

	struct PhysicallyBasedMaterial {
		glm::vec4 DiffuseAlbedo = glm::vec4(1.0f);
		glm::vec1 Metallic = glm::vec1(0.0f);
		glm::vec1 Roughness = glm::vec1(0.25f);
		glm::vec1 AmbientOcclusion = glm::vec1(1.0f);
		glm::vec1 Unused = glm::vec1(0.0f);

		PhysicallyBasedMaterial() = default;

		PhysicallyBasedMaterial(
			const glm::vec4& diffuseAlbedo,
			const glm::vec1& metallic,
			const glm::vec1& roughness,
			const glm::vec1& ambientOcclusion) :
			DiffuseAlbedo(diffuseAlbedo),
			Metallic(metallic),
			Roughness(roughness),
			AmbientOcclusion(ambientOcclusion) {}
	};

	struct PhysicallyBasedTextureMaterial {
		std::shared_ptr<GpuTexture> DiffuseAlbedo;
		std::shared_ptr<GpuTexture> Metallic;
		std::shared_ptr<GpuTexture> Normal;
		std::shared_ptr<GpuTexture> Roughness;
		std::shared_ptr<GpuTexture> AmbientOcclusion;

		PhysicallyBasedTextureMaterial() = default;

		PhysicallyBasedTextureMaterial(
			const std::shared_ptr<GpuTexture>& diffuseAlbedo,
			const std::shared_ptr<GpuTexture>& metallic,
			const std::shared_ptr<GpuTexture>& normal,
			const std::shared_ptr<GpuTexture>& roughness,
			const std::shared_ptr<GpuTexture>& ambientOcclusion) :
			DiffuseAlbedo(diffuseAlbedo),
			Metallic(metallic),
			Normal(normal),
			Roughness(roughness),
			AmbientOcclusion(ambientOcclusion) {}
	};

	using PBRMaterial = PhysicallyBasedMaterial;
	using PBRTextureMaterial = PhysicallyBasedTextureMaterial;

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
			Direction(glm::normalize(direction)),
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
			const glm::vec3& position,
			const glm::vec3& direction,
			const glm::vec1& falloffStart,
			const glm::vec1& falloffEnd,
			const glm::vec1& spotPower) -> Light
		{
			return Light(
				strength, position, direction,
				falloffStart, falloffEnd, spotPower
			);
		}
	};

	struct Transform3D {
		glm::mat4x4 NormalTransform = glm::mat4x4(1);
		glm::mat4x4 Projection = glm::mat4x4(1);
		glm::mat4x4 Transform = glm::mat4x4(1);
		glm::mat4x4 View = glm::mat4x4(1);
		glm::vec4 EyePosition = glm::vec4(1);

		Transform3D() = default;

		Transform3D(
			const glm::mat4x4& projection,
			const glm::mat4x4& transform,
			const glm::mat4x4& view,
			const glm::vec4& eyePosition) :
			NormalTransform(glm::transpose(glm::inverse(transform))),
			Projection(projection),
			Transform(transform),
			View(view),
			EyePosition(eyePosition) {}
	};
}