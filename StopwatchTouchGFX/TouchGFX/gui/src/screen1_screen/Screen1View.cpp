#include <gui/screen1_screen/Screen1View.hpp>

Screen1View::Screen1View()
{
    tickCount = 0;
}
void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::handleTickEvent()
{
    static uint32_t prevTick = 0;
    static float rad = 0.0f;

    Screen1ViewBase::handleTickEvent();

    tickCount = osKernelGetTickCount();

    uint8_t res = 0;
    uint32_t count = osMessageQueueGetCount(Queue1Handle);

    if (count > 0)
    {
        osMessageQueueGet(Queue1Handle, &res, NULL, 0);

        if (res == 'P')
        {
            float deltaSec = (tickCount - prevTick) / 1000.0f;

            /* Kim giây: 1 vòng / 60 giây => 2*pi/60 rad/s = pi/30 rad/s */
            rad += deltaSec * (3.1415926f / 30.0f);

            txtrHand.updateZAngle(rad);
            txtrHand.invalidate();
        }
    }

    prevTick = tickCount;
}
