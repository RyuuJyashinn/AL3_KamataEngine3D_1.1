#pragma once
#include"KamataEngine.h"
using namespace KamataEngine;

class Player {
public:
	void Initialize(Model* model, Camera* camera);
	void Update();
	void Draw(const Camera& camera);

private:

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;
	Camera* camera_= nullptr;
};