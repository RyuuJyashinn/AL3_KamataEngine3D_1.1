#pragma once
#include "Enemy.h" // 明确包含 Enemy 类
#include "KamataEngine.h"
#include "MapChipField.h" // 明确包含 MapChipField 类
#include "Player.h"       // 明确包含 Player 类
#include "SkyDome.h"      // 明确包含 SkyDome 类
class TitleScene {
public:
	void Initialize();
	void Update();
	void Draw();
	bool IsFinished() const { return finished_; }


private:
	bool finished_ = false;
	KamataEngine::Model* model_ = nullptr; // 明确命名空间
	KamataEngine::Model* modelPlayer_ = nullptr;
	KamataEngine::WorldTransform worldTransform_; // 明确命名空间
	KamataEngine::WorldTransform worldTransformPlayer_;
	KamataEngine::Camera camera_; // 明确命名空间
};