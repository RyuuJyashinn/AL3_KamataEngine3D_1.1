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

void ChangeScene(Scene scene, GameScene* gameScene,TitleScene* titleScene ) {
	switch (scene) {
	case Scene::kTitle:
		if (titleScene->IsFinished()) {
			// シーン変更
			scene = Scene::kGame;
			// 旧シーンの解放
			delete titleScene;
			titleScene = nullptr;
			// 新シーンの生成と初期化
			gameScene = new GameScene;
			gameScene->Initialize();
		}
		break;
	case Scene::kGame:
		if (gameScene->IsFinished()) {
			scene = Scene::kTitle;
			// 旧シーンの解放
			delete gameScene;
			gameScene = nullptr;
			// 新シーンの生成と初期化
			titleScene = new TitleScene;
			titleScene->Initialize();
		}

		break;
	}


}


void Update(Scene scene, GameScene* gameScene, TitleScene* titleScene) {

	switch (scene) {
	case Scene::kTitle:
		titleScene->Update();
		break;
	case Scene::kGame:
		gameScene->Update();
		break;
	}
}



void Draw(Scene scene, GameScene* gameScene, TitleScene* titleScene) {

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
	
	GameScene* gameScene = nullptr;
	TitleScene* titleScene = nullptr; 
	// ケ-ムシ-ンのインスタンス初期化
	
	Scene scene = Scene::kTitle;
	while (true) {
	// 更新开始
	//监测面板开始
		imguiManager->Begin();
	//Engine 更新
	if (KamataEngine::Update()) {
		break;
	    }

	ChangeScene(scene, gameScene, titleScene);
	//ケ-ムシ-ン更新
	Update(scene, gameScene, titleScene);

    // 监测面板结束
	imguiManager->End( );
	//更新结束
	// 
	// 描画开始
	dxCommon->PreDraw();
	//ケ-ムシ-ン描画
	Draw(scene, gameScene, titleScene);


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
