#include <QDebug>
#include <QWriteLocker>
#include <QReadLocker>

#include "pathogen.h"


namespace QtEpidemy {

    Pathogen::Pathogen(RatioType survivalR, RatioType contactR, RatioType duration,
                       QObject *parent) :
    QObject(parent), m_stats()
    {
        m_stats[PS_DURATION] = duration;
        m_stats[PS_CONTACTRATE] = contactR;
        m_stats[PS_SURVIVALRATE] = survivalR;
        qDebug() << tr("Pathogen created with survival rate %1, infection %2, duration %3 days")
                .arg(survivalR).arg(contactR).arg(duration);
    }

    void Pathogen::setStatistic(PathogenStat ps, RatioType rt) {
        qDebug() << "Pathogen::setStatistic() ps" << ps << "rt" << rt;
        clampToZero<RatioType>(rt);
        QWriteLocker locker(&lock);
        m_stats[ps] = rt;
        emit statChanged(ps, rt);
    }


    void Pathogen::forceStatEmit() {
        qDebug() << "Pathogen::forceStatEmit()";
        QReadLocker locker(&lock);
        for(int i = 0; i < PS_MAX_STATS; ++i) {
            emit statChanged((PathogenStat)i, m_stats[i]);
        }
    }

    const RatioType* Pathogen::getPathogenStats() const {
        QReadLocker locker(&lock);
        return m_stats;
    }

    RatioType Pathogen::getStat(PathogenStat ps) const {
        QReadLocker locker(&lock);
        return m_stats[ps];
    }

}
