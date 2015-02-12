#include "ruler.h"
#include <wiringPi.h>
#include <cstdio>
#include <cstdlib>

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
    delay(30);
}

Ruler::~Ruler()
{
    quit();
    wait();
}

void Ruler::run()
{
    printf("Ruler started\n");
    while (true)
    {
        digitalWrite(TRIG, HIGH);
        delayMicroseconds(20);
        digitalWrite(TRIG, LOW);

        // Wait for echo start
        while (digitalRead(ECHO) == LOW);

        // Wait for echo end
        long startTime = micros();
        while (digitalRead(ECHO) == HIGH);
        long travelTime = micros() - startTime;

        // Get distance in centimeters
        int distance = travelTime / 58;

        const int size = distances.size();
        for (int i = 0; i < size; i++)
            distances[i] = (i + 1 < size) ? distances[i + 1] : distance;

        for (int i = 0; i < size; i++)
        {
            if (i + 1 < size && distances[i + 1] > distances[i])
                break;
            else if (i + 1 == size && distances[i] < distances[i - 1])
            {
                emit isClose(distance);
                printf("Approximation detected!\n");
            }
        }
    }
}
