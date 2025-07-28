#pragma once
#include "Enemy.h"
#include "KamataEngine.h"
#include "MapChipField.h"
#include "Player.h"      
#include "SkyDome.h"     
class TitleScene {
public:
	void Initialize();
	void Update();
	void Draw();
	bool IsFinished() const { return finished_; } 

	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Model* modelPlayer_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::WorldTransform worldTransformPlayer_;
	KamataEngine::Camera camera_;

private:
	bool finished_ = false;
	
};