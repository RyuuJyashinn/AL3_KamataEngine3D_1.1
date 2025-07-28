#include <Windows.h>
#include"KamataEngine.h"
#include"GameScene.h"
#include"TitleScene.h"
using namespace KamataEngine;



enum class Scene {
	kUnknown=0,
	kTitle,
	kGame,


};
Scene scene = Scene::kUnknown;
GameScene* gameScene = nullptr;
TitleScene* titleScene = nullptr;

void ChangeScene() {
	switch (scene) {
	case Scene::kTitle:
		if (titleScene && titleScene->IsFinished()) {
			delete titleScene; 
			GameScene* newGameScene = new GameScene();
			newGameScene->Initialize();

			gameScene = newGameScene;
			titleScene = nullptr;    
			scene = Scene::kGame;
		}
		break;

	case Scene::kGame:

		if (gameScene && gameScene->IsFinished()) {
			delete gameScene;

			TitleScene* newTitleScene = new TitleScene();
			newTitleScene->Initialize();

			titleScene = newTitleScene;
			gameScene = nullptr;
			scene = Scene::kTitle;
		}
		break;
	}
}

void UpdateScene() {

	switch (scene) {
	case Scene::kTitle:
		titleScene->Update();
		break;
	case Scene::kGame:
		gameScene->Update();
		break;
	}
}



void DrawScene() {

	switch (scene) {
	case Scene::kTitle:
		titleScene->Draw();
		break;
	case Scene::kGame:
		gameScene->Draw();
		break;
	}
}
    // Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	//Engine 初期化
	KamataEngine::Initialize(L"GC2C_13_リュウ_ジャシン");
	//Debug Sheet
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();


	//DXInstance取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	//ケ-ムシ-ンのインスタンス生成
	// 
	// ケ-ムシ-ンのインスタンス初期化
	scene = Scene::kTitle;
	titleScene = new TitleScene;
	titleScene->Initialize();
	
	while (true) {
	// 更新开始
	//监测面板开始
		imguiManager->Begin();
	//Engine 更新
	if (KamataEngine::Update()) {
		break;
	    }

	ChangeScene();
	//ケ-ムシ-ン更新
	UpdateScene();

    // 监测面板结束
	imguiManager->End( );
	//更新结束
	// 
	// 描画开始
	dxCommon->PreDraw();
	//ケ-ムシ-ン描画
	DrawScene();


	AxisIndicator::GetInstance()->Draw();
	//监测面板 画面
	imguiManager->Draw();
	// 描画终了
	dxCommon->PostDraw();
	}
	
    //Engine 终了
	KamataEngine::Finalize();
	return 0;
}
