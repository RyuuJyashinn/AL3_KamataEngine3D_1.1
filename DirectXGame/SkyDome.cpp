#include "SkyDome.h"
using namespace KamataEngine;

void SkyDome::Initialize() { 
	worldTransForm_.Initialize();
	modelSkyDome_ = Model::CreateFromOBJ("skydome", true);

}

void SkyDome::Update() {

	 worldTransForm_.TransferMatrix(); }



void SkyDome::Draw(const Camera& camera) {
	if (modelSkyDome_) {
		modelSkyDome_->Draw(worldTransForm_, camera);
	}
}