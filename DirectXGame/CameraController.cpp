#include"CameraController.h"

void CameraController::Initialize() { 
	camera_.Initialize();

}

void CameraController::Update() {

	
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	targetOnset_ = targetWorldTransform.translation_ + targetOffset_ + target_->GetVelocity() * kVelocityBias;
	camera_.translation_.x = Lerp(camera_.translation_.x,targetOnset_.x,kInterpolatetionRate);

	camera_.translation_.x = max(camera_.translation_.x, target_->GetVelocity().x+ movableArea_.left);
	camera_.translation_.x = min(camera_.translation_.x,   target_->GetVelocity().x + movableArea_.right);
	camera_.translation_.y = max(camera_.translation_.y, target_->GetVelocity().y + movableArea_.bottom);
	camera_.translation_.y = min(camera_.translation_.y,   target_->GetVelocity().y + movableArea_.top);

	//camera_.translation_.x = max(camera_.translation_.x, target_->GetWorldTransform().translation_.x + margin_.left);
	//camera_.translation_.x = min(camera_.translation_.x, target_->GetWorldTransform().translation_.x + margin_.right);
	//camera_.translation_.y = max(camera_.translation_.y, target_->GetWorldTransform().translation_.y + margin_.top);
	//camera_.translation_.y = min(camera_.translation_.y, target_->GetWorldTransform().translation_.y + margin_.bottom);

	camera_.UpdateMatrix();
}

void CameraController::Reset() { 

	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	camera_.translation_ = targetWorldTransform.translation_ + targetOffset_;

}