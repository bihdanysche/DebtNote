#include "Money.h"

#include <QtMath>

Money Money::fromHryvnias(double hryvnias) {
    return Money(qRound64(hryvnias * 100.0));
}

QString Money::toString() const {
    const qint64 absolute = qAbs(m_kopecks);
    const QString sign = m_kopecks < 0 ? "-" : "";

    return QString("%1%2.%3 грн")
        .arg(sign)
        .arg(absolute / 100)
        .arg(absolute % 100, 2, 10, QChar('0'));
}