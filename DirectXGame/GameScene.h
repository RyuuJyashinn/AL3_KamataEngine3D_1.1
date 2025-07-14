#pragma once
#include"KamataEngine.h"
#include"Player.h"
#include"Enemy.h"
#include<vector> 
#include"SkyDome.h"
#include"MapChipField.h"
#include"CameraController.h"
class GameScene {
public:
	KamataEngine::Sprite* sprite_ = nullptr;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera camera_;
	KamataEngine::DebugCamera* debugCamera_ = nullptr;
	float inputFloat3[3] = {0, 0, 0};

	public:
	void Initialize();
	void Update();
	void Draw();
	//~GameScene();
	void CheckAllCollison();

private:
	bool isDebugCameraActive_ = false;
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;	
    Model* modelBlock_ = nullptr;
	uint32_t textureHandle_ = 0;
	uint32_t soundDateHandle_ = 0;
	Player* player_ = nullptr;
	Model* modelPlayer_ = nullptr;

	Model* modelEnemy_ = nullptr;
	SkyDome* skydome_ = nullptr;
	MapChipField* mapChipField_;
	CameraController* cameraController_ = nullptr;

	std::list<Enemy*> enemies_;
	uint32_t enemyNum_=3;
};