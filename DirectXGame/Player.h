#pragma once
#include"KamataEngine.h"
using namespace KamataEngine;
enum class LRDirection {
kRight,
kLeft,
};
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
	static inline const float kAcceleration = 0.1f;
	static inline const float kAttenuation = 0.1f;
	static inline const float kLimitRunSpeed = 5.0f;
	LRDirection lrDircetion_ = LRDirection::kRight;

	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.0f;
	static inline const float kTimeTurn = 0.3f;
};