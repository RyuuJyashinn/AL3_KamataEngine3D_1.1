#include"Player.h"
using namespace KamataEngine;

void Player::Initialize(Model* model, Camera* camera) { 
	
	assert(model);
	model_ = model;

	worldTransform_.Initialize();

	camera_ = camera;

}

void Player::Update() {

	worldTransform_.TransferMatrix(); 
}


void Player::Draw(const Camera& camera) {

	model_->Draw(worldTransform_, camera);

}

