//=============================================================================
//
// ランク処理 [rank.h]
// Author : SHUGO KURODA
//
//=============================================================================
#ifndef _RANK_H_
#define _RANK_H_

#include "object2D.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScore;

//*****************************************************************************
// ランキングクラス(派生クラス)
//*****************************************************************************
class CRank : public CObject
{
public:
	// 最大ランキングスコア数
	static const int MAX_RANKING = 5;
	// 最大プレイヤースコア数
	static const int MAX_PLAYER_SCORE = 1;

	enum TYPE
	{//背景の種類
		TYPE_YOU_SCORE = 0,	// あなたのスコアUI
		TYPE_BG,			// 背景
		TYPE_RANK,			// ランキングUI
		TYPE_MAX			// 背景の最大数
	};

	CRank();
	~CRank();

	static CRank *Create();
	static HRESULT Load();		// テクスチャの読み込み
	static void Unload();		// テクスチャの削除

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	// プレイヤーのスコア設定
	static void SetScore(int nScore) { m_nScorePlayer = nScore; }

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];	//テクスチャのポインタ
	static int m_nScorePlayer;
	// スコア格納用
	CScore* m_pScore[MAX_RANKING + MAX_PLAYER_SCORE];
	// オブジェクトのポインタ
	CObject2D* m_apObject2D[TYPE_MAX];
	// スコア格納用
	int m_aScore[MAX_RANKING + MAX_PLAYER_SCORE];
	bool m_bMyScoreFade[MAX_RANKING + MAX_PLAYER_SCORE];
	bool m_bNextMode;
};

#endif
