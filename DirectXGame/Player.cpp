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
	/*if (Input::GetInstance()->PushKey(DIK_UP)) {

		velocity_ += Vector3(0, kJumpAcceleration, 0);
	}*/
	bool upKeyPressed = Input::GetInstance()->PushKey(DIK_UP);

	if (upKeyPressed) {
		if (!jumpKeyPressed_ && onGround_) { // 只有首次按下且在地面时才跳跃
			velocity_.y = kJumpAcceleration;
			onGround_ = false;
			jumpKeyPressed_ = true; // 标记按键已按下
		}
	} else {
		jumpKeyPressed_ = false; // 按键松开时重置状态
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
	CheckMapCollisionBottom(info);
	CheckMapCollisionLeft(info);
	CheckMapCollisionRight(info);
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
	MapChipType mapChipTypeNext;
	bool isHit = false;
	//左上
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex,indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex + 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		isHit = true;
	}
	//右上
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex + 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		isHit = true;
	}


	if (isHit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, +kHeight / 2.0f, 0));
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, +kHeight / 2.0f, 0));
		if (indexSetNow.yIndex != indexSet.yIndex) {
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.move.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));
			info.ceiling = true;
		}
	}
	//if (isHit) {
	//	indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, +kHeight / 2.0f, 0));
	//	MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
	//	info.move.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));
	//	info.ceiling = true;
	//
	//}

}


void Player::CheckMapCollisionBottom(CollisionMapInfo& info) {
	if (info.move.y >= 0.0f) {
		return;
	}

	std::array<Vector3, static_cast<uint32_t>(Player::kNumCorner)> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Player::Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	bool leftIsHit = false;
	bool rightIsHit = false;
	// 左下
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom] + Vector3(0, -kGroundSearchHeight, 0));
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex-1);
	if (mapChipType == MapChipType::kBlock&&mapChipTypeNext != MapChipType::kBlock) {
		leftIsHit = true;
	}

	// 右下

    indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom] + Vector3(0, -kGroundSearchHeight, 0));
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex - 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		rightIsHit = true;
	}


	if (rightIsHit|| leftIsHit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, -kHeight / 2.0f, 0));
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_  + Vector3(0, -kHeight / 2.0f, 0));
		if (indexSetNow.yIndex != indexSet.yIndex) {
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.move.y = std::min(0.0f, rect.top - worldTransform_.translation_.y + (kHeight / 2.0f + kBlank));
			info.landing = true;
		}
		
	}
}

void Player::CheckMapCollisionLeft(CollisionMapInfo& info) {
if (info.move.x >= 0.0f) {
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
	//MapChipType mapChipTypeNext;
	bool isHit = false;
	// 左上
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	//mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex-1, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock/* && mapChipTypeNext != MapChipType::kBlock*/) {
		isHit = true;
	}
	// 左上
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	//mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex-1, indexSet.yIndex );
	if (mapChipType == MapChipType::kBlock/* && mapChipTypeNext != MapChipType::kBlock*/) {
		isHit = true;
	}

	if (isHit) {

		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(-kWidth / 2.0f, 0, 0));
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.x = std::max(0.0f, (rect.right - worldTransform_.translation_.x + (kWidth / 2.0f + kBlank)));
		info.hitWall = true;
	}


}

void Player::CheckMapCollisionRight(CollisionMapInfo& info) {
	if (info.move.x <= 0.0f) {
		return; 
	}

	std::array<Vector3, static_cast<uint32_t>(Player::kNumCorner)> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(
		    worldTransform_.translation_ + info.move, 
		    static_cast<Player::Corner>(i)            
		);
	}

	MapChipType mapChipType;
	//MapChipType mapChipTypeNext;
	bool isHit = false;
	// 右上
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	//mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex+1, indexSet.yIndex );
	if (mapChipType == MapChipType::kBlock /*&& mapChipTypeNext != MapChipType::kBlock*/) {
		isHit = true;
	}
	// 右下
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	//mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex+1, indexSet.yIndex );
	if (mapChipType == MapChipType::kBlock/* && mapChipTypeNext != MapChipType::kBlock*/) {
		isHit = true;
	}

	if (isHit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(+kWidth / 2.0f, 0, 0));
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.x = std::min(0.0f,(rect.left - worldTransform_.translation_.x - (kWidth / 2.0f + kBlank)));
		info.hitWall = true;
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




//接地状態の切り替え処理
void Player::SoultionWhenLanding(const CollisionMapInfo& info) {

	if (onGround_) {
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		} else {
			//落下 
			MapChipType mapChipType;
			bool leftHit = false;
			bool rightHit = false;
			std::array<Vector3, static_cast<uint32_t>(Player::kNumCorner)> positionsNew;
			for (uint32_t i = 0; i < positionsNew.size(); ++i) {
				positionsNew[i] = CornerPosition(worldTransform_.translation_, static_cast<Player::Corner>(i));
			}
			// 左下点检测
			MapChipField::IndexSet indexSet;
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom] + Vector3(0, -kGroundSearchHeight, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				leftHit = true;
			}

			// 右下点检测

			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom] + Vector3(0, -kGroundSearchHeight, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				rightHit = true;
			}

		if (!leftHit && !rightHit) {
				onGround_ = false;	
		}
		}
	}
	// 空中
	else {
		if (info.landing) {
			onGround_ = true;                            
			velocity_.x *= (1.0f - kAttenuationLanding); 
			velocity_.y = 0.0f;                          
		}
	}
}

void Player::SoultionWhenWalling(const CollisionMapInfo& info) {
	if (info.hitWall) {
		
		if (std::abs(velocity_.x) > 0.1f) {
			velocity_.x *= (1.0f - kAttenuationWall);
		} else {
			velocity_.x = 0.0f; 
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
	SoultionWhenLanding(collisionMapInfo);
	SoultionWhenWalling(collisionMapInfo);
	// 应用修正后的移动量（不再叠加velocity_）


	worldTransform_.translation_ += collisionMapInfo.move;


	// 6. 其他逻辑（旋转、矩阵更新等）
	AnimateTurn();
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

	
void Player::Draw(const Camera& camera) {

	model_->Draw(worldTransform_, camera);

}

