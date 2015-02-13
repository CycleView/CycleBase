#ifndef RULER_H
#define RULER_H

#include <QThread>
#include <limits>
#include <array>

class Ruler : public QThread
{
    Q_OBJECT

public:
    Ruler(QObject *parent = 0);
    ~Ruler();

private:
    std::array<int, 5> distances = {{ std::numeric_limits<int>::max() }};

    void run() Q_DECL_OVERRIDE;

signals:
    void isClose(int centimeters);
};

#endif
