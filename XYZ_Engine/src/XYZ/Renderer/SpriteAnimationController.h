#pragma once
#include "SpriteAnimation.h"

#include <unordered_map>

namespace XYZ {
	class SpriteAnimationController
	{
	public:
		void AddAnimation(const std::string& name, Animation anim) { m_Animations.insert(std::pair<std::string, Animation>(name, anim)); };
		
		void StartAnimation(const std::string& name) { m_Current = &m_Animations[name]; };
		void UpdateSpriteAnimation(SpriteAnimation* anim);
	private:
		int m_Width;
		int m_Height;

		std::unordered_map<std::string, Animation> m_Animations;
		Animation* m_Current;
	};
}