#define NOMINMAX
#include "Ball.h"
#include "KamataEngine.h"
#include "MapChipField.h"
#include "MyMath.h"
#include <algorithm>
#include <cmath>
#include <numbers>
using namespace KamataEngine;

void Ball::Initialize(Model* model, Camera* camera, const Vector3& position) {
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = Vector3(0.5f, 0.5f, 0.5f); // 球体尺寸调整为方块的一半
	camera_ = camera;

	// 初始速度为零，等待发射
	velocity_ = Vector3(0, 0, 0);
	isLaunched_ = false;
}

void Ball::Launch(const Vector3& direction) {
	if (!isLaunched_) {
		velocity_ = direction * kInitialSpeed;
		isLaunched_ = true;
	}
}

void Ball::CheckMapChipCollision(CollisionMapInfo& info) {
	CheckMapCollisionUp(info);
	CheckMapCollisionBottom(info);
	CheckMapCollisionLeft(info);
	CheckMapCollisionRight(info);
}

void Ball::CheckMapCollisionUp(CollisionMapInfo& info) {
	if (info.move.y <= 0.0f) {
		return;
	}

	std::array<Vector3, static_cast<uint32_t>(Ball::kNumCorner)> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Ball::Corner>(i));
	}

	MapChipType mapChipType;
	bool isHit = false;

	// 左上角检测
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		isHit = true;
	}

	// 右上角检测
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		isHit = true;
	}

	if (isHit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, +kRadius, 0));
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - (kRadius + kBlank));
		info.ceiling = true;
	}
}

void Ball::CheckMapCollisionBottom(CollisionMapInfo& info) {
	if (info.move.y >= 0.0f) {
		return;
	}

	std::array<Vector3, static_cast<uint32_t>(Ball::kNumCorner)> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Ball::Corner>(i));
	}

	MapChipType mapChipType;
	bool isHit = false;

	// 左下角检测
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		isHit = true;
	}

	// 右下角检测
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		isHit = true;
	}

	if (isHit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, -kRadius, 0));
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.y = std::min(0.0f, rect.top - worldTransform_.translation_.y + (kRadius + kBlank));
		info.landing = true;
	}
}

void Ball::CheckMapCollisionLeft(CollisionMapInfo& info) {
	if (info.move.x >= 0.0f) {
		return;
	}

	std::array<Vector3, static_cast<uint32_t>(Ball::kNumCorner)> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Ball::Corner>(i));
	}

	MapChipType mapChipType;
	bool isHit = false;

	// 左上角检测
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex - 1, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		isHit = true;
	}

	// 左下角检测
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex - 1, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		isHit = true;
	}

	if (isHit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(-kRadius, 0, 0));
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.x = std::max(0.0f, rect.right - worldTransform_.translation_.x + (kRadius + kBlank));
		info.hitWall = true;
	}
}

void Ball::CheckMapCollisionRight(CollisionMapInfo& info) {
	if (info.move.x <= 0.0f) {
		return;
	}

	std::array<Vector3, static_cast<uint32_t>(Ball::kNumCorner)> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Ball::Corner>(i));
	}

	MapChipType mapChipType;
	bool isHit = false;

	// 右上角检测
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex + 1, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		isHit = true;
	}

	// 右下角检测
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex + 1, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		isHit = true;
	}

	if (isHit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(+kRadius, 0, 0));
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.x = std::min(0.0f, rect.left - worldTransform_.translation_.x - (kRadius + kBlank));
		info.hitWall = true;
	}
}

Vector3 Ball::CornerPosition(const Vector3& center, Ball::Corner corner) {
	Vector3 offsetTable[static_cast<uint32_t>(Ball::kNumCorner)] = {
	    {+kRadius, -kRadius, 0}, // kRightBottom
	    {-kRadius, -kRadius, 0}, // kLeftBottom
	    {-kRadius, +kRadius, 0}, // kLeftTop
	    {+kRadius, +kRadius, 0}  // kRightTop
	};

	return center + offsetTable[static_cast<uint32_t>(corner)];
}

void Ball::CheckPlayerCollision(Player* player) {
	if (!player || !isLaunched_)
		return;

	// 获取玩家位置和尺寸
	Vector3 playerPos = player->GetWorldTransform().translation_;
	Vector3 playerSize = Vector3(player->GetWidth() / 2.0f, player->GetHeight() / 2.0f, 0);

	// 检测球与玩家的碰撞
	Vector3 ballPos = worldTransform_.translation_;

	// AABB碰撞检测
	if (ballPos.x + kRadius > playerPos.x - playerSize.x && ballPos.x - kRadius < playerPos.x + playerSize.x && ballPos.y + kRadius > playerPos.y - playerSize.y &&
	    ballPos.y - kRadius < playerPos.y + playerSize.y) {

		// 计算碰撞反弹方向
		Vector3 normal = Vector3(0, 1, 0); // 默认向上反弹

		// 根据碰撞位置调整反弹角度
		float relativeIntersectX = (playerPos.x - ballPos.x) / playerSize.x;
		float bounceAngle = relativeIntersectX * kMaxBounceAngle;

		// 设置新的速度方向
		velocity_.x = kInitialSpeed * std::sin(bounceAngle);
		velocity_.y = kInitialSpeed * std::cos(bounceAngle);

		// 确保球向上运动
		if (velocity_.y < 0) {
			velocity_.y = -velocity_.y;
		}
	}
}

void Ball::Update() {
	if (!isLaunched_)
		return;

	// 应用重力（如果需要）
	// velocity_.y -= kGravity * (1.0f / 60.0f);

	CollisionMapInfo collisionMapInfo;
	collisionMapInfo.move = velocity_;

	// 地图碰撞检测
	CheckMapChipCollision(collisionMapInfo);

	// 处理碰撞反弹
	if (collisionMapInfo.ceiling || collisionMapInfo.landing) {
		velocity_.y = -velocity_.y * kBounceDamping;
	}
	if (collisionMapInfo.hitWall) {
		velocity_.x = -velocity_.x * kBounceDamping;
	}

	// 应用移动
	worldTransform_.translation_ += collisionMapInfo.move;

	// 检查是否掉落到底部（游戏结束条件）
	if (worldTransform_.translation_.y < kDeathHeight) {
		Reset();
	}

	// 更新世界变换
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void Ball::Reset() {
	isLaunched_ = false;
	velocity_ = Vector3(0, 0, 0);
	// 重置位置到玩家上方
	// 这个需要在GameScene中设置
}

void Ball::Draw(const Camera& camera) { model_->Draw(worldTransform_, camera); }