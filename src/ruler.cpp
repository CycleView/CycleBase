#include "ruler.h"
#include <wiringPi.h>
#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>

// GPIP23 = 4, GPIO24 = 5
#define TRIG 4
#define ECHO 5

Ruler::Ruler(QObject *parent)
    : QThread(parent)
{
    wiringPiSetup();
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    // TRIG pin must start LOW
    digitalWrite(TRIG, LOW);

    QThread::msleep(200);
}

Ruler::~Ruler()
{
    quit();
    wait();
}

void Ruler::run()
{
    std::cout << "Ruler started" << std::endl;
    while (true)
    {
        digitalWrite(TRIG, HIGH);
        QThread::usleep(10);
        digitalWrite(TRIG, LOW);

        // Wait for echo start
        while (digitalRead(ECHO) == LOW);
        auto startTime = std::chrono::steady_clock::now();

        // Wait for echo end
        while (digitalRead(ECHO) == HIGH);
        auto travelTime = std::chrono::steady_clock::now() - startTime;

        // Get distance in centimeters
        int distance = std::chrono::duration_cast<std::chrono::microseconds>(travelTime).count() / 58;

        //std::cout << "Measured: " << distance << "cm" << std::endl;

        const int size = distances.size();
        for (int i = 0; i < size; i++)
            distances[i] = (i + 1 < size) ? distances[i + 1] : distance;

        for (int i = 0; i < size; i++)
        {
            if (i + 1 < size && distances[i + 1] >= distances[i])
                break;
            else if (i + 1 == size && distances[i] < distances[i - 1])
            {
                std::cout << "Approximation detected!" << std::endl;
                emit isClose(distance);
            }
        }

        QThread::msleep(300);
    }
}
