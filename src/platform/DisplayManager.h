#pragma once

class DisplayManager
{
public:
    static long getCurrentTime()
    {
        return 0;
    }

    static float getFrameTimeSeconds()
    {
        return 0.01666666667f;
    }

    int getWidth() { return width; }
    int getHeight() { return height; }
    float getAspectRatio() { return ((float) width) / height; }
    void setSize(int new_width, int new_height)
    {
        width = new_width;
        height = new_height;
    };

private:
    int FPS_CAP = 120;
    int width = 1280;
    int height = 720;

    long lastFrameTime {};
    float delta {};
    int fps = 0;
};

