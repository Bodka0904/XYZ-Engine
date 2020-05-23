#include "stdafx.h"
#include "SpriteAnimation.h"


namespace XYZ {
	SpriteAnimation::SpriteAnimation(int numRows, int numCols, uint32_t width, uint32_t height)
		:m_Rows(numRows), m_Columns(numCols),
		m_Width(width), m_Height(height), m_AnimationLen(0.0f), m_CurrentTime(0.0f)
	{
		m_NumFrames = numRows * numCols;
	}

	void SpriteAnimation::SetFrameInterval(int first, int last, float timeFrame)
	{
		XYZ_ASSERT(timeFrame > 0.0f, "Frame length can not be zero");
		XYZ_ASSERT(first < last, "First frame must be smaller than last");
		XYZ_ASSERT(last <= m_NumFrames, "Last frame bigger than maximal number of frames");

		m_Animation.FrameInterval.first = first;
		m_Animation.FrameInterval.second = last;
		m_Animation.FrameLen = timeFrame;
		m_AnimationLen = ((m_Animation.FrameInterval.second - m_Animation.FrameInterval.first) + 1) * m_Animation.FrameLen;
	}
	void SpriteAnimation::Update(float dt, Renderable2D* sprite)
	{
		if (m_CurrentTime > m_AnimationLen)
			m_CurrentTime = 0.0f;

		m_Animation.CurrentFrame = (int)floor(m_CurrentTime / m_Animation.FrameLen) + m_Animation.FrameInterval.first;
		sprite->TexCoord = calcTexCoords();
		m_CurrentTime += dt;
	}
	const glm::vec4 SpriteAnimation::calcTexCoords()
	{
		glm::vec4 texCoords;
		float frameWidth = (float)m_Width / (float)m_Columns;
		float frameHeight = (float)m_Height / (float)m_Rows;


		int row = m_Animation.CurrentFrame / m_Columns;
		int column = m_Animation.CurrentFrame - (row * m_Columns);


		texCoords.x = (frameWidth * column) / m_Width;
		texCoords.y = (frameHeight * row) / m_Height;
		texCoords.z = (frameWidth * (column + 1)) / m_Width;
		texCoords.w = (frameWidth * (row + 1)) / m_Height;

		return texCoords;
	}
}