#ifndef CONSTANTS_H
#define CONSTANTS_H



#include <qglobal.h>

namespace QtEpidemy {
    const static float DT = 1.0/24.0;  // how many DAYS elapse between steps. 1/24 = 1h

    typedef float ratioType;
    typedef qint32 amountType;

    enum PathogenStats { PS_SURVIVAL, PS_INFECTION, PS_DURATION, PS_MAX_STATS };

    enum CityStats { CS_D_INF_DEATHS, CS_D_QUAR_DEATHS, CS_D_INF_RECOVER, CS_D_QUAR_RECOVER,
                     CS_D_INFECTIONS, CS_D_QUARANTINES, CS_SUSCEPTIBLE, CS_INFECTED,
                     CS_RECOVERED, CS_DEAD, CS_QUARANTINED, CS_QUAR_RATE, CS_MAX_STATS};


    template<typename T>
    inline void clampToZero(T& val) {
        if(val < 0) {
            val = (T)0;
        }
    }
}
#endif // CONSTANTS_H
