#include "Player.h"
#include "KamataEngine.h"
using namespace KamataEngine;
void Player::Initialize(Model* model, uint32_t textureHandle, Camera* camera) {
	assert(model);
	model_ = model;

	worldTransform_.Initialize();	
	
	textureHandle_ = textureHandle;
	textureHandle_ = TextureManager::Load("uvChecker.png");

	
	camera_ = camera;

}

void Player::Update() {
	worldTransform_.TransferMatrix();
}


void Player::Draw() {
	
	model_->Draw(worldTransform_, debugCamera_->GetCamera(), textureHandle_);

}