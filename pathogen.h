#ifndef PATHOGEN_H
#define PATHOGEN_H

#include <QObject>
#include "constants.h"

namespace QtEpidemy {

    class Pathogen : public QObject
    {
        Q_OBJECT
    public:
        explicit Pathogen(ratioType survival = 1, ratioType infection = 0,
                          ratioType duration = 0, QObject *parent = 0);

    public slots:
        void setSurvivalRate(ratioType);
        void setInfectionRate(ratioType);
        void setDiseaseDuration(ratioType);


    signals:
        void changed(Pathogen const *);

    protected:
         ratioType m_survivalRate;
         ratioType m_infectionRate;
         ratioType m_diseaseDuration;

    public slots:

         void setStatistic(PathogenStats,ratioType);

    };

}

#endif // PATHOGEN_H
