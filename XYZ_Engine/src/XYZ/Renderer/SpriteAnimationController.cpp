#include "stdafx.h"
#include "SpriteAnimationController.h"



namespace XYZ {
	void SpriteAnimationController::UpdateSpriteAnimation(SpriteAnimation* anim)
	{
		anim->SetFrameInterval(m_Current->FrameInterval.first, m_Current->FrameInterval.second, m_Current->FrameLen);
	}
}