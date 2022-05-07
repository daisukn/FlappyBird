#pragma once

#include "Math.h"
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_gamecontroller.h>
//#include <SDL2/SDL_mouse.h>


// ボタンステータスの定義
enum ButtonState
{
    ENone,
    EPressed,
    EReleased,
    EHeld
};


// キーボード用
class KeyboardState
{
public:
    // InputSystemにフレンド
    friend class InputSystem;
    // boolで応答
    bool GetKeyValue(SDL_Scancode keyCode) const;
    // ステータスで応答
    ButtonState GetKeyState(SDL_Scancode keyCode) const;
private:
    const Uint8* CurrState;
    Uint8 PrevState[SDL_NUM_SCANCODES];
};

// コントローラー用
class ControllerState
{
public:
    friend class InputSystem;

    // ボタン
    bool GetButtonValue(SDL_GameControllerButton button) const;
    ButtonState GetButtonState(SDL_GameControllerButton button) const;

    const Vector2& GetLeftStick() const { return LeftStick; }
    const Vector2& GetRightStick() const { return RightStick; }
    float GetLeftTrigger() const { return LeftTrigger; }
    float GetRightTrigger() const { return RightTrigger; }

    bool GetIsConnected() const { return IsConnected; }
private:
    Uint8 CurrButtons[SDL_CONTROLLER_BUTTON_MAX];
    Uint8 PrevButtons[SDL_CONTROLLER_BUTTON_MAX];
    // 左右のスティック
    Vector2 LeftStick;
    Vector2 RightStick;
    // 左右のトリガー
    float LeftTrigger;
    float RightTrigger;
    // 接続されているか
    bool IsConnected;
};



// 入力情報のラッパー
// マウス、ジョイコントローラーなどもここでラップ
struct InputState
{
    KeyboardState Keyboard;
    ControllerState Controller;
};


// 入力システム（キーボードのみ実装）
class InputSystem
{
public:
    bool Initialize();
    void Shutdown();

    // SDL_PollEvents前に呼ぶ、状態を記録
    void PrepareForUpdate();
    // SDL_PollEventsの後に呼ぶ
    void Update();
    
    // 状態を返す
    const InputState& GetState() const { return state; }

private:
    float Filter1D(int input);
    Vector2 Filter2D(int inputX, int inputY);
    SDL_GameController* controller;
    InputState state;
};


