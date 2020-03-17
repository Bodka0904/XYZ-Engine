#include "stdafx.h"
#include "ECSManager.h"

namespace XYZ {
	std::unique_ptr<ECSManager> ECSManager::m_Instance = std::make_unique<ECSManager>();
}