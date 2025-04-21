#include "GameScene.h"
#include "KamataEngine.h"


using namespace KamataEngine;

void GameScene::Initialize() {
	worldTransform_.Initialize();
	camera_.Initialize();
	model_ = Model::Create();
	PrimitiveDrawer::GetInstance()->SetCamera(&camera_);
	textureHandle_ = TextureManager::Load("uvChecker.png");

	player_ = new Player();
	player_->Initialize(model_, textureHandle_, &camera_);


}
void GameScene::Update() { 
	player_->Update();

}

void GameScene::Draw() { 
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Model::PreDraw(dxCommon->GetCommandList());
	
	player_->Draw();

	Model::PostDraw();


}
