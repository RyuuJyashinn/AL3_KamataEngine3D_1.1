#pragma once
#include"KamataEngine.h"
#include"Player.h"
using namespace KamataEngine;
using namespace MathUtility;
class Player;
class CameraController {
	public:
	void Initialize();
	void Update();
	void setTarget(Player* target) { target_ = target; }
	void Reset();
	const KamataEngine::Camera& GetViewProjection() const { return camera_; }

	private:
	Vector3 targetOffset_ = {0, 0, -15.0f};
	Camera camera_;
    Player* target_ = nullptr;
};