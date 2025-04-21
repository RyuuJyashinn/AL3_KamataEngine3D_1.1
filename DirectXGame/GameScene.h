#pragma once
#include"KamataEngine.h"
#include"Player.h"
class GameScene {
public:
	KamataEngine::Sprite* sprite_ = nullptr;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera camera_;
	KamataEngine::DebugCamera* debugCamera_ = nullptr;
	float inputFloat3[3] = {0, 0, 0};

	public:
	void Initialize();
	void Update();
	void Draw();

private:
	uint32_t textureHandle_ = 0;
	uint32_t soundDateHandle_ = 0;
	Player* player_ = nullptr;
};