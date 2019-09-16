#include <iostream>
#include "InputManager.h"

void InputManager::update()
{
    for (auto &key : m_KeyMap) {
        m_PrevKeyMap[key.first] = key.second;
    }
}

void InputManager::pressKey(unsigned int keyID)
{
    m_KeyMap[keyID] = true;
    std::cout << "Key " << keyID << " pressed" << std::endl;
}

void InputManager::releaseKey(unsigned int keyID)
{
    m_KeyMap[keyID] = false;
    std::cout << "Key " << keyID << " released" << std::endl;
}

bool InputManager::isKeyDown(unsigned int keyID)
{
    auto iter = m_KeyMap.find(keyID);
    if (iter != m_KeyMap.end()) {
        return iter->second;
    }
    return false;
}

bool InputManager::wasKeyDown(unsigned int keyID)
{
    auto iter = m_PrevKeyMap.find(keyID);
    if (iter != m_PrevKeyMap.end()) {
        return iter->second;
    }
    return false;
}

bool InputManager::isKeyPressed(unsigned int keyID)
{
    return isKeyDown(keyID) && !wasKeyDown(keyID);
}

bool InputManager::isButtonDown(unsigned int keyID)
{
    auto iter = m_ButtonMap.find(keyID);
    if (iter != m_ButtonMap.end()) {
        return iter->second;
    }
    return false;
}

bool InputManager::wasButtonDown(unsigned int keyID)
{
    auto iter = m_PrevButtonMap.find(keyID);
    if (iter != m_PrevButtonMap.end()) {
        return iter->second;
    }
    return false;
}

bool InputManager::isButtonPressed(unsigned int keyID)
{
    return isButtonDown(keyID) && !wasButtonDown(keyID);
}

void InputManager::setMouseCoords(float x, float y)
{
    m_PrevMouseCoords = m_MouseCoords;
    m_MouseCoords.x = x;
    m_MouseCoords.y = y;
}
