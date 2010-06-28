#ifndef CONSTANTS_H
#define CONSTANTS_H



#include <qglobal.h>
#include <qnamespace.h>
#include <qdebug.h>

namespace QtEpidemy {


    typedef qreal RatioType;
    typedef qint32 AmountType;

    typedef quint32 TickType;

    const static RatioType DT = 1.0/24.0;  // how many DAYS elapse between steps. 1/24 = 1h

    inline void debugPr(const char* func, QString stuff) {
        qDebug() << func << "\n\t" << stuff;
    }

#define DPR(message) debugPr(Q_FUNC_INFO, message)

    // use this when the class has objectName set (just City at the moment)
#define CDPR(message) debugPr(Q_FUNC_INFO, objectName() % ": " % message)
#define CDPR_STAT(newstat, oldstat) CDPR(tr("%1 (old %2)").arg(newstat).arg(oldstat));

    enum PathogenStat { PS_SURVIVALRATE, PS_CONTACTRATE, PS_DURATION,
                        PS_IMMUNITYLOSSRATE, PS_MAX_STATS };

    const QString PS_NAMES[] = {QT_TR_NOOP("PS_SURVIVALRATE"),
                                QT_TR_NOOP("PS_CONTACTRATE"),
                                QT_TR_NOOP("PS_DURATION"),
                                QT_TR_NOOP("PS_IMMUNITYLOSS"),
                                QT_TR_NOOP("PS_MAX_STATS")};

    enum CityStat {CS_D_INF_DEATHS, CS_D_QUAR_DEATHS, CS_D_INF_RECOVER,
                   CS_D_QUAR_RECOVER, CS_D_INFECTIONS, CS_D_QUARANTINES,
                   CS_D_IMMUNITYLOSS, CS_SUSCEPTIBLE, CS_INFECTED,
                   CS_RECOVERED, CS_DEAD, CS_QUARANTINED, CS_QUAR_RATE,
                   CS_POPULATION, CS_MAX_STATS};

    const QString CS_NAMES[] = {QT_TR_NOOP("CS_D_INF_DEATHS"),
                               QT_TR_NOOP("CS_D_QUAR_DEATHS"),
                               QT_TR_NOOP(" CS_D_INF_RECOVER"),
                               QT_TR_NOOP("CS_D_QUAR_RECOVER"),
                               QT_TR_NOOP("CS_D_INFECTIONS"),
                               QT_TR_NOOP("CS_D_QUARANTINES"),
                               QT_TR_NOOP("CS_D_IMMUNITYLOSS"),
                               QT_TR_NOOP("CS_SUSCEPTIBLE"),
                               QT_TR_NOOP("CS_INFECTED"),
                               QT_TR_NOOP("CS_RECOVERED"),
                               QT_TR_NOOP("CS_DEAD"),
                               QT_TR_NOOP("CS_QUARANTINED"),
                               QT_TR_NOOP("CS_QUAR_RATE"),
                               QT_TR_NOOP("CS_POPULATION"),
                               QT_TR_NOOP("CS_MAX_STATS")
                               };

    enum TransportType {TT_PLANE,
                        TT_CAR,
                        TT_BOAT,
                        TT_ONFOOT};


    const QString TRANSPORT_NAMES[] = {QT_TR_NOOP("TT_PLANE"),
                                       QT_TR_NOOP("TT_CAR"),
                                       QT_TR_NOOP("TT_BOAT"),
                                       QT_TR_NOOP("TT_ONFOOT")
                                       };

    enum PassengerType {PT_SUSCEPTIBLE,
                        PT_INFECTED,
                        PT_MAX_TYPES};

    const QString PASSENGER_TYPE_NAMES[] = {QT_TR_NOOP("PT_SUSCEPTIBLE"),
                                            QT_TR_NOOP("PT_INFECTED"),
                                            QT_TR_NOOP("PT_MAX_TYPES")
                                            };


    // speeds of different vehicles in km/h
    const qint16 TRANSPORT_SPEEDS[] = {800, 80, 45, 6};


    const Qt::GlobalColor CURVE_PENCOLORS[] = {Qt::red, Qt::green, Qt::blue, Qt::cyan, Qt::magenta,
                                               Qt::darkYellow, Qt::black};

    const Qt::PenStyle CURVE_PENSTYLES[] = {Qt::SolidLine, Qt::DashLine, Qt::DotLine,
                                            Qt::DashDotDotLine};
    const quint8 CURVE_NUMCOLORS = 6;
    const quint8 CURVE_NUMSTYLES = 5;

    template<typename T>
    inline void clampToZero(T& val) {
        if(val < (T)0) {
            val = (T)0;
        }
    }
}

#endif // CONSTANTS_H
