#pragma once
#include "Material.h"
#include "XYZ/ECS/Component.h"

#include <glm/glm.hpp>

namespace XYZ {
	/**
	* @struct Renderable2D
	* @brief represents 2D renderable object.
	*/
	struct Renderable2D : public Type<Renderable2D>
	{
		/**
		* Construct a 2D renderable, from given parameters
		* @param[in] Material	Material of the renderable
		* @param[in] Color		Color of the renderable
		* @param[in] TexCoord	Bottom left and top right texture coordinates of renderable
		* @param[in] Position   Position of the renderable
		* @param[in] Size		Size of the renderable
		* @param[in] Rotation   Rotation of the renderable
		* @param[in] Visible	Specify if the renderable is visible and should be rendered
		* @param[in] TextureID  Specify the ID of the texture
		*/
		Renderable2D(
			std::shared_ptr<Material> Material,
			const glm::vec4& Color,
			const glm::vec4& TexCoord,
			const glm::vec3& Position,
			const glm::vec2& Size,
			float Rotation,
			bool Visible,
			int TextureID
			)
		:
			material(Material),
			color(Color),
			texCoord(TexCoord),
			position(Position),
			size(Size),
			rotation(Rotation),
			visible(Visible),
			textureID(TextureID)
		{}

		~Renderable2D()
		{
		}

		std::shared_ptr<Material> material;
		glm::vec4 color = glm::vec4(0);
		glm::vec4 texCoord = glm::vec4(0); // (x = left, y = bottom, z = right, w = top)
		glm::vec3 position = glm::vec3(0);
		glm::vec2 size = glm::vec2(0);
		float rotation = 0.0f;
		bool visible = true;
		int textureID = 0;
	};
}