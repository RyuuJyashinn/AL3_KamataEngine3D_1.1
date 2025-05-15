#pragma once
#include"KamataEngine.h"
using namespace KamataEngine;

class SkyDome {

	public:
	void Initialize();
	void Update();
	void Draw(const Camera& camera);

	private:
	WorldTransform worldTransForm_;
	Model* modelSkyDome_ = nullptr;

	
};
