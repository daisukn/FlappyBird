#include "InputSystem.h"

#include <SDL2/SDL.h>
#include <cstring>


bool KeyboardState::GetKeyValue(SDL_Scancode keyCode) const
{
    return CurrState[keyCode] == 1;
}

// キーボードの状態を返す
ButtonState KeyboardState::GetKeyState(SDL_Scancode keyCode) const
{
    if (PrevState[keyCode] == 0)
    {
        if (CurrState[keyCode] == 0)
        {
            return ENone;
        }
        else
        {
            return EPressed;
        }
    }
    else // 前の状態が1の時
    {
        if (CurrState[keyCode] == 0)
        {
            return EReleased;
        }
        else
        {
            return EHeld;
        }
    }
}



bool ControllerState::GetButtonValue(SDL_GameControllerButton button) const
{
    return CurrButtons[button] == 1;
}

// コントローラーのボタンの状態を返す
ButtonState ControllerState::GetButtonState(SDL_GameControllerButton button) const
{
    if (PrevButtons[button] == 0)
    {
        if (CurrButtons[button] == 0)
        {
            return ENone;
        }
        else
        {
            return EPressed;
        }
    }
    else
    {
        if (CurrButtons[button] == 0)
        {
            return EReleased;
        }
        else
        {
            return EHeld;
        }
    }
}



bool InputSystem::Initialize()
{
    // キーボード
    state.Keyboard.CurrState = SDL_GetKeyboardState(NULL);
    memset(state.Keyboard.PrevState, 0, SDL_NUM_SCANCODES);
    
    // マウス、コントローラーもここで初期化
    // コントローラーを探す
    controller = SDL_GameControllerOpen(0);
    // コントローラーがあれば初期化
    state.Controller.IsConnected = (controller != nullptr);
    memset(state.Controller.CurrButtons, 0, SDL_CONTROLLER_BUTTON_MAX);
    memset(state.Controller.PrevButtons, 0, SDL_CONTROLLER_BUTTON_MAX);
    
    return true;
}

void InputSystem::Shutdown()
{
}

// 前の状態をコピー
void InputSystem::PrepareForUpdate()
{
    // 前フレームの状態にコピー
    // キーボード
    memcpy(state.Keyboard.PrevState, state.Keyboard.CurrState, SDL_NUM_SCANCODES);
    // コントローラー
    memcpy(state.Controller.PrevButtons, state.Controller.CurrButtons, SDL_CONTROLLER_BUTTON_MAX);
}

void InputSystem::Update()
{
    // キーボードはアップデート必要なし
    
    
    // Controller
    // ボタン
    for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++)
    {
        state.Controller.CurrButtons[i] = SDL_GameControllerGetButton(controller, SDL_GameControllerButton(i));
    }

    // トリガー
    state.Controller.LeftTrigger = Filter1D(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT));
    state.Controller.RightTrigger = Filter1D(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT));

    int x = 0, y = 0;
    // スティック
    x = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
    y = -SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);
    state.Controller.LeftStick = Filter2D(x, y);

    x = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);
    y = -SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY);
    state.Controller.RightStick = Filter2D(x, y);
    
}


// デッドゾーンをフリップ
float InputSystem::Filter1D(int input)
{
    // A value < dead zone is interpreted as 0%
    const int deadZone = 250;
    // A value > max value is interpreted as 100%
    const int maxValue = 30000;

    float retVal = 0.0f;

    // Take absolute value of input
    int absValue = input > 0 ? input : -input;
    // Ignore input within dead zone
    if (absValue > deadZone)
    {
        // Compute fractional value between dead zone and max value
        retVal = static_cast<float>(absValue - deadZone) /
        (maxValue - deadZone);
        // Make sure sign matches original value
        retVal = input > 0 ? retVal : -1.0f * retVal;
        // Clamp between -1.0f and 1.0f
        retVal = Math::Clamp(retVal, -1.0f, 1.0f);
    }

    return retVal;
}

// デッドゾーンをフリップ
Vector2 InputSystem::Filter2D(int inputX, int inputY)
{
    const float deadZone = 8000.0f;
    const float maxValue = 30000.0f;

    // Make into 2D vector
    Vector2 dir;
    dir.x = static_cast<float>(inputX);
    dir.y = static_cast<float>(inputY);

    float length = dir.Length();

    // If length < deadZone, should be no input
    if (length < deadZone)
    {
        dir = Vector2::Zero;
    }
    else
    {
        // Calculate fractional value between
        // dead zone and max value circles
        float f = (length - deadZone) / (maxValue - deadZone);
        // Clamp f between 0.0f and 1.0f
        f = Math::Clamp(f, 0.0f, 1.0f);
        // Normalize the vector, and then scale it to the
        // fractional value
        dir *= f / length;
    }

    return dir;
}



