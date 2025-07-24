#include "GameScene.h"
#include "KamataEngine.h"
#include"MyMath.h"

using namespace KamataEngine;
void GameScene::Initialize() {
	worldTransform_.Initialize();
	camera_.Initialize();

	modelBlock_ = Model::CreateFromOBJ("block");
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

	//
	player_ = new Player();
	player_->setMapChipField(mapChipField_);//2.7
	modelPlayer_ = Model::CreateFromOBJ("player");
	Vector3 playerPostion = mapChipField_->GetMapChipPositionByIndex(1, 18);
	player_->Initialize(modelPlayer_, &camera_, playerPostion);
	// DeathParticles
	deathParticles_ = nullptr; 
	//deathParticles_ = new DeathParticles;
	modelDeath_ = Model::CreateFromOBJ("deathParticle");
	//deathParticles_->Initialize(modelDeath_, &camera_, playerPostion);
	//敌
	modelEnemy_ = Model::CreateFromOBJ("enemy");
	for (uint32_t i = 0; i < enemyNum_;++i) {
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(5-i, 18-i);
		newEnemy->Initialize(modelEnemy_, &camera_, enemyPosition);
		enemies_.push_back(newEnemy);
	}
	//
	phase_ = Phase::kPlay;

	// camera contro
	cameraController_ = new CameraController();
	cameraController_->Initialize();
	cameraController_->setTarget(player_);
	cameraController_->Reset();

	CameraController::Rect cameraArea = {10.0f, 100-12.0f ,5.0f,5.0f};
	cameraController_->SetMovableArea(cameraArea);
	//
}

void GameScene::CheckAllCollison() {
	#pragma region
	{
		AABB aabb1, aabb2;
		aabb1 = player_->GetAABB();

		for (Enemy* enemy : enemies_) {

			aabb2 = enemy->GetAABB();
			if (IsCollision(aabb1, aabb2)) {
				player_->OnCollision(enemy);
				enemy->OnCollision(player_);
			}
		}

	}
	

    #pragma endregion

 }

void GameScene::ChangePhase() {
	switch (phase_) {
	case Phase::kPlay:
		if (player_->IsDead()) {
			phase_ = Phase::kDeath;
			const Vector3& deathParticlesPosition=player_->GetWorldPosition();
			deathParticles_ = new DeathParticles();
			deathParticles_->Initialize(modelDeath_, &camera_, deathParticlesPosition);
		}
		break;
	case Phase::kDeath:
	
		break;
	}
}


void GameScene::Update() { 
	
	switch (phase_) {
	case Phase::kPlay:
		//
		skydome_->Update();
		//
		player_->Update();
		//
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}
		//
		cameraController_->Update();
		//
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
		//
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;

				worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
				worldTransformBlock->TransferMatrix();
			}
		}
		//
		CheckAllCollison();
		//
		ChangePhase();
		//
		break;
	case Phase::kDeath:
		//
		skydome_->Update();
		//
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}
		//
		if (deathParticles_ != nullptr) {

			deathParticles_->Update();
		}
		//
		cameraController_->Update();
		//
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
		//
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;

				worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
				worldTransformBlock->TransferMatrix();
			}
		}
		//
		ChangePhase();
		break;
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

		skydome_->Draw(camera_);
	if (player_->IsDead() != true) {
		    player_->Draw(camera_);
	}
	   

	    if (deathParticles_ != nullptr) {

		    deathParticles_->Draw(camera_);
	    }
	    for (Enemy* enemy : enemies_) { 
			enemy->Draw(camera_);
	    }
	Model::PostDraw();
	  

}


//GameScene::~GameScene() {
//	/*for (Enemy* enemy : enemies_) {
//		delete enemy;
//	}*/
//
//}
