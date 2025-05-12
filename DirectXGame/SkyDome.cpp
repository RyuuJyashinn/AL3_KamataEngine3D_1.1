#include "SkyDome.h"
using namespace KamataEngine;

void SkyDome::Initialize(Model* model,  Camera* camera) { 
	
	assert(model);
	modelSkyDome_= model;

	worldTransForm_.Initialize();

	camera_ = camera;
	textureHandle_ = textureHandle;
}

void SkyDome::Update() {

	 worldTransForm_.TransferMatrix(); }



void SkyDome::Draw() { 

	modelSkyDome_->Draw(worldTransForm_, *camera_); }