#pragma once

#include <unordered_map>
#include <math/Vec.h>

class InputManager
{
public:
    InputManager() = default;

    static void update();

    static void pressKey(unsigned int keyID);
    static void releaseKey(unsigned int keyID);

    static bool isKeyDown(unsigned int keyID);
    static bool isKeyPressed(unsigned int keyID);

    static bool isButtonDown(unsigned int buttonId);
    static bool isButtonPressed(unsigned int buttonId);

    static void setMouseCoords(float x, float y);
    static Vec2 getMouseCoords() { return m_MouseCoords; }
    static Vec2 getMouseMotion() { return m_MouseCoords - m_PrevMouseCoords; };

    static int getScroll() { return m_CurrScroll - m_PrevScroll; }

private:
    inline static std::unordered_map<unsigned int, bool> m_KeyMap;
    inline static std::unordered_map<unsigned int, bool> m_PrevKeyMap;
    inline static std::unordered_map<unsigned int, bool> m_ButtonMap;
    inline static std::unordered_map<unsigned int, bool> m_PrevButtonMap;
    inline static Vec2 m_MouseCoords;
    inline static Vec2 m_PrevMouseCoords;
    inline static int m_CurrScroll;
    inline static int m_PrevScroll;

    static bool wasKeyDown(unsigned int keyID);
    static bool wasButtonDown(unsigned int buttonID);
};

