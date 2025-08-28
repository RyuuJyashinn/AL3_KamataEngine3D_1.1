#include "GameScene.h"
#include "KamataEngine.h"
#include"MyMath.h"

using namespace KamataEngine;
void GameScene::Initialize() {

	//phase_ = Phase::kPlay;
	worldTransform_.Initialize();
	camera_.Initialize();
	//
	const uint32_t kNumBlockHorizontal = mapChipField_->GetNumBlockHorizontal();
	const uint32_t kNumBlockVirtical = mapChipField_->GetNumBlockVirual();
	worldTransformBlocks_.resize(kNumBlockVirtical);

    for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	modelBlock_ = Model::CreateFromOBJ("blackblock");
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/theMap.csv");

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
	//

	worldTransformTarget_.resize(kNumBlockVirtical);

	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		worldTransformTarget_[i].resize(kNumBlockHorizontal);
	}

	modelTarget_ = Model::CreateFromOBJ("redblock");
	mapChipTarget_=new MapChipField;
	mapChipTarget_->LoadMapChipCsv("Resources/theMap.csv");


	for (uint32_t y = 0; y < kNumBlockVirtical; ++y) {
		for (uint32_t x = 0; x < kNumBlockHorizontal; ++x) {
			if (mapChipTarget_->GetMapChipTypeByIndex(x, y) == MapChipType::kTarget) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformTarget_[y][x] = worldTransform;
				worldTransformTarget_[y][x]->translation_ = mapChipTarget_->GetMapChipPositionByIndex(x, y);
			}
		}
	}

	debugCamera_ = new DebugCamera(1280, 720);
	PrimitiveDrawer::GetInstance()->SetCamera(&camera_);

	skydome_ = new SkyDome();
	skydome_->Initialize();


	player_ = new Player();
	player_->setMapChipField(mapChipField_);//2.7
	modelPlayer_ = Model::CreateFromOBJ("whiteblock");
	Vector3 playerPostion = mapChipField_->GetMapChipPositionByIndex(10, 16);
	player_->Initialize(modelPlayer_, &camera_, playerPostion);
	//ball
	ball_ = new Ball();
	ball_->SetMapChipField(mapChipField_);
	ball_->SetPlayer(player_);
	modelBall_ = Model::CreateFromOBJ("ball");                 // 使用方块模型
	Vector3 ballPosition = playerPostion + Vector3(0, 1.0f, 0); // 在玩家上方
	ball_->Initialize(modelBall_, &camera_, ballPosition);
	// camera control
	cameraController_ = new CameraController();
	cameraController_->Initialize();
	cameraController_->setTarget(player_);
	cameraController_->Reset();

	CameraController::Rect cameraArea = {10.0f, 100-12.0f ,5.0f,5.0f};
	cameraController_->SetMovableArea(cameraArea);
	//
}
void GameScene::Update() { 

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;

			worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
			worldTransformBlock->TransferMatrix();

		}
	}

	
	for (std::vector<WorldTransform*>& worldTransformTargetLine : worldTransformTarget_) {
		for (WorldTransform* worldTransformTarget : worldTransformTargetLine) {
			if (!worldTransformTarget)
				continue;

			worldTransformTarget->matWorld_ = MakeAffineMatrix(worldTransformTarget->scale_, worldTransformTarget->rotation_, worldTransformTarget->translation_);
			worldTransformTarget->TransferMatrix();
		}
	}
	//ball
	if (Input::GetInstance()->TriggerKey(DIK_RETURN) && !ball_->IsLaunched()) {
		Vector3 playerPosition = player_->GetWorldTransform().translation_;

		Vector3 ballPosition = playerPosition + Vector3(0, 1.0f, 0); // 在玩家上方
		ball_->Initialize(modelBall_, &camera_, ballPosition);
		// 随机方向（向上偏左或偏右）
		float randomAngle = (rand() % 2 == 0) ? 0.785f : -0.785f; // 45度或-45度
		Vector3 launchDirection = Vector3(std::sin(randomAngle), std::cos(randomAngle), 0);
		ball_->Launch(launchDirection);
	}

	ball_->Update();
	ball_->CheckPlayerCollision(player_);
	if (ball_->CheckTargetCollision()) {
		finished_ = true; // 触发游戏结束
	}
	//
	skydome_->Update();
	player_->Update();
	cameraController_->Update();
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
		camera_.matView = cameraController_->GetViewProjection().matView;
		camera_.matProjection = cameraController_->GetViewProjection().matProjection;
		camera_.TransferMatrix();
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

		for (std::vector<WorldTransform*>& worldTransformTargetLine : worldTransformTarget_) {
		for (WorldTransform* worldTransformTarget : worldTransformTargetLine) {
			    if (!worldTransformTarget)
				continue;
			    modelTarget_->Draw(*worldTransformTarget, camera_);
		}
	}
	ball_->Draw(camera_);
		skydome_->Draw(camera_);
	    player_->Draw(camera_);

	Model::PostDraw();
	  

}
