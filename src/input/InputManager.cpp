#include <iostream>
#include "InputManager.h"

void InputManager::Update()
{
    for (auto &key : m_KeyMap) {
        m_PrevKeyMap[key.first] = key.second;
    }
}

void InputManager::PressKey(unsigned int keyId)
{
    m_KeyMap[keyId] = true;
}

void InputManager::ReleaseKey(unsigned int keyId)
{
    m_KeyMap[keyId] = false;
}

bool InputManager::IsKeyDown(unsigned int keyId)
{
    auto iter = m_KeyMap.find(keyId);
    if (iter != m_KeyMap.end()) {
        return iter->second;
    }
    return false;
}

bool InputManager::WasKeyDown(unsigned int keyId)
{
    auto iter = m_PrevKeyMap.find(keyId);
    if (iter != m_PrevKeyMap.end()) {
        return iter->second;
    }
    return false;
}

bool InputManager::IsKeyPressed(unsigned int keyId)
{
    return IsKeyDown(keyId) && !WasKeyDown(keyId);
}

bool InputManager::IsButtonDown(unsigned int keyID)
{
    auto iter = m_ButtonMap.find(keyID);
    if (iter != m_ButtonMap.end()) {
        return iter->second;
    }
    return false;
}

bool InputManager::WasButtonDown(unsigned int buttonId)
{
    auto iter = m_PrevButtonMap.find(buttonId);
    if (iter != m_PrevButtonMap.end()) {
        return iter->second;
    }
    return false;
}

bool InputManager::IsButtonPressed(unsigned int keyID)
{
    return IsButtonDown(keyID) && !WasButtonDown(keyID);
}

void InputManager::SetMouseCoords(float x, float y)
{
    m_PrevMouseCoords = m_MouseCoords;
    m_MouseCoords.x = x;
    m_MouseCoords.y = y;
}
