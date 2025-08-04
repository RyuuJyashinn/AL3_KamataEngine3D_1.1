#include"Fade.h"
#include "KamataEngine.h"
#include <algorithm>
using namespace KamataEngine;
void Fade::Initialize() { 
	
	textureHandle_ = TextureManager::Load("white1x1.png");
	sprite_ = Sprite::Create(textureHandle_, {0, 0});
	sprite_->SetSize(Vector2(1280, 720));
	// 画面サイズ
	sprite_->SetColor(Vector4(0, 0, 0, 1));

}
void Fade::Start(Status status, float duration) {

	status_ = status;
	duration = duration;
	counter_ = 0.0f;

}


void Fade::Update() {

	switch (status_) {
	case Status::None:
		// 何もしない
		break;
	case Status::FadeIn:
		counter_ += 1.0f / 60.0f;
		// フェード継続時間に達したら打ち止め
		if (counter_ >= duration_) {
			counter_ = duration_;
		}
		sprite_->SetColor(Vector4(0, 0, 0,1.0f- std::clamp(counter_ / duration_, 0.0f, 1.0f)));
		break;
	case Status::FadeOut:
		counter_ += 1.0f / 60.0f;
		// フェード継続時間に達したら打ち止め
		if (counter_ >= duration_) {
			counter_ = duration_;
		}
		sprite_->SetColor(Vector4(0, 0, 0, std::clamp(counter_ / duration_, 0.0f, 1.0f)));
		break;
	}
}
void Fade::Draw() {

DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Sprite::PreDraw(dxCommon->GetCommandList());
	sprite_->Draw();
	Sprite::PostDraw();

}