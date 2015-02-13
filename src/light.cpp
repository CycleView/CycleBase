#include "light.h"
#include <wiringPiSPI.h>
#include <iostream>
#include <cstring> // for std::strerror
#include <cstdlib>
#include <thread>
#include <chrono>

using namespace std;

#define CHANNEL 0

Light::Light(QObject *parent)
    : QThread(parent),
    mBlinkTime(mNormalBlink),
    mBlinkingState(On)
{
    if (wiringPiSPISetup(CHANNEL, 1000000) < 0)
        std::cout << "SPI setup failed: " << std::strerror(errno) << std::endl;

    mTimer.setSingleShot(true);
    mTimer.setInterval(10000);
    connect(&mTimer, &QTimer::timeout, [=]() {
        mBlinkTime = mNormalBlink;
        printf("Normal blink restored!\n");
    });
}

Light::~Light()
{
    quit();
    wait();
}

void Light::run()
{
    std::cout << "Light started" << std::endl;
    for(;;)
    {
        if (mBlinkingState == On)
        {
            memcpy(buf, on, 2);
            wiringPiSPIDataRW(CHANNEL, buf, 2);
            QThread::msleep(mBlinkTime.load());
            memcpy(buf, off, 2);
            wiringPiSPIDataRW(CHANNEL, buf, 2);
            QThread::msleep(mBlinkTime.load());
        }
    }
}

void Light::onClose()
{
    int blinkTime = mBlinkTime.load();
    if (blinkTime != mFastBlink)
    {
        std::cout << "Fast blink activated!" << std::endl;
        mBlinkTime.store(mFastBlink);
    }
    mTimer.start();
}

void Light::setBlinkingState(BlinkingState state)
{
    mBlinkingState = state;
}
