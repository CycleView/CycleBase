#ifndef LIGHT_H
#define LIGHT_H

#include <stdint.h>
#include <QObject>
#include <QThread>
#include <QTimer>

class Light : public QThread
{
    Q_OBJECT

public:
    Light(QObject *parent = 0);
    ~Light();

    enum BlinkingState
    {
        Off = 0,
        On
    };

    void setBlinkingState(BlinkingState state);

public slots:
    void onClose();

private:
    const int mNormalBlink = 500;
    const int mFastBlink = 100;
    int mBlinkTime;
    BlinkingState mBlinkingState;
    QTimer timer;

    uint8_t on[2] = { 0xFF, 0xFF };
    uint8_t off[2] = { 0xFF, 0x00 };
    uint8_t buf[2];

    void run() Q_DECL_OVERRIDE;
};

#endif
