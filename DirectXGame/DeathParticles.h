#pragma once
#include "KamataEngine.h"
#include"MapChipField.h"
#include"MyMath.h"
#include<array>
class DeathParticles {
public:
	void Initialize(Model* model, Camera* camera, const Vector3& position);
	void Update();
	void Draw(const Camera& camera);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;
	Camera* camera_ = nullptr;

	static inline const uint32_t kNumParticles = 8;
	std::array<WorldTransform, kNumParticles> worldTransforms_;

	static inline const float kDuration = 1.5f;
	static inline const float kSpeed = 0.2f;
	static inline const float kAngleUnit = 2.0f * 3.141592f / static_cast<float>(kNumParticles);

	bool isFinished_ = false;
	float counter_ = 0.0f;

	ObjectColor objectColor_;
	Vector4 color_;
};