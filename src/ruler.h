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
    int max = std::numeric_limits<int>::max();
    std::array<int, 4> distances = {{ max }};

    void run() Q_DECL_OVERRIDE;

signals:
    void isClose(int centimeters);
};

#endif
