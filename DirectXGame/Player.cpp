#define NOMINMAX
#include"Player.h"
#include<numbers>
#include<algorithm>
#include<cmath>
#include "KamataEngine.h"
#include"MyMath.h"
#include"MapChipField.h"
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

void Player::CheckMapChipCollision(CollisionMapInfo& info) {
	CheckMapCollisionUp(info);


}


void Player::CheckMapCollisionUp(CollisionMapInfo& info) {
	if (info.move.y <= 0.0f) {
		return; // 早期 return（避免不必要计算）
	}

	std::array<Vector3, static_cast<uint32_t>(Player::kNumCorner)> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(
		    worldTransform_.translation_ + info.move, // 修正：使用 info.move 而不是 "info.移動量"
		    static_cast<Player::Corner>(i)            // 修正：显式指定 Player::Corner
		);
	}

	MapChipType mapChipType;
	bool isHit = false;
	//左上
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex,indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		isHit = true;
	}
	//右上
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		isHit = true;
	}

	if (isHit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, +kHeight / 2.0f, 0));
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));
		info.ceiling = true;
	
	}

}

void Player::SoultionWhenTouchTop(const CollisionMapInfo& info) {

	if (info.ceiling) {
		velocity_.y = 0;
	}
};

Vector3 Player::CornerPosition(const Vector3& center, Player::Corner corner) {
	Vector3 offsetTable[static_cast<uint32_t>(Player::kNumCorner)] = {
	    {+kWidth / 2.6f, -kHeight / 2.0f, 0}, // kRightBottom
	    {-kWidth / 2.0f, -kHeight / 2.8f, 0}, // kLeftBottom
	    {+kWidth / 2.0f, kHeight / 2.0f,  0}, // kRightTop
	    {-kWidth / 2.0f, kHeight / 2.0f,  0}  // kLeftTop
	};

	return center + offsetTable[static_cast<uint32_t>(corner)];
}

void Player::MoveByTheIsHitResult(const CollisionMapInfo& info) {

	worldTransform_.translation_ += info.move;

};



void Player::CheckMapCollisionBottom(CollisionMapInfo& info) {
	if (info.move.y>0) {
		return;
	}
	MapChipType mapChipType;
	bool isHit = false;
	MapChipField::IndexSet indexSet;


	if (isHit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, -kHeight / 2.0f, 0));
		MapChipField::Rect rect=mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);

		info.move.y = std::min(0.0f, rect.top - worldTransform_.translation_.y + (kHeight / 2.0f + kBlank));
		info.landing = true;
	}
}

// ⑥接地状態の切り替え処理
void Player::CheckMapLanding(const CollisionMapInfo& info) {
	// 自キャラが接地状態?
	if (onGround_) {
		// 接地状態の処理
		//  ジャンプ開始
		if (velocity_.y >0.0f)
			onGround_ = false;

	} else {
		// 落下判定
		MapChipType mapChipType;
		bool isHit = false;

		//  移動後の4つの角の座標
		//  左下点の判定
		std::array<Vector3, static_cast<uint32_t>(Player::kNumCorner)> positionsNew;
        MapChipField::IndexSet indexSet;
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
		if (mapChipType == MapChipType::kBlock) {
			isHit = true;
		}
		//  右下点の判定
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
		if (mapChipType == MapChipType::kBlock) {
			isHit = true;
		}
		//  落下なら空中状態に切り替え
		if (!isHit) {
			// 空中状態に切り替える
			onGround_ = false;
		} else {
			// 空中状態の処理
			//  着地フラグ
			if (info.landing) {
				// 着地状態に切り替える(落下を止める)
				onGround_ = true;
				// 着地時にX速度を減衰
				velocity_.x *= (1.0f - kAttenuationLanding);
				// Y速度をゼロにする
				velocity_.y = 0.0f;
			}
		}
	}
}

void Player::Update() {
	InputMove();

	CollisionMapInfo collisionMapInfo;
	collisionMapInfo.move = velocity_;

	// 碰撞检测和处理
	CheckMapChipCollision(collisionMapInfo);
	SoultionWhenTouchTop(collisionMapInfo);

	// 应用修正后的移动量（不再叠加velocity_）
	worldTransform_.translation_ += collisionMapInfo.move;

	// 地面检测（使用修正后的位置）
	CheckMapLanding();
    

	// 6. 其他逻辑（旋转、矩阵更新等）
	AnimateTurn();
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

	
void Player::Draw(const Camera& camera) {

	model_->Draw(worldTransform_, camera);

}

