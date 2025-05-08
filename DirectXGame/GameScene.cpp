#include "GameScene.h"
#include "KamataEngine.h"
#include"MyMath.h"

using namespace KamataEngine;

void GameScene::Initialize() {
	worldTransform_.Initialize();
	camera_.Initialize();

	modelBlock_ = Model::CreateFromOBJ("cube");
	PrimitiveDrawer::GetInstance()->SetCamera(&camera_);
	textureHandle_ = TextureManager::Load("uvChecker.png");
	//camera
	debugCamera_ = new DebugCamera(1280, 720);
	
	//box
	const uint32_t kNumBlockHorizontal = 20;
	const uint32_t kNumBlockVirtical = 10;
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;
	worldTransformBlocks_.resize( kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(kNumBlockHorizontal, nullptr); // 👈 明确初始化为nullptr
	}

	for (uint32_t y = 0; y < kNumBlockVirtical; ++y) {
		for (uint32_t x = y%2; x < kNumBlockHorizontal; x+=2) {

			worldTransformBlocks_[y][x] = new WorldTransform();
			worldTransformBlocks_[y][x]->Initialize();
			worldTransformBlocks_[y][x]->translation_.x = kBlockWidth * x;
			worldTransformBlocks_[y][x]->translation_.y = kBlockHeight* y;
		}
	}
	


}
void GameScene::Update() { 

	

	#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = TRUE;
	
	}
	#endif

	if (isDebugCameraActive_) {
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		camera_.TransferMatrix();
	} else {
		camera_.UpdateMatrix();
	}

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;

			worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
			worldTransformBlock->TransferMatrix();

		}
	}


}

void GameScene::Draw() { 
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Model::PreDraw(dxCommon->GetCommandList());

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			modelBlock_->Draw(*worldTransformBlock, camera_);
		}
	}

	Model::PostDraw();


}
