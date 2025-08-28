#include"TitleScene.h"
#include "KamataEngine.h"
#include"MyMath.h"
#include"assert.h"
using namespace KamataEngine;

void TitleScene::Initialize() {

	textureHandle_ = TextureManager::Load("title.png");
	sprite_ = Sprite::Create(textureHandle_, {0, 0});



}

void TitleScene::Update() {
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
	
	finished_ = true;
	}

}

void TitleScene::Draw() { 
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	ID3D12GraphicsCommandList* commandList = dxCommon->GetCommandList();

	// 传入命令列表参数
	Sprite::PreDraw(commandList);
	sprite_->Draw();
	Sprite::PostDraw();
}