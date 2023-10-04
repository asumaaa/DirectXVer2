#include "DXInput.h"

DXInput *DXInput::GetInstance()
{
    static DXInput instance;
    return& instance;
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

    //if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) pad_left = 1;               //�Q�[���p�b�h�\���L�[��
    //if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) pad_right = 1;             //�Q�[���p�b�h�\���L�[�E
    //if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) pad_up = 1;                   //�Q�[���p�b�h�\���L�[��
    //if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) pad_down = 1;               //�Q�[���p�b�h�\���L�[��
    //if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_A) pad_A = 1;                          //�Q�[���p�b�hA
    //if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_B) pad_B = 1;                          //�Q�[���p�b�hB
    //if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_X) pad_X = 1;                          //�Q�[���p�b�hX
    //if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) pad_Y = 1;                          //�Q�[���p�b�hY
    //if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) pad_leftShoulder = 1;   //�Q�[���p�b�hL
    //if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) pad_rightShoulder = 1; //�Q�[���p�b�hR

    // �Q�[���p�b�h�f�b�h�]�[������
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

    // �Q�[���p�b�h�̐U��
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

    //XInputEnable(false); // XInput�𖳌�

}

void DXInput::Update()
{
    //1�t���[���O�̃R���g���[���[��ۑ�
    UpdateOldKey();

    //�X�V
    InputProcess();

    //���݂̃t���[���̃L�[���擾
    UpdateKey();

    //���݂̃t���[���̃X�e�B�b�N���擾
    UpdateStick();
}

void DXInput::UpdateKey()
{
    if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) key.PAD_LEFT = 1; else { key.PAD_LEFT = 0; }
    if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) key.PAD_RIGHT = 1; else { key.PAD_RIGHT = 0; }             //�Q�[���p�b�h�\���L�[�E
    if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) key.PAD_UP = 1; else { key.PAD_UP = 0; }                  //�Q�[���p�b�h�\���L�[��
    if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) key.PAD_DOWN = 1; else { key.PAD_DOWN = 0; }              //�Q�[���p�b�h�\���L�[��
    if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_A) key.PAD_A = 1; else { key.PAD_A = 0; }                         //�Q�[���p�b�hA
    if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_B) key.PAD_B = 1; else { key.PAD_B = 0; }                         //�Q�[���p�b�hB
    if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_X) key.PAD_X = 1; else { key.PAD_X = 0; }                         //�Q�[���p�b�hX
    if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) key.PAD_Y = 1; else { key.PAD_Y = 0; }                         //�Q�[���p�b�hY
    if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) key.PAD_LEFT_SHOULDER = 1; else { key.PAD_LEFT_SHOULDER = 0; }  //�Q�[���p�b�hL
    if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) key.PAD_RIGHT_SHOULDER = 1; else { key.PAD_RIGHT_SHOULDER = 0; } //�Q�[���p�b�hR
}

void DXInput::UpdateOldKey()
{
    oldKey = key;
}

void DXInput::UpdateStick()
{
    /*stick.RStickX = GamePad.state.Gamepad.sThumbRX / 65535;
    stick.RStickY = GamePad.state.Gamepad.sThumbRY / 65535;
    stick.LStickX = GamePad.state.Gamepad.sThumbLX / 65535;
    stick.LStickY = GamePad.state.Gamepad.sThumbLY / 65535;*/
    stick.RStickX = GamePad.state.Gamepad.sThumbRX / 32767;
    stick.RStickY = GamePad.state.Gamepad.sThumbRY / 32767;
    stick.LStickX = GamePad.state.Gamepad.sThumbLX / 32767;
    stick.LStickY = GamePad.state.Gamepad.sThumbLY / 32767;
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
    if (pad == PAD_LEFT){
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
}
