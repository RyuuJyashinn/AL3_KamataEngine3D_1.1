#pragma once
#include "KamataEngine.h"
#include"MapChipField.h"
#include "Player.h"
using namespace KamataEngine;

class Ball {
public:
	void Initialize(Model* model, Camera* camera, const Vector3& position);
	void Update();
	void Draw(const Camera& camera);

	    void Launch(const Vector3& direction);
	 void CheckPlayerCollision(Player* player);
	void Reset();

	const WorldTransform& GetWorldTransform() const { return worldTransform_; }
	bool IsLaunched() const { return isLaunched_; }
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }
	void SetPlayer(Player* player) { player_ = player; }
	bool CheckTargetCollision();

private:
	struct CollisionMapInfo {
		bool ceiling = false;
		bool landing = false;
		bool hitWall = false;
		Vector3 move;
	};

	enum Corner { kRightBottom, kLeftBottom, kLeftTop, kRightTop, kNumCorner };

	void CheckMapChipCollision(CollisionMapInfo& info);
	void CheckMapCollisionUp(CollisionMapInfo& info);
	void CheckMapCollisionBottom(CollisionMapInfo& info);
	void CheckMapCollisionLeft(CollisionMapInfo& info);
	void CheckMapCollisionRight(CollisionMapInfo& info);
	Vector3 CornerPosition(const Vector3& center, Corner corner);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	Camera* camera_ = nullptr;
	Vector3 velocity_ = {};
	MapChipField* mapChipField_ = nullptr;
	Player* player_ = nullptr;
	bool isLaunched_ = false;

	// 常量
	static inline const float kRadius = 0.25f; // 球体半径
	static inline const float kBlank = 0.05f;
	static inline const float kInitialSpeed = 0.3f;
	static inline const float kBounceDamping = 0.95f;    // 反弹阻尼
	static inline const float kGravity = 0.0f;           // 如果需要重力
	static inline const float kDeathHeight = -5.0f;      // 死亡高度
	static inline const float kMaxBounceAngle = 1.0472f; // 60度弧度
};
