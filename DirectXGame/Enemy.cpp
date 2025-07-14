#include"Enemy.h"
#include<numbers>
#include<algorithm>
#include<cmath>
#include "KamataEngine.h"
#include"MyMath.h"
#include"MapChipField.h"
using namespace KamataEngine;

void Enemy::Initialize(Model* model, Camera* camera, const Vector3& position){
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	camera_ = camera;

	velocity_ = {kWalkSpeed, 0, 0};
}

void Enemy::Update() {
	// タイマー更新
	walkTime_ += 1.0f / 60.0f;

	// 位置更新
	worldTransform_.translation_ += velocity_;

	// アニメーション周期でループ
	if (walkTime_ > kWalkMotiontime) {
		walkTime_ -= kWalkMotiontime;
	}

	// 回転アニメーション計算
	// 1. 周期に基づいて0.0~1.0の値を取得
	float cycle = walkTime_ / kWalkMotiontime;

	// 2. サイン波で-1.0~1.0の値を取得 (2πを周期とする)
	float param = std::sin(cycle * 2.0f * std::numbers::pi_v<float>);

	// 3. 値を0.0~1.0に変換
	float t = (param + 1.0f) / 2.0f;

	// 4. 角度を線形補間
	float degree = kWalkMotionAngleStart + (kWalkMotionAngleEnd - kWalkMotionAngleStart) * t;

	// 5. ラジアンに変換してX軸回転を適用
	worldTransform_.rotation_.x = degree * (std::numbers::pi_v<float> / 180.0f);

	// ワールド行列更新
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}
Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}
AABB Enemy::GetAABB() {
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;
	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};
	return aabb;
}

void Enemy::OnCollision(const Player* player) { 
	(void)player;
}

void Enemy::Draw(const Camera& camera) {
	model_->Draw(worldTransform_, camera);
}