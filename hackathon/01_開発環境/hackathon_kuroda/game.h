//=============================================================================
//
// ゲーム画面処理 [game.h]
// Author : SHUGO KURODA
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "base.h"
#include "enemy_data.h"
#include "cloud_data.h"

#include "player.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScore;
class CMeshField;

//-----------------------------------------------------------------------------
// ベースクラス(派生クラス)
//-----------------------------------------------------------------------------
class CGame : public CBase
{
private:	//定数
	static const int MAX_POS_CLOUD = 250;
	static const int MIN_POS_CLOUD = 180;

public:		//メンバ関数
	CGame();
	~CGame() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

	// 雲を生成
	void CreateCloud();
	// ロゴを生成
	void CreateLogo(int nCounter);

	// 雲の生成状態を設定
	static void SetCreateCloud(bool bCreate) { m_bCreateCloud = bCreate; }
	// プレイヤー情報の取得
	static CPlayer *GetPlayer(int nNum) { return m_pPlayer; }
	// スコア情報の取得
	static CMeshField *GetMeshField() { return m_pMeshField; }
	// 泡の生成状態を取得
	static bool GetBubble() { return m_bCreateCloud; }
	// プレイヤーのスコアをランキングに設定
	static void SetPlayerScore();

private:
	// テクスチャ読み込み
	static void LoadAll();
	// テクスチャ破棄
	static void UnloadAll();

private:	//メンバ変数
	// プレイヤー情報
	static CPlayer *m_pPlayer;
	static CMeshField *m_pMeshField;

	// 雲を生成するかどうか
	static bool m_bCreateCloud;

	// 雲の生成情報
	CloudInfo m_CloudInfo;
	// 雲が生成されるまでの時間(乱数)
	int m_nRandBubble;
	// 雲が生成されるまでの時間カウンター
	int m_nCntBubble;
};

#endif	//_GAME_H_
