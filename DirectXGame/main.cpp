#include <Windows.h>
#include"KamataEngine.h"
#include"GameScene.h"
#include"TitleScene.h"
using namespace KamataEngine;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	//Engine 初期化
	KamataEngine::Initialize(L"GC2C_13_リュウ_ジャシン");
	//Debug Sheet
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();


	//DXInstance取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	//ケ-ムシ-ンのインスタンス生成
	GameScene* gameScene = new GameScene();
	TitleScene* titleScene = nullptr; 
	// ケ-ムシ-ンのインスタンス初期化
	gameScene->Initialize();


	while (true) {
	// 更新开始
	//监测面板开始
		imguiManager->Begin();
	//Engine 更新
	if (KamataEngine::Update()) {
		break;
	    }
	//ケ-ムシ-ン更新
	gameScene->Update();
    // 监测面板结束
	imguiManager->End( );
	//更新结束
	// 
	// 描画开始
	dxCommon->PreDraw();
	//ケ-ムシ-ン描画
	gameScene->Draw();
	AxisIndicator::GetInstance()->Draw();
	//监测面板 画面
	imguiManager->Draw();
	// 描画终了
	dxCommon->PostDraw();
	}
	delete gameScene;
	gameScene = nullptr;
    //Engine 终了
	KamataEngine::Finalize();
	return 0;
}
