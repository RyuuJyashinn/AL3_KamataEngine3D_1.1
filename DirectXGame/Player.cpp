#define NOMINMAX
#include"Player.h"
#include<numbers>
#include<algorithm>
#include<cmath>
#include "KamataEngine.h"
#include"MyMath.h"

using namespace KamataEngine;

void Player::Initialize(Model* model, Camera* camera, const Vector3& position) { 
	
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	camera_ = camera;

}

void Player::InputMove() {

	if (onGround_) {
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {

			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				if (velocity_.x < 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				if (lrDircetion_ != LRDirection::kRight) {
					lrDircetion_ = LRDirection::kRight;

					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
				turnFirstRotationY_ = worldTransform_.rotation_.y;
				turnTimer_ = kTimeTurn;

				acceleration.x += kAcceleration;
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				if (velocity_.x > 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				if (lrDircetion_ != LRDirection::kLeft) {
					lrDircetion_ = LRDirection::kLeft;

					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
				turnFirstRotationY_ = worldTransform_.rotation_.y;
				turnTimer_ = kTimeTurn;

				acceleration.x -= kAcceleration;
			}
			velocity_ += acceleration;
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		} else {
			velocity_.x *= (1.0f - kAttenuation);
		}
		// 空中
	} else {
		velocity_ += Vector3(0, -kGravityAcceleration, 0);
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}
	//jump
		if (Input::GetInstance()->PushKey(DIK_UP)) {

		velocity_ += Vector3(0, kJumpAcceleration, 0);
	}
}

void Player::CheckMapLanding() {

	// 接地判定
	bool landing = false;
	if (velocity_.y < 0) {
		if (worldTransform_.translation_.y <= 1.0f) {
			landing = true;
		}
	}
	// 接地判定
	if (onGround_) {
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		}
	} else {
		if (landing) {
			worldTransform_.translation_.y = 1.0f;
			velocity_.y += (1.0f - kAttenuation);
			velocity_.y = 0.0f;
			onGround_ = true;
		}
	}
}

void Player::AnimateTurn() {

	if (turnTimer_ > 0.0f) {

		turnTimer_ -= 1.0f / 60.0f;

		float destinationRotationYTbale[] = {
		    std::numbers::pi_v<float> / 2.0f,
		    std::numbers::pi_v<float> * 3.0f / 2.0f,
		};

		float destinationRotationY = destinationRotationYTbale[static_cast<uint32_t>(lrDircetion_)];
		worldTransform_.rotation_.y = EaseInOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
	}


}

void Player::IsMapChipCollisionHappened() {




}
void Player::Update() {
	//wasd 
	InputMove();
	
	// 落地
	CheckMapLanding();
	//
	worldTransform_.translation_ += velocity_;

	//转弯
	AnimateTurn();
	//
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

	
void Player::Draw(const Camera& camera) {

	model_->Draw(worldTransform_, camera);

}

