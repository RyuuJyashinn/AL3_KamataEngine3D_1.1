#pragma once
#include"KamataEngine.h"
using namespace KamataEngine;

class SkyDome {

	public:
	void Initialize(Model* model,  Camera* camera);
	void Update();
	void Draw();

	private:
	WorldTransform worldTransForm_;
	    Model* modelSkyDome_ = nullptr;
	Camera* camera_ = nullptr;
	
};
