#ifndef CONSTANTS_H
#define CONSTANTS_H



#include <qglobal.h>
#include <qnamespace.h>
#define stringify(name) # name

namespace QtEpidemy {
    const static float DT = 1.0/24.0;  // how many DAYS elapse between steps. 1/24 = 1h

    typedef double ratioType;
    typedef qint32 amountType;

    enum PathogenStats { PS_SURVIVAL, PS_INFECTION, PS_DURATION, PS_MAX_STATS };

    enum CityStats { CS_D_INF_DEATHS, CS_D_QUAR_DEATHS, CS_D_INF_RECOVER, CS_D_QUAR_RECOVER,
                     CS_D_INFECTIONS, CS_D_QUARANTINES, CS_SUSCEPTIBLE, CS_INFECTED,
                     CS_RECOVERED, CS_DEAD, CS_QUARANTINED, CS_QUAR_RATE, CS_POPULATION, CS_MAX_STATS};

    const QString STATNAMES[] = {/*(char*)*/QT_TR_NOOP("Daily infected deaths"),
                               /*(char*)*/QT_TR_NOOP("Daily quarantined deaths"),
                               /*(char*)*/QT_TR_NOOP("Daily infected recoveries"),
                               /*(char*)*/QT_TR_NOOP(stringify(CS_D_QUAR_RECOVER)),
                               /*(char*)*/QT_TR_NOOP(stringify(CS_D_INFECTIONS)),
                               /*(char*)*/QT_TR_NOOP(stringify(CS_D_QUARANTINES)),
                               /*(char*)*/QT_TR_NOOP(stringify(CS_SUSCEPTIBLE)),
                               /*(char*)*/QT_TR_NOOP(stringify(CS_INFECTED)),
                               /*(char*)*/QT_TR_NOOP(stringify(CS_RECOVERED)),
                               /*(char*)*/QT_TR_NOOP(stringify(CS_DEAD)),
                               /*(char*)*/QT_TR_NOOP(stringify(CS_QUARANTINED)),
                               /*(char*)*/QT_TR_NOOP(stringify(CS_QUAR_RATE)),
                               /*(char*)*/QT_TR_NOOP(stringify(CS_POPULATION)),
                               /*(char*)*/QT_TR_NOOP(stringify(CS_MAX_STATS))
                           };

    const Qt::GlobalColor PENCOLORS[] = {Qt::red, Qt::green, Qt::blue, Qt::cyan, Qt::magenta,
                                         Qt::darkYellow};
    const quint8 NUMCOLORS = 6;

    template<typename T>
    inline void clampToZero(T& val) {
        if(val < 0) {
            val = (T)0;
        }
    }
}
#endif // CONSTANTS_H
