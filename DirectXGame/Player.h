#pragma once
#include"KamataEngine.h"
using namespace KamataEngine;

class Player {
public:
	void Initialize(Model* model, Camera* camera,const Vector3& position);
	void Update();
	void Draw(const Camera& camera);

private:

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;
	Camera* camera_= nullptr;
	Vector3 velocity_ = {};
	static inline const float kAcceleration = 2.0f;
};