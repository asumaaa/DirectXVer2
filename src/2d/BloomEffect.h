/**
 * @file BloomEffect.h
 * @brief ポストエフェクト ブルーム
 * @author Asuma Syota
 * @date 2023/10/29
 */

#pragma once
#include "PostEffect.h"

class BloomEffect : public PostEffect
{
public:	//メンバ関数

	/// <summary>
	///パイプライン設定、作成
	/// </summary>
	void CreateGraphicsPipeLine();
};
