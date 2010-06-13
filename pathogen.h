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
        explicit Pathogen(ratioType survival = 1, ratioType infection = 0,
                          ratioType duration = 0, QObject *parent = 0);

    public slots:
        void setStatistic(PathogenStats,ratioType);

        void forceStatEmit();


    signals:
        void statChanged(PathogenStats, ratioType);

    protected:
        QReadWriteLock lock;

        // PS_MAX_STATS in constants.h enum PathogenStats
        ratioType m_stats[PS_MAX_STATS];


    };

}

#endif // PATHOGEN_H
