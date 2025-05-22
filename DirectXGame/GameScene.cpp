#include "GameScene.h"
#include "KamataEngine.h"
#include"MyMath.h"

using namespace KamataEngine;
void GameScene::Initialize() {
	worldTransform_.Initialize();
	camera_.Initialize();

	modelBlock_ = Model::CreateFromOBJ("block");
	modelPlayer_ = Model::CreateFromOBJ("player");

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	const uint32_t kNumBlockHorizontal = mapChipField_->GetNumBlockHorizontal();
	const uint32_t kNumBlockVirtical = mapChipField_->GetNumBlockVirual();
	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	for (uint32_t y = 0; y < kNumBlockVirtical; ++y) {
		for (uint32_t x = 0; x < kNumBlockHorizontal; ++x) {
			if (mapChipField_->GetMapChipTypeByIndex(x, y) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[y][x] = worldTransform;
				worldTransformBlocks_[y][x]->translation_ = mapChipField_->GetMapChipPositionByIndex(x, y);
			}
		}
	}

	debugCamera_ = new DebugCamera(1280, 720);
	PrimitiveDrawer::GetInstance()->SetCamera(&camera_);

	skydome_ = new SkyDome();
	skydome_->Initialize();


	player_ = new Player();
	player_->Initialize(modelPlayer_, &camera_);
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

	skydome_->Update();
	player_->Update();
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

		skydome_->Draw(camera_);
	    player_->Draw(camera_);

	Model::PostDraw();
	  

}
