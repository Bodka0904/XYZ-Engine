#pragma once
#include "Sprite.h"

namespace XYZ {
	struct Animation
	{
		std::pair<int, int> frameInterval;
		float frameLen;
		int currentFrame = 0;
	};

	class SpriteAnimation
	{
	public:
		SpriteAnimation(int numRows, int numCols,uint32_t width, uint32_t height);
		void SetFrameInterval(int first, int last, float timeFrame);

		void Update(float dt,Sprite* sprite);
	private:
		const glm::vec4 calcTexCoords();
	private:
		Animation m_Animation;
		uint32_t m_Width;
		uint32_t m_Height;
		
		int m_Rows;
		int m_Columns;
		int m_NumFrames;

		float m_AnimationLen;
		float m_CurrentTime;
	};

}