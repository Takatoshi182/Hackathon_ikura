//===============================================================================================
//
// ゲーム画面処理 [title.cpp]
// Author : SHUGO KURODA
//
//===============================================================================================
#include "title.h"
#include "sound.h"

#include "manager.h"
#include "input_keyboard.h"
#include "input_joypad.h"
#include "object.h"
#include "object2D.h"
#include "library.h"
#include "fade.h"

#include "cloud.h"
#include "fade_scene.h"
#include "library.h"

//-----------------------------------------------------------------------------------------------
// using宣言
//-----------------------------------------------------------------------------------------------
using namespace LibrarySpace;

//-----------------------------------------------------------------------------------------------
// 静的メンバ変数
//-----------------------------------------------------------------------------------------------
// テクスチャのポインタ
LPDIRECT3DTEXTURE9 CTitle::m_apTexture[OBJ_MAX] = { nullptr };

//-----------------------------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------------------------
CTitle::CTitle() :m_nCountMoveBg(0), m_bTitleDraw(false), m_bPressFade(false), m_nCntLoop(0), m_bEntry{ false }, m_bPush(false), m_move(0.0f,0.0f,0.0f)
{
	for (int nCnt = 0; nCnt < OBJ_MAX - 1; nCnt++)
	{
		m_apObject2D[nCnt] = nullptr;
	}

	//雲の生成情報を初期化
	ZeroMemory(&m_CloudInfo, sizeof(m_CloudInfo));
}

//-----------------------------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------------------------
CTitle::~CTitle()
{
}

//-----------------------------------------------------------------------------------------------
// テクスチャの読み込み処理
//-----------------------------------------------------------------------------------------------
HRESULT CTitle::Load()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title000.jpg", &m_apTexture[BG_SKY]);			// 背景の空
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title001.png", &m_apTexture[LOGO_TITLE]);		// タイトルロゴ
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title002.png", &m_apTexture[LOGO_PLAYER]);		// プレイヤーロゴ
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title004.png", &m_apTexture[LOGO_PRESS]);		// PRESSロゴ
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tutorial000.png", &m_apTexture[LOGO_TUTORIAL]);// チュートリアル
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title003.png", &m_apTexture[LOGO_SHADOW]);		// プレイヤーロゴ(黒)

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// テクスチャの破棄
//-----------------------------------------------------------------------------------------------
void CTitle::Unload()
{
	for (int nCnt = 0; nCnt < OBJ_MAX; nCnt++)
	{
		// テクスチャの破棄
		if (m_apTexture[nCnt] != nullptr)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = nullptr;
		}
	}
}

//-----------------------------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------------------------
HRESULT CTitle::Init()
{
	// テクスチャのロード
	CTitle::Load();

	// スクリーンサイズの保存
	D3DXVECTOR2 ScreenSize = D3DXVECTOR2((float)CRenderer::SCREEN_WIDTH, (float)CRenderer::SCREEN_HEIGHT);

	for (int nCnt = 0; nCnt < OBJ_MAX - 1; nCnt++)
	{// 生成
		m_apObject2D[nCnt] = new CObject2D;
	}

	// 空
	m_apObject2D[BG_SKY]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, -ScreenSize.y / 2, 0.0f));
	m_apObject2D[BG_SKY]->SetSize(D3DXVECTOR2(ScreenSize.x, ScreenSize.y * 3));
	// タイトルロゴ
	m_apObject2D[LOGO_TITLE]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, ScreenSize.y - 450.0f, 0.0f));
	m_apObject2D[LOGO_TITLE]->SetSize(D3DXVECTOR2(ScreenSize.x - 550.0f, ScreenSize.y / 2.5f));
	// プレイヤーロゴ
	m_apObject2D[LOGO_PLAYER]->SetPosition(D3DXVECTOR3(-ScreenSize.x, 160.0f, 0.0f));
	m_apObject2D[LOGO_PLAYER]->SetSize(D3DXVECTOR2(300.0f, 150.0f));
	// Pressロゴ
	m_apObject2D[LOGO_PRESS]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, ScreenSize.y - 150.0f, 0.0f));
	m_apObject2D[LOGO_PRESS]->SetSize(D3DXVECTOR2(ScreenSize.x - 550.0f, 90.0f));
	// フェード
	m_apObject2D[BG_FADE]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, ScreenSize.y / 2, 0.0f));
	m_apObject2D[BG_FADE]->SetSize(D3DXVECTOR2(ScreenSize.x, ScreenSize.y));
	// チュートリアル
	m_apObject2D[LOGO_TUTORIAL]->SetPosition(D3DXVECTOR3(ScreenSize.x / 2, ScreenSize.y / 2, 0.0f));
	m_apObject2D[LOGO_TUTORIAL]->SetSize(D3DXVECTOR2(ScreenSize.x, ScreenSize.y));

	for (int nCnt = 0; nCnt < OBJ_MAX - 1; nCnt++)
	{// 初期化とテクスチャの設定
		m_apObject2D[nCnt]->Init();
		m_apObject2D[nCnt]->BindTexture(m_apTexture[nCnt]);
	}

	//背景とプレイヤーロゴ以外を前に描画する(タイプを設定する)
	for (int nCnt = LOGO_TITLE; nCnt < OBJ_MAX - 1; nCnt++)
	{
		m_apObject2D[nCnt]->SetObjType(CObject::OBJ_TITLE);
	}
	//プレイヤーロゴを背景の次に描画する
	m_apObject2D[LOGO_PLAYER]->SetObjType(CObject::OBJ_TITLE_LOGO);
	m_apObject2D[BG_FADE]->SetObjType(CObject::OBJ_BLACK);
	m_apObject2D[LOGO_TUTORIAL]->SetObjType(CObject::OBJ_LOGO);

	//タイトルとPressロゴを透明にする
	m_apObject2D[LOGO_TITLE]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	m_apObject2D[LOGO_PRESS]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	m_apObject2D[BG_FADE]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	m_apObject2D[LOGO_TUTORIAL]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	// タイトルBGM
	CSound::Play(CSound::SOUND_LABEL_TITLE);

	return S_OK;
}

//-----------------------------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------------------------
void CTitle::Uninit()
{
	// ポインタの破棄
	for (int nCnt = 0; nCnt < OBJ_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != nullptr)
		{
			m_apObject2D[nCnt] = nullptr;
		}
	}

	//タイトルテクスチャの破棄
	CTitle::Unload();

	// タイトルBGM
	CSound::Stop();
}

//-----------------------------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------------------------
void CTitle::Update()
{
	// キーボード情報の取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_ATTACK) == true)
	{//攻撃キー押下
		if (m_bPush == false)
		{
			m_bPush = true;

			// モードの設定
			CManager::GetFade()->SetFade(CFade::FADE_OUT, CManager::MODE::MODE_GAME);
		}
	}

	if (m_bPush == false)
	{
		// タイトル画面をループさせるまでの時間
		m_nCntLoop++;

		// 50秒経過でタイトル画面リセット
		if (m_nCntLoop >= 5000)
		{
			//カウンターリセット
			m_nCntLoop = 0;

			// モードの設定
			CManager::GetFade()->SetFade(CFade::FADE_OUT, CManager::MODE::MODE_TITLE);
		}
	}
}
