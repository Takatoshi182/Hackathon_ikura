//===================================================================
//
//	プレイヤーの処理[player.h]
//	Author:SHUGO KURODA
//
//===================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "object2D.h"

//*******************************************************************
//	前方宣言
//*******************************************************************
class CScore;

//*******************************************************************
//	プレイヤークラスの定義
//*******************************************************************
class CPlayer : public CObject2D
{
private:
	//プレイヤーのサイズ(X)
	static const float SIZE_X;
	// プレイヤーのサイズ(Y)
	static const float SIZE_Y;
	// プレイヤーの基本移動量
	static const float MOVE_DEFAULT;

public:

	//状態
	enum STATE
	{
		STATE_NORMAL = 0,	//通常
		STATE_ENTRY,		//登場
		STATE_RESPAWN,		//無敵(リスポーン)状態
		STATE_DIE,			//死亡状態
		STATE_MAX
	};

	//アニメーション番号
	enum ANIMTYPE
	{
		TYPE_NEUTRAL = 0,
		TYPE_UP,
		TYPE_DOWN,
		TYPE_MAX
	};

	CPlayer();
	~CPlayer() override;

	//メンバ関数
	static CPlayer *Create(const D3DXVECTOR3& pos,const int& nNum);	//インスタンス生成処理
	static HRESULT Load();		// テクスチャの読み込み
	static void Unload();		// テクスチャの削除

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	STATE GetState() { return m_state; }
	// スコア情報の取得
	CScore *GetScore() { return m_pScore; }

	void State();

private:	//メンバ変数
	// テクスチャのポインタ
	static LPDIRECT3DTEXTURE9 m_apTexture;

	// スコアのポインタ
	CScore *m_pScore;

	//移動量
	D3DXVECTOR3 m_move;
	//状態
	STATE m_state;
	//状態カウンター
	int m_nCntState;
	//攻撃カウンター
	int m_nCntAttack;
	//プレイヤーが操作できるかどうか
	bool m_bControl;
	//クラブを振りかぶっているかどうか
	bool m_bSwing;
};

#endif //_PLAYER_H_