#pragma once

#include <QString>
#include <QtGlobal>

class Money {
public:
    Money() = default;
    explicit Money(qint64 kopecks) : m_kopecks(kopecks) {}

    static Money fromHryvnias(double hryvnias);

    qint64 kopecks() const { return m_kopecks; }
    double toHryvnias() const { return m_kopecks / 100.0; }
    QString toString() const;

    bool isZero() const { return m_kopecks == 0; }
    bool isPositive() const { return m_kopecks > 0; }
    bool isNegative() const { return m_kopecks < 0; }

    Money operator-() const { return Money(-m_kopecks); }
    Money& operator+=(const Money& other) { m_kopecks += other.m_kopecks; return *this; }
    Money& operator-=(const Money& other) { m_kopecks -= other.m_kopecks; return *this; }

    friend Money operator+(Money a, const Money& b) { a += b; return a; }
    friend Money operator-(Money a, const Money& b) { a -= b; return a; }

    friend bool operator==(const Money& a, const Money& b) { return a.m_kopecks == b.m_kopecks; }
    friend bool operator!=(const Money& a, const Money& b) { return a.m_kopecks != b.m_kopecks; }
    friend bool operator<(const Money& a, const Money& b) { return a.m_kopecks < b.m_kopecks; }
    friend bool operator>(const Money& a, const Money& b) { return a.m_kopecks > b.m_kopecks; }
    friend bool operator<=(const Money& a, const Money& b) { return a.m_kopecks <= b.m_kopecks; }
    friend bool operator>=(const Money& a, const Money& b) { return a.m_kopecks >= b.m_kopecks; }

private:
    qint64 m_kopecks = 0;
};