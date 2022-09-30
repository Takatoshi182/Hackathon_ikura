//===============================================================================================
//
// ゲーム画面処理 [game.cpp]
// Author : SHUGO KURODA
//
//===============================================================================================
#include "manager.h"
#include "game.h"
#include "object.h"
#include "sound.h"

#include "library.h"
#include "load.h"
#include "fade_scene.h"
#include "input_keyboard.h"
#include "input_joypad.h"

#include "score.h"
#include "number.h"
#include "logo.h"
#include "pause.h"
#include "rank.h"
#include "score.h"

#include "cloud.h"
#include "enemy.h"
#include "enemy_boss.h"
#include "bullet.h"
#include "bullet_option.h"
#include "barrier.h"
#include "explosion.h"
#include "bubble.h"
#include "effect.h"
#include "bg_move.h"

//-----------------------------------------------------------------------------------------------
// using宣言
//-----------------------------------------------------------------------------------------------
using namespace LibrarySpace;

//-----------------------------------------------------------------------------------------------
// 静的メンバ変数
//-----------------------------------------------------------------------------------------------
bool CGame::m_bCreateCloud = true; 
CPlayer *CGame::m_pPlayer = {};
CMeshField *CGame::m_pMeshField = nullptr;

//-----------------------------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------------------------
CGame::CGame() :m_nCntBubble(0), m_nRandBubble(0)
{
	//雲の生成情報を初期化
	ZeroMemory(&m_CloudInfo, sizeof(m_CloudInfo));
}

//-----------------------------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------------------------
CGame::~CGame()
{
}

//-----------------------------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------------------------
HRESULT CGame::Init()
{
	// テクスチャ読み込み
	LoadAll();

	// プレイヤー生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(CRenderer::SCREEN_WIDTH - 300.0f, CRenderer::SCREEN_HEIGHT / 2, 0.0f), 0);

	//ポインタの初期化
	m_pMeshField = nullptr;
	m_bCreateCloud = true;

	// 決定音
	CSound::Play(CSound::SOUND_LABEL_GAME);

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------------------------
void CGame::Uninit()
{
	//テクスチャの破棄
	UnloadAll();

	// ポーズ状態を解除
	CManager::SetPause(false);

	// 決定音
	CSound::Stop();
}

//-----------------------------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------------------------
void CGame::Update()
{
	// キーボード情報の取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	// ゲームパッド情報の取得
	CInputJoypad *pJoypad = CManager::GetInputJoypad();

	if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_PAUSE) == true || pJoypad->GetTrigger(CInputJoypad::JOYKEY_START, 0) == true)
	{
		// ポーズ状態の取得
		bool bPause = CManager::GetPause();
		// ポーズ中でなければ
		if (bPause == false)
		{//ポーズ画面を生成
			CPause::Create(0);
		}
	}

	////雲を生成するかどうか
	//if (m_bCreateCloud == true)
	//{
	//	//雲の生成処理
	//	CreateCloud();
	//}
}

//-----------------------------------------------------------------------------------------------
// ロゴの生成
//-----------------------------------------------------------------------------------------------
void CGame::CreateLogo(int nCounter)
{
	if (nCounter == 4800)
	{
		// ボス接近中のロゴ
		CLogo::Create(D3DXVECTOR3(CRenderer::SCREEN_WIDTH / 2, 300.0f, 0.0f), D3DXVECTOR2(CRenderer::SCREEN_WIDTH - 400.0f, 90.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, CLogo::TYPE_WARNING, CLogo::ANIM_LENGTHWISE, 420);

		// ゲームBGMをストップ
		CSound::Stop(CSound::SOUND_LABEL_GAME);
		// 警告音
		CSound::Play(CSound::SOUND_LABEL_SE_WARNING);
	}

	if (nCounter == 4920)
	{
		// ボス接近中の説明ロゴ
		CLogo::Create(D3DXVECTOR3(CRenderer::SCREEN_WIDTH / 2, 500.0f, 0.0f), D3DXVECTOR2(CRenderer::SCREEN_WIDTH - 760.0f, 270.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, CLogo::TYPE_WARNING_SUB, CLogo::ANIM_HORIZONTALLY, 300);
	}

	if (nCounter == 4800)
	{
		// ボス接近時の薄暗いフェード
		CFadeScene::Create(CFadeScene::TYPE_BLACK);
	}
}

//-----------------------------------------------------------------------------------------------
// ランキングスコアの設定
//-----------------------------------------------------------------------------------------------
void CGame::SetPlayerScore()
{
	// プレイヤーのスコアを保存
	if (m_pPlayer != nullptr)
	{
		// プレイヤースコアの初期化
		CRank::SetScore(0);

		CScore* pScore = m_pPlayer->GetScore();

		if (pScore != nullptr)
		{
			int nSocre = pScore->GetScore();
			CRank::SetScore(nSocre);
		}
	}
}

//-----------------------------------------------------------------------------------------------
// テクスチャ読み込み
//-----------------------------------------------------------------------------------------------
void CGame::LoadAll()
{
	// プレイヤー
	CPlayer::Load();
	// 爆発
	CExplosion::Load();
	// スコア
	CNumber::Load();
	// ロゴ
	CLogo::Load();
	// ポーズ画面
	CPause::Load();
}

//-----------------------------------------------------------------------------------------------
// テクスチャ破棄　
//-----------------------------------------------------------------------------------------------
void CGame::UnloadAll()
{
	// プレイヤー
	CPlayer::Unload();
	// 爆発
	CExplosion::Unload();
	// スコア
	CNumber::Unload();
	// ロゴ
	CLogo::Unload();
	// ポーズ画面
	CPause::Unload();
}
