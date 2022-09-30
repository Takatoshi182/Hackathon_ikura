//=============================================================================
//
//	プレイヤー処理[player.cpp]
//	Author : SHUGO KURODA
//
//=============================================================================
#include "manager.h"
#include "input_keyboard.h"
#include "input_joypad.h"
#include "sound.h"
#include "renderer.h"

#include "library.h"

#include "bg.h"
#include "bullet.h"
#include "player.h"
#include "explosion.h"
#include "score.h"

#include "rank.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define PLAYER_UI_SIZE		(D3DXVECTOR2(200.0f, 50.0f))
#define LIFE_UI_SIZE		(D3DXVECTOR2(120.0f, 30.0f))
#define LEVEL_UI_SIZE		(D3DXVECTOR2(50.0f, 50.0f))
#define ATTACK_INTERVAL		(7)

//-----------------------------------------------------------------------------
// using宣言
//-----------------------------------------------------------------------------
using namespace LibrarySpace;

//*****************************************************************************
// 定数宣言
//*****************************************************************************
const float CPlayer::SIZE_X = 200.0f;
const float CPlayer::SIZE_Y = 600.0f;
const float CPlayer::MOVE_DEFAULT = 10.0f;

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
// テクスチャのポインタ
LPDIRECT3DTEXTURE9 CPlayer::m_apTexture = { nullptr };

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CPlayer::CPlayer() :
	m_move(0.0f, 0.0f, 0.0f), m_state(STATE_NORMAL), m_nCntState(0), m_nCntAttack(0), m_bControl(false), m_pScore(nullptr), m_bSwing(false)
{
	//オブジェクトの種類設定
	SetObjType(EObject::OBJ_PLAYER);
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CPlayer::~CPlayer()
{
}

//-----------------------------------------------------------------------------
// インスタンス生成処理
//-----------------------------------------------------------------------------
CPlayer *CPlayer::Create(const D3DXVECTOR3& pos, const int& nNum)
{
	//インスタンス生成
	CPlayer *pPlayer = new CPlayer;

	if (pPlayer != nullptr)
	{
		// 位置設定
		pPlayer->SetPosition(D3DXVECTOR3(pos));
		// 生成処理
		pPlayer->Init();
		// テクスチャの設定
		pPlayer->BindTexture(m_apTexture);
	}

	return pPlayer;
}

//-----------------------------------------------------------------------------
// テクスチャの読み込み
//-----------------------------------------------------------------------------
HRESULT CPlayer::Load()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/PutterClub000.png",
		&m_apTexture);

	return S_OK;
}

//-----------------------------------------------------------------------------
//	テクスチャの削除
//-----------------------------------------------------------------------------
void CPlayer::Unload()
{
	// テクスチャの破棄
	if (m_apTexture != nullptr)
	{
		m_apTexture->Release();
		m_apTexture = nullptr;
	}
}

//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
HRESULT CPlayer::Init()
{
	// 登場状態にする
	m_state = STATE_ENTRY;

	// サイズの設定
	CObject2D::SetSize(D3DXVECTOR2(SIZE_X, SIZE_Y));
	// 初期化
	CObject2D::Init();

	m_bControl = true;

	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void CPlayer::Uninit()
{
	CObject2D::Uninit();
}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void CPlayer::Update()
{
	// 位置情報を取得
	D3DXVECTOR3 pos = CObject2D::GetPosition();

	// キーボード情報の取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	// 位置情報を取得
	float rot = CObject2D::GetRot();

	// 
	if (pKeyboard->GetPress(CInputKeyboard::KEYINFO_ATTACK) == true && m_bControl == true)
	{//攻撃キー押下 && 操作可能状態なら

		// 振りかぶっている状態にする
		m_bSwing = true;

		rot += 0.01f;

		//攻撃カウンターの加算
		m_nCntAttack++;

		if (m_nCntAttack > ATTACK_INTERVAL)
		{
			//攻撃カウンターをリセット
			m_nCntAttack = 0;

			//サウンド再生
			CSound::Play(CSound::SOUND_LABEL_SE_SHOT);
		}
	}
	if (m_bSwing == true)
	{
		// 振りかぶっている状態にする
		m_bSwing = true;
		m_bControl = false;
	}

	// 回転率の更新
	SetRot(rot);

	//位置情報更新
	CObject2D::SetPosition(pos);

	//状態管理
	State();

	//頂点座標の設定
	CObject2D::SetVertex();
}

//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void CPlayer::Draw()
{
	//描画
	CObject2D::Draw();
}

//-----------------------------------------------------------------------------
// 状態管理
//-----------------------------------------------------------------------------
void CPlayer::State()
{
	switch (m_state)
	{
	case CPlayer::STATE_NORMAL:
		break;
	case CPlayer::STATE_ENTRY:
		break;
	case CPlayer::STATE_RESPAWN:
		break;
	case CPlayer::STATE_DIE:
		break;
	default:
		break;
	}
}
