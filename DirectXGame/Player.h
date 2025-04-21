#pragma once
#include"KamataEngine.h"
class Player {
    public:

	void Initialize(Model* model, uint32_t textureHandle, Camera* camera);
	void Update();
	void Draw();

	private:

	uint32_t textureHandle_ = 0;
	uint32_t soundDateHandle_ = 0;
	KamataEngine::Sprite* sprite_ = nullptr;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera* camera_=nullptr;
	KamataEngine::DebugCamera* debugCamera_ = nullptr;




};