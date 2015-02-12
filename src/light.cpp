#include "light.h"
#include <wiringPiSPI.h>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <thread>
#include <chrono>

using namespace std;

#define CHANNEL 0

Light::Light(QObject *parent)
    : QThread(parent),
    mBlinkingState(Off)
{
    if (wiringPiSPISetup(CHANNEL, 1000000) < 0)
        fprintf(stderr, "SPI setup failed %s\n", strerror(errno));

    timer.setSingleShot(true);
    timer.setInterval(10000);
    connect(&timer, &QTimer::timeout, [=]() {
        mBlinkTime = mNormalBlink;
    });
}

Light::~Light()
{
    quit();
    wait();
}

void Light::run()
{
    printf("Light started\n");
    while (mBlinkingState == On)
    {
        memcpy(buf, on, 2);
        wiringPiSPIDataRW(CHANNEL, buf, 2);
        std::this_thread::sleep_for(std::chrono::milliseconds(mBlinkTime));
        memcpy(buf, off, 2);
        wiringPiSPIDataRW(CHANNEL, buf, 2);
        std::this_thread::sleep_for(std::chrono::milliseconds(mBlinkTime));
    }
}

void Light::onClose()
{
    mBlinkTime = mFastBlink;
}

void Light::setBlinkingState(BlinkingState state)
{
    mBlinkingState = state;
}
