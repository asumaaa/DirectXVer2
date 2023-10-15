/**
 * @file DXInput.h
 * @brief ゲームコントローラの入力を管理
 * @author Asuma Syota
 * @date 2023/4
 */

#pragma once

#include <dwrite.h>
#include <wchar.h>
#include <wrl/client.h>
#include <random>
#include <xinput.h>
#include "WinApp.h"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")
#pragma comment (lib, "xinput.lib")

#define MaxCountrollers 4
#define MaxVibration 65535

/// <summary>
/// ゲームパッド
/// </summary>
class DXInput
{
public:
    struct CountrolerState
    {
        XINPUT_STATE state; // コントローラーの状態の取得
        XINPUT_VIBRATION vibration;  // バイブレーション
        //bool Connected;
    };
    CountrolerState GamePad;

    struct Key
    {
        int PAD_LEFT = 0;
        int PAD_RIGHT = 0;
        int PAD_UP = 0;
        int PAD_DOWN = 0;
        int PAD_A = 0;
        int PAD_B = 0;
        int PAD_X = 0;
        int PAD_Y = 0;
        int PAD_LEFT_SHOULDER = 0;
        int PAD_RIGHT_SHOULDER = 0;
    };

    struct Thumb
    {
        float RStickX = 0.0f;
        float RStickY = 0.0f;
        float LStickX = 0.0f;
        float LStickY = 0.0f;
        float RStick = 0.0f;
        float LStick = 0.0f;
    };

    enum Pad
    {
        PAD_LEFT,
        PAD_RIGHT,
        PAD_UP,
        PAD_DOWN,
        PAD_A,
        PAD_B,
        PAD_X,
        PAD_Y,
        PAD_LEFT_SHOULDER,
        PAD_RIGHT_SHOULDER,
    };

    enum Stick
    {
        RStickX,
        RStickY,
        LStickX,
        LStickY,
        RStick,
        LStick,
    };

public:// メンバ関数

    /// <summary>
    ///インスタンス取得
    /// </summary>
    static DXInput* GetInstance();

    //インストラクタ デストラクタ
    DXInput();
    ~DXInput();


    /// <summary>
    ///コントローラー更新
    /// </summary>
    void InputProcess();


    /// <summary>
    ///更新
    /// </summary>
    void Update();

    /// <summary>
    ///ボタン更新
    /// </summary>
    void UpdateKey();

    /// <summary>
    ///1フレーム前のボタン更新
    /// </summary>
    void UpdateOldKey();

    /// <summary>
    ///スティック更新
    /// </summary>
    void UpdateStick();

    /// <summary>
    ///1フレーム前のスティック更新
    /// </summary>
    void UpdateOldStick();

    /// <summary>
    ///ボタン取得
    /// </summary>  
    bool PushKey(Pad pad);
    
    /// <summary>
    ///トリガーボタン取得
    /// </summary>  
    bool TriggerKey(Pad pad);

    /// <summary>
    ///スティック取得
    /// </summary>
    float GetStick(Stick stick);

    /// <summary>
    ///スティックを真上から見た際の角度取得(ラジアン)
    /// </summary>
    float GetStickRot(Stick stick);

    /// <summary>
    ///1フレーム前のスティック取得
    /// </summary>
    float GetOldStick(Stick stick);

    /*HRESULT UpdateCountrollerState();*/

private:// メンバ変数
    //WinApp* winApp = nullptr;    // WindowsAPI

    Key key;
    Key oldKey;
    Thumb stick;
    Thumb oldStick;

private:
    static int  pad_A, pad_B, pad_X, pad_Y;
    static int pad_leftShoulder, pad_rightShoulder;
    static int pad_left, pad_right, pad_up, pad_down;
};