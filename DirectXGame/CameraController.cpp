#include"CameraController.h"

void CameraController::Initialize() { 
	camera_.Initialize();

}

void CameraController::Update() {

	
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	camera_.translation_ = targetWorldTransform.translation_ + targetOffset_;
	camera_.UpdateMatrix();
}

void CameraController::Reset() { 

	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	camera_.translation_ = targetWorldTransform.translation_ + targetOffset_;

}