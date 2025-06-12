#pragma once
#include "KamataEngine.h"
#include"MapChipField.h"
using namespace KamataEngine;
enum class LRDirection {
	kRight,
	kLeft,
};
class Player {
public:
	void Initialize(Model* model, Camera* camera, const Vector3& position);
	void Update();
	void Draw(const Camera& camera);
	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }
	const Vector3 GetVelocity() const { return velocity_; }
	void setMapChipField(MapChipField* mapChipField) { mapChipField=mapChipField_; }

	void InputMove();
	void CheckMapLanding();
	void AnimateTurn();
	void IsMapChipCollisionHappened();
	struct CollisionMapInfo {
		bool topCollision = false;
		bool landingCollision = false;
		bool wallCollision = false;
		Vector3 vec;
	};

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;
	Camera* camera_ = nullptr;
	Vector3 velocity_ = {};
	static inline const float kAcceleration = 0.005f;
	static inline const float kAttenuation = 0.05f;
	static inline const float kLimitRunSpeed = 2.0f;
	LRDirection lrDircetion_ = LRDirection::kRight;

	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.0f;
	static inline const float kTimeTurn = 0.3f;
	bool onGround_ = true;
	static inline const float kGravityAcceleration = 0.05f;
	static inline const float kLimitFallSpeed = 1.0f;
	static inline const float kJumpAcceleration = 0.1f;

	MapChipField* mapChipField_ = nullptr;
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

};