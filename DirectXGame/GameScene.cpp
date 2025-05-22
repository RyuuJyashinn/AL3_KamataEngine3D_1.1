#include "GameScene.h"
#include "KamataEngine.h"
#include"MyMath.h"

using namespace KamataEngine;
void GameScene::Initialize() {
	// 初始化基础组件
	worldTransform_.Initialize();
	camera_.Initialize();

	// 初始化模型资源
	modelBlock_ = Model::CreateFromOBJ("block");
	modelPlayer_ = Model::CreateFromOBJ("player");

	// 初始化地图数据（必须先于方块初始化）
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	// 初始化方块世界变换
	const uint32_t kNumBlockHorizontal = mapChipField_->GetNumBlockHorizontal();
	const uint32_t kNumBlockVirtical = mapChipField_->GetNumBlockVirual();
	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(kNumBlockHorizontal, nullptr);
	}

	for (uint32_t y = 0; y < kNumBlockVirtical; ++y) {
		for (uint32_t x = y % 2; x < kNumBlockHorizontal; ++x) {
			if (mapChipField_->GetMapChipTypeByIndex(y, x) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[y][x] = worldTransform;
				worldTransformBlocks_[y][x]->translation_ = mapChipField_->GetMapChipPositionByIndex(y, x);
			}
		}
	}

	// 初始化调试相机
	debugCamera_ = new DebugCamera(1280, 720);
	PrimitiveDrawer::GetInstance()->SetCamera(&camera_);

	// 初始化天空盒
	skydome_ = new SkyDome();
	skydome_->Initialize();

	// 初始化玩家
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
