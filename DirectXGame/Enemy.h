#pragma once
#include "KamataEngine.h"
#include"MapChipField.h"
#include "Player.h" 
class Player;
class Enemy {
public:
	void Initialize(Model* model, Camera* camera, const Vector3& position);
	void Update();
	void Draw(const Camera& camera);

	Vector3 GetWorldPosition();
	AABB GetAABB();
	void OnCollision(const Player* player);


	private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;
	Camera* camera_ = nullptr;

	static inline const float kWalkSpeed = 0.01f;
	Vector3 velocity_ = {};
	static inline const float kWalkMotionAngleStart = -15.0f;
	static inline const float kWalkMotionAngleEnd =15.0f;
	static inline const float kWalkMotiontime = 2.0f;

	float walkTime_ = 0.0f;

		static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
};