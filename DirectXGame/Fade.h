#pragma once
#include"KamataEngine.h"

class Fade {
public:
	void Initialize();
	void Update();
	void Draw();
	enum class Status {
		None,FadeIn,FadeOut,


	};
	void Start(Status status, float duration);

private:
	uint32_t textureHandle_ = 0;
	// スプライト
	KamataEngine::Sprite* sprite_ = nullptr;
	Status status_ = Status::None;
	float duration_ = 0.0f;
	float counter_ = 0.0f;
};