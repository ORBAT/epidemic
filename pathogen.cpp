#include <QDebug>


#include "pathogen.h"


namespace QtEpidemy {

    Pathogen::Pathogen(ratioType survival, ratioType infection, ratioType duration,
                       QObject *parent) :
    QObject(parent), m_stats()
    {
        m_stats[PS_DURATION] = duration;
        m_stats[PS_INFECTION] = infection;
        m_stats[PS_SURVIVAL] = survival;
        qDebug() << tr("Pathogen created with survival rate %1, infection %2, duration %2 days")
                .arg(survival).arg(infection).arg(duration);
    }

    void Pathogen::setStatistic(PathogenStats ps, ratioType rt) {
        clampToZero(rt);
        m_stats[ps] = rt;
        emit statChanged(ps, rt);
    }


    void Pathogen::forceStatEmit() {
        QReadLocker locker(&lock);
        for(int i = 0; i < PS_MAX_STATS; ++i) {
            emit statChanged((PathogenStats)i, m_stats[i]);
        }
    }

}
