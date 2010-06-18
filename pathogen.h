#ifndef PATHOGEN_H
#define PATHOGEN_H

#include <QObject>
#include <QReadWriteLock>
#include "constants.h"


namespace QtEpidemy {

    class Pathogen : public QObject
    {
        Q_OBJECT
    public:
        explicit Pathogen(RatioType survival = 1, RatioType infection = 0,
                          RatioType duration = 0, QObject *parent = 0);

    public slots:
        void setStatistic(PathogenStats,RatioType);

        void forceStatEmit();


    signals:
        void statChanged(PathogenStats, RatioType);

    protected:
        QReadWriteLock lock;

        // PS_MAX_STATS in constants.h enum PathogenStats
        RatioType m_stats[PS_MAX_STATS];


    };

}

#endif // PATHOGEN_H
