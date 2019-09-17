#pragma once

#include <unordered_map>
#include <math/Vec.h>

class InputManager
{
public:
    InputManager() = default;

    static void Update();

    static void PressKey(unsigned int keyId);
    static void ReleaseKey(unsigned int keyId);

    static bool IsKeyDown(unsigned int keyId);
    static bool IsKeyPressed(unsigned int keyId);

    static bool IsButtonDown(unsigned int buttonId);
    static bool IsButtonPressed(unsigned int buttonId);

    static void SetMouseCoords(float x, float y);
    static Vec2 GetMouseCoords() { return m_MouseCoords; }
    static Vec2 GetMouseMotion() { return m_MouseCoords - m_PrevMouseCoords; };
    static int GetScroll() { return m_CurrScroll - m_PrevScroll; }


private:
    inline static std::unordered_map<unsigned int, bool> m_KeyMap;
    inline static std::unordered_map<unsigned int, bool> m_PrevKeyMap;
    inline static std::unordered_map<unsigned int, bool> m_ButtonMap;
    inline static std::unordered_map<unsigned int, bool> m_PrevButtonMap;
    inline static Vec2 m_MouseCoords;
    inline static Vec2 m_PrevMouseCoords;
    inline static int m_CurrScroll;
    inline static int m_PrevScroll;

    static bool WasKeyDown(unsigned int keyId);
    static bool WasButtonDown(unsigned int buttonId);
};

