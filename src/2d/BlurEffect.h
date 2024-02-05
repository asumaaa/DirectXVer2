/**
 * @file BlurEffect.h
 * @brief ポストエフェクト ブラー
 * @author Asuma Syota
 * @date 2023/4
 */

#pragma once
#include "PostEffect.h"

class BlurEffect : public PostEffect
{
public:	//メンバ関数

	/// <summary>
	///パイプライン設定、作成
	/// </summary>
	void CreateGraphicsPipeLine();
};
