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

        const RatioType* getPathogenStats() const;

        RatioType getStat(PathogenStat ps) const;

    public slots:
        void setStatistic(PathogenStat,RatioType);

        void forceStatEmit();


    signals:
        void statChanged(PathogenStat, RatioType);

    protected:
        mutable QReadWriteLock lock;

        /* PS_MAX_STATS in constants.h enum PathogenStats.

           The current statistics are as follows:
           PS_CONTACTRATE       contact rate, ie. the probability of getting the
                                disease when in contact with an infected person.

           PS_DURATION          duration of the disease in days

           PS_SURVIVALRATE      survival rate, ie. the probability of surviving
                                once infected. A lower survival rate leads to a
                                higher visibility.

           PS_IMMUNITYLOSSRATE  average loss of immunity rate for recovered
                                individuals

           PS_VISIBILITY        how noticeable the infection is. 0-1.

           */
        RatioType m_stats[PS_MAX_STATS];


    };

}

#endif // PATHOGEN_H
