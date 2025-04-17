#include "GameScene.h"
#include "KamataEngine.h"


using namespace KamataEngine;

void GameScene::Initialize() { 
	worldTransform_.Initialize();
	camera_.Initialize();
	soundDateHandle_ = Audio::GetInstance()->LoadWave("mokugyo.wav");
	Audio::GetInstance()->PlayWave(soundDateHandle_);

	textureHandle_ = TextureManager::Load("uvChecker.png");
	//delete sprite_;
	model_ = Model::Create();
	// delete model_;

	PrimitiveDrawer::GetInstance()->SetCamera(&camera_);

	debugCamera_ = new DebugCamera(1280,720);
	// delete debugCamera_;
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetCamera(&debugCamera_->GetCamera());
}
void GameScene::Update() { 
	ImGui::Begin(" Debug.V1");
	debugCamera_->Update();
	ImGui::InputFloat3("InputFloat3",inputFloat3);
	ImGui::SliderFloat3("InputFloat3", inputFloat3,0.0f,1.0f);
	ImGui::Text("LIUJIACHEN %d %d",2050,12,31);
	ImGui::ShowDemoWindow();
	ImGui::End();
}

void GameScene::Draw() { 
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Model::PreDraw(dxCommon->GetCommandList());
	model_->Draw(worldTransform_, debugCamera_->GetCamera(), textureHandle_);
	Model::PostDraw();


}
