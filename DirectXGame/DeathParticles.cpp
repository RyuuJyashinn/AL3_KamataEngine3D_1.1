#include"DeathParticles.h"


void DeathParticles::Initialize(Model* model, Camera* camera, const Vector3& position) {

	assert(model);
	model_ = model;
	
	for (WorldTransform& worldTransform : worldTransforms_) {
		worldTransform_.Initialize();
		worldTransform_.translation_ = position;
	}

	camera_ = camera;

}


void DeathParticles::Update() {
	for (WorldTransform& worldTransform : worldTransforms_) {
	
		worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
		worldTransform_.TransferMatrix();
	}



}


void DeathParticles::Draw(const Camera& camera) {
	for (WorldTransform& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform_, camera);
	}
	
}
