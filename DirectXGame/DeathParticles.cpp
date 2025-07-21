#include"DeathParticles.h"
#include"MyMath.h"
#include"KamataEngine.h"
#include<algorithm>
using namespace MathUtility;
using namespace KamataEngine;

void DeathParticles::Initialize(Model* model, Camera* camera, const Vector3& position) {

	assert(model);
	model_ = model;
	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};
	for (WorldTransform& worldTransform : worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}

	camera_ = camera;

}


void DeathParticles::Update() {
	if (isFinished_) {
		return;
	}

	color_.w = std::clamp(1.0f - counter_ / kDuration, 0.0f, 1.0f);
	// 色変更オブジェクトに色の数値を設定する
	objectColor_.SetColor(color_);

	for (uint32_t i = 0; i < kNumParticles;++i) {
			// 基本となる速度ベクトル
		    Vector3 velocity = {kSpeed, 0, 0};
			// 回転角を計算する
		    float angle = kAngleUnit * i;
			// Z軸まわり回転行列
		    Matrix4x4 matrixRotation = MakeRotateZMatrix(angle);
			// 基本ベクトルを回転させて速度ベクトルを得る
			velocity = Transform(velocity, matrixRotation);
			// 移動処理
			worldTransforms_[i].translation_.x += velocity.x;
		    worldTransforms_[i].translation_.y += velocity.y;
		    worldTransforms_[i].translation_.z += velocity.z;
	}

	counter_ += 1.0f / 60.0f;
	// 存続時間の上限に達したら
	if (counter_ >= kDuration) {
		counter_ = kDuration;
		// 終了扱いにする
		isFinished_ = true;
	}
	

	for (WorldTransform& worldTransform : worldTransforms_) {
	
		worldTransform.matWorld_ = MakeAffineMatrix(worldTransform.scale_, worldTransform.rotation_, worldTransform.translation_);
		worldTransform.TransferMatrix();
	}
}


void DeathParticles::Draw(const Camera& camera) {
	if (isFinished_) {

		return;
	}
	for (WorldTransform& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, camera,&objectColor_);
	}
	
}
