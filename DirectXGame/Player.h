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
	void setMapChipField(MapChipField* mapChipField) {
		mapChipField_ = mapChipField; 
	}

	void InputMove();
	void CheckMapLanding();
	void AnimateTurn();
	struct CollisionMapInfo {
		bool ceiling = false;
		bool landing = false;
		bool hitWall = false;
		Vector3 move;
	};

	void CheckMapChipCollision(CollisionMapInfo& info);
	void CheckMapCollisionUp(CollisionMapInfo& info);
	void CheckMapCollisionBottom(CollisionMapInfo& info);
	void CheckMapCollisionLeft(CollisionMapInfo& info);
	void CheckMapCollisionRight(CollisionMapInfo& info);

	enum Corner {
		kRightBottom,
		kLeftBottom,
		kLeftTop,
		kRightTop,

		kNumCorner
	};
	Vector3 CornerPosition(const Vector3& center, Corner corner);

	void MoveByTheIsHitResult(const CollisionMapInfo& info);
	void SoultionWhenTouchTop(const CollisionMapInfo& info);
	void SoultionWhenLanding(const CollisionMapInfo& info);
	void SoultionWhenWalling(const CollisionMapInfo& info);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;
	Camera* camera_ = nullptr;
	Vector3 velocity_ = {};
	static inline const float kAcceleration = 0.02f;
	static inline const float kAttenuation = 0.05f;
	static inline const float kLimitRunSpeed = 2.0f;
	LRDirection lrDircetion_ = LRDirection::kRight;

	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.0f;
	static inline const float kTimeTurn = 0.3f;
	bool onGround_ = true;
	static inline const float kGravityAcceleration = 0.025f;
	static inline const float kLimitFallSpeed = 0.8f;
	static inline const float kJumpAcceleration = 0.4f;

	MapChipField* mapChipField_ = nullptr;
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
	static inline const float kBlank = 0.03f;
	static inline const float kAttenuationLanding = 0.2f;
	static inline const float kGroundSearchHeight = 0.01f;
	static inline const float kAttenuationWall = 0.2f;

	  bool jumpKeyPressed_ = false;
};