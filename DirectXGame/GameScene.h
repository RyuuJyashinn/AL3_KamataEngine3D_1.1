#pragma once
#include"KamataEngine.h"
#include"Player.h"
#include<vector> 
#include"SkyDome.h"
#include"MapChipField.h"
#include"CameraController.h"
#include"Ball.h"
class GameScene {
public:
	KamataEngine::Sprite* sprite_ = nullptr;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera camera_;
	KamataEngine::DebugCamera* debugCamera_ = nullptr;
	float inputFloat3[3] = {0, 0, 0};
	void Initialize();
	void Update();
	void Draw();

	enum class Phase {
		kPlay,
		kTitle,
	};
	bool IsFinished() const { return finished_; }


private:
	bool isDebugCameraActive_ = false;
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;	
    Model* modelBlock_ = nullptr;
	uint32_t textureHandle_ = 0;
	uint32_t soundDateHandle_ = 0;
	Player* player_ = nullptr;
	Model* modelPlayer_ = nullptr;
	SkyDome* skydome_ = nullptr;
	MapChipField* mapChipField_;

	std::vector<std::vector<WorldTransform*>> worldTransformTarget_;	
	MapChipField* mapChipTarget_;
	Model* modelTarget_ = nullptr;

	CameraController* cameraController_ = nullptr;

	Ball* ball_ = nullptr;
	Model* modelBall_ = nullptr;

	Phase phase_;
	bool finished_ = false;
};