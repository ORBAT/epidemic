#ifndef CONSTANTS_H
#define CONSTANTS_H

#endif // CONSTANTS_H

#include <qglobal.h>

namespace QtEpidemy {
    const static float DT = 1.0/24.0;  // how many DAYS elapse between steps

    typedef float ratioType;
    typedef qint32 amountType;
    enum PathogenStats { SURVIVAL, INFECTION, DURATION };

    template<typename T>
    inline void clampToZero(T& val) {
        if(val < 0) {
            val = (T)0;
        }
    }
}
