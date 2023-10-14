#include "DXInput.h"
#include "mathOriginal.h"

DXInput* DXInput::GetInstance()
{
    static DXInput instance;
    return&instance;
}

DXInput::DXInput()
{
}

DXInput::~DXInput()
{
}

void DXInput::InputProcess() {

    XInputGetState(0, &GamePad.state);

    //int pad_A = 0, pad_B = 0, pad_X = 0, pad_Y = 0;
    //int pad_leftShoulder = 0, pad_rightShoulder = 0;
    //int pad_left = 0, pad_right = 0, pad_up = 0, pad_down = 0;

    //if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) pad_left = 1;               //ゲームパッド十字キー左
    //if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) pad_right = 1;             //ゲームパッド十字キー右
    //if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) pad_up = 1;                   //ゲームパッド十字キー上
    //if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) pad_down = 1;               //ゲームパッド十字キー下
    //if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_A) pad_A = 1;                          //ゲームパッドA
    //if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_B) pad_B = 1;                          //ゲームパッドB
    //if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_X) pad_X = 1;                          //ゲームパッドX
    //if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) pad_Y = 1;                          //ゲームパッドY
    //if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) pad_leftShoulder = 1;   //ゲームパッドL
    //if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) pad_rightShoulder = 1; //ゲームパッドR

    // ゲームパッドデッドゾーン処理
    if ((GamePad.state.Gamepad.sThumbLX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
        GamePad.state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
        (GamePad.state.Gamepad.sThumbLY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
            GamePad.state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
    {
        GamePad.state.Gamepad.sThumbLX = 0;
        GamePad.state.Gamepad.sThumbLY = 0;
    }
    if ((GamePad.state.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
        GamePad.state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
        (GamePad.state.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
            GamePad.state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
    {
        GamePad.state.Gamepad.sThumbRX = 0;
        GamePad.state.Gamepad.sThumbRY = 0;
    }

    // ゲームパッドの振動
    /*XInputSetState(0, &GamePad.vibration);*/

    //if (pad_A)
    //{
    //    GamePad.vibration.wLeftMotorSpeed = MaxVibration;
    //    /*XInputSetState(0, &GamePad.vibration);*/
    //}
    //if (pad_B)
    //{
    //    GamePad.vibration.wRightMotorSpeed = MaxVibration;
    //    /*XInputSetState(0, &GamePad.vibration);*/
    //}
    //if (pad_X)
    //{
    //    GamePad.vibration.wLeftMotorSpeed = MaxVibration;
    //    /* XInputSetState(0, &GamePad.vibration);*/
    //}
    //if (pad_Y)
    //{
    //    GamePad.vibration.wRightMotorSpeed = MaxVibration;
    //    /*XInputSetState(0, &GamePad.vibration);*/
    //}

    /*WCHAR wcText[256] = { 0 };
    swprintf(wcText, 256, L"PAD_A=%d PAD_B=%d PAD_X=%d PAD_Y=%d", pad_A, pad_B, pad_X, pad_Y);*/

}

void DXInput::XInputInvaid() {

    //XInputEnable(false); // XInputを無効

}

void DXInput::Update()
{
    //1フレーム前のコントローラーを保存
    UpdateOldKey();

    //1フレーム前のスティックを保存
    UpdateOldStick();

    //更新
    InputProcess();

    //現在のフレームのキーを取得
    UpdateKey();

    //現在のフレームのスティックを取得
    UpdateStick();
}

void DXInput::UpdateKey()
{
    if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) key.PAD_LEFT = 1; else { key.PAD_LEFT = 0; }
    if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) key.PAD_RIGHT = 1; else { key.PAD_RIGHT = 0; }             //ゲームパッド十字キー右
    if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) key.PAD_UP = 1; else { key.PAD_UP = 0; }                  //ゲームパッド十字キー上
    if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) key.PAD_DOWN = 1; else { key.PAD_DOWN = 0; }              //ゲームパッド十字キー下
    if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_A) key.PAD_A = 1; else { key.PAD_A = 0; }                         //ゲームパッドA
    if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_B) key.PAD_B = 1; else { key.PAD_B = 0; }                         //ゲームパッドB
    if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_X) key.PAD_X = 1; else { key.PAD_X = 0; }                         //ゲームパッドX
    if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) key.PAD_Y = 1; else { key.PAD_Y = 0; }                         //ゲームパッドY
    if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) key.PAD_LEFT_SHOULDER = 1; else { key.PAD_LEFT_SHOULDER = 0; }  //ゲームパッドL
    if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) key.PAD_RIGHT_SHOULDER = 1; else { key.PAD_RIGHT_SHOULDER = 0; } //ゲームパッドR
}

void DXInput::UpdateOldKey()
{
    oldKey = key;
}

void DXInput::UpdateStick()
{
    stick.RStickX = GamePad.state.Gamepad.sThumbRX / 32767.0f;
    stick.RStickY = GamePad.state.Gamepad.sThumbRY / 32767.0f;
    stick.LStickX = GamePad.state.Gamepad.sThumbLX / 32767.0f;
    stick.LStickY = GamePad.state.Gamepad.sThumbLY / 32767.0f;
    stick.RStick = length(stick.RStickX, stick.RStickY);
    stick.LStick = length(stick.LStickX, stick.LStickY);
}

void DXInput::UpdateOldStick()
{
   oldStick = stick;
}


bool DXInput::PushKey(Pad pad)
{
    if (pad == PAD_LEFT)return key.PAD_LEFT;
    if (pad == PAD_RIGHT)return key.PAD_RIGHT;
    if (pad == PAD_UP)return key.PAD_UP;
    if (pad == PAD_DOWN)return key.PAD_DOWN;
    if (pad == PAD_A)return key.PAD_A;
    if (pad == PAD_B)return key.PAD_B;
    if (pad == PAD_X)return key.PAD_X;
    if (pad == PAD_Y)return key.PAD_Y;
    if (pad == PAD_LEFT_SHOULDER)return key.PAD_LEFT_SHOULDER;
    if (pad == PAD_RIGHT_SHOULDER)return key.PAD_RIGHT_SHOULDER;
}

bool DXInput::TriggerKey(Pad pad)
{
    if (pad == PAD_LEFT) {
        if (key.PAD_LEFT != oldKey.PAD_LEFT)return true;
        return false;
    }
    if (pad == PAD_RIGHT) {
        if (key.PAD_RIGHT != oldKey.PAD_RIGHT)return true;
        return false;
    }
    if (pad == PAD_UP) {
        if (key.PAD_UP != oldKey.PAD_UP)return true;
        return false;
    }
    if (pad == PAD_DOWN) {
        if (key.PAD_DOWN != oldKey.PAD_DOWN)return true;
        return false;
    }
    if (pad == PAD_A) {
        if (key.PAD_A != oldKey.PAD_A)return true;
        return false;
    }
    if (pad == PAD_B) {
        if (key.PAD_B != oldKey.PAD_B)return true;
        return false;
    }
    if (pad == PAD_X) {
        if (key.PAD_X != oldKey.PAD_X)return true;
        return false;
    }
    if (pad == PAD_Y) {
        if (key.PAD_Y != oldKey.PAD_Y)return true;
        return false;
    }
    if (pad == PAD_LEFT_SHOULDER) {
        if (key.PAD_LEFT_SHOULDER != oldKey.PAD_LEFT_SHOULDER)return true;
        return false;
    }
    if (pad == PAD_RIGHT_SHOULDER) {
        if (key.PAD_RIGHT_SHOULDER != oldKey.PAD_RIGHT_SHOULDER)return true;
        return false;
    }
}

float DXInput::GetStick(Stick stick)
{
    if (stick == RStickX)return this->stick.RStickX;
    if (stick == RStickY)return this->stick.RStickY;
    if (stick == LStickX)return this->stick.LStickX;
    if (stick == LStickY)return this->stick.LStickY;
    if (stick == RStick)return this->stick.RStick;
    if (stick == LStick)return this->stick.LStick;
}

float DXInput::GetStickRot(Stick stick)
{
    float stickX = 0.0f;
    float stickY = 0.0f;
    //右スティックの角度を参照する場合
    if (stick == RStick || stick == RStickX || stick == RStickY)
    {
        //入力がない場合0を返す
        if (this->stick.RStickX == 0 && this->stick.RStickY == 0)
        {
            return 0.0f;
        }
        //正規化したYの値を代入
        stickY = normalize(this->stick.RStickX, this->stick.RStickY).y;
        //ステックのXの値を代入
        stickX = this->stick.RStickY;
    }
    //左スティックの角度を参考にする場合
    else if (stick == LStick || stick == LStickX || stick == LStickY)
    {
        //入力がない場合0を返す
        if (this->stick.LStickX == 0 && this->stick.LStickY == 0)
        {
            return 0.0f;
        }
        //正規化したYの値を代入
        stickY = normalize(this->stick.LStickX, this->stick.LStickY).y;
        //ステックのXの値を代入
        stickX = this->stick.LStickX;
    }
    //0~180の場合
    if (stickX >= 0.0f)
    {
        //-1する
        stickY -= 1.0f;
        //-を外す
        stickY *= -1.0f;
        //度数法に変換
        stickY *= 90.0f;
        //ラジアンに変換
        stickY *= (float(PI) / 180.0f);
    }
    //180~360の場合
    else
    {
        //+1する
        stickY += 1.0f;
        //度数法に変換
        stickY *= 90.0f;
        //180度追加
        stickY += 180.0f;
        //ラジアンに変換
        stickY *= (float(PI) / 180.0f);
    }
    return stickY;
}

float DXInput::GetOldStick(Stick stick)
{
    if (stick == RStickX)return this->oldStick.RStickX;
    if (stick == RStickY)return this->oldStick.RStickY;
    if (stick == LStickX)return this->oldStick.LStickX;
    if (stick == LStickY)return this->oldStick.LStickY;
    if (stick == RStick)return this->oldStick.RStick;
    if (stick == LStick)return this->oldStick.LStick;
}
