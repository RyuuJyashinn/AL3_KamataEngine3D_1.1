#pragma once
#include"KamataEngine.h"
#include "Player.h"
class GameScene {
	
public:
	void Initialize();
	void Update();
	void Draw();

private:
	float inputFloat3[3] = {0, 0, 0};
	Player* player_ = nullptr;
};