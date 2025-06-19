#pragma once
#include"KamataEngine.h"
#include"Player.h"
#include <algorithm>
using namespace KamataEngine;
class Player;


class CameraController {
	public:
	void Initialize();
	void Update();
	void setTarget(Player* target) { target_ = target; }
	void Reset();
	const KamataEngine::Camera& GetViewProjection() const { return camera_; }
	struct Rect {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};
	void SetMovableArea(Rect area) { movableArea_ = area; }


	private:
	Vector3 targetOffset_ = {0, 0, -15.0f};
	Vector3 targetOnset_;
	Camera camera_;
    Player* target_ = nullptr;
	Rect movableArea_ = {0, 100, 0, 100};
	static inline const Rect margin_ = {-9.0f, 9.0f, -5.0f, 5.0f};
	static inline const float kInterpolatetionRate = 1.0f;
	static inline const float kVelocityBias = 8.0f;
};