#pragma once
#include "XYZ/ECS/Component.h"

#include "SortingLayer.h"
#include "Material.h"

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
			std::shared_ptr<Material> material,
			const glm::vec4& color,
			const glm::vec4& texCoord,
			bool visible,
			int textureID
			)
		:
			Material(material),
			Color(color),
			TexCoord(texCoord),
			Visible(visible),
			TextureID(textureID),
			SortLayerID(SortingLayer::Get().GetOrderValueByName("default"))
		{}

		~Renderable2D()
		{
		}

		std::shared_ptr<Material> Material;
		glm::vec4 Color;
		glm::vec4 TexCoord; // (x = left, y = bottom, z = right, w = top)
		bool Visible;
		int TextureID;
		SortingLayerID SortLayerID;
	};
}