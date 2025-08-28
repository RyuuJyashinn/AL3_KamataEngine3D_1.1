#pragma once
#include"KamataEngine.h"

using namespace KamataEngine;

class TitleScene {
public:
	void Initialize();
	void Update();
	void Draw();
	bool IsFinished() const { return finished_; }


private:
	bool finished_ = false;

	uint32_t textureHandle_ = 0;
	Sprite* sprite_ = nullptr;
};