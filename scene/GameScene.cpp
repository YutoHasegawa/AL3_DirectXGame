#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() { delete model_; }

void GameScene::Initialize() {
	//ここにシーン初期化処理

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	//描画初期化
	textureHandle_ = TextureManager::Load("mario.jpg");
	model_ = Model::Create();
	// x,y,z方向のスケーリング
	worldTransform_.scale_ = {5.0f, 5.0f, 5.0f};
	// x,y,z軸周りの回転角を設定
	//またXM_PIは180度、XM/PI/4.0fは45度
	//また、XMConvertToRadians(45.0f)でも45度となる
	worldTransform_.rotation_ = {XM_PI / 4.0f, XM_PI / 4.0f, 0.0f};
	//x,y,z軸周りの平行移動
	worldTransform_.translation_ = {10.0f, 10.0f, 10.0f};
	// 3D描画初期化
	worldTransform_.Initialize();
	viewProjection_.Initialize();
}

void GameScene::Update() {
	//ここにシーンの更新処理
	debugText_->SetPos(50,50);
	debugText_->Printf("translation:(%f, %f, %f)", worldTransform_.translation_.z, worldTransform_.translation_.y, worldTransform_.translation_.z);
	debugText_->SetPos(50,70);
	debugText_->Printf("rotation:(%f, %f, %f)", worldTransform_.rotation_.x, worldTransform_.rotation_.y, worldTransform_.rotation_.z);
	debugText_->SetPos(50,90);
	debugText_->Printf("scale:(%f, %f, %f)", worldTransform_.scale_.x, worldTransform_.scale_.y, worldTransform_.scale_.z);
}

void GameScene::Draw() {
	//ここに描画処理

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
