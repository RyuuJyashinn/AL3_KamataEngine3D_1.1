#include "GameScene.h"
#include "KamataEngine.h"


using namespace KamataEngine;

void GameScene::Initialize() { 
	
	
	player_ = new Player();
	player_->Initialize();


}
void GameScene::Update() { 
	player_->Update();

	ImGui::Begin(" Debug.V1");
	ImGui::InputFloat3("InputFloat3", inputFloat3);
	ImGui::SliderFloat3("InputFloat3", inputFloat3, 0.0f, 1.0f);
	ImGui::Text("LIUJIACHEN %d %d", 2050, 12, 31);
	ImGui::ShowDemoWindow();
	ImGui::End();
}

void GameScene::Draw() { 
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Model::PreDraw(dxCommon->GetCommandList());

	player_->Draw();

	Model::PostDraw();
}
