#include "city.h"

namespace QtEpidemy {

    City::City(amountType population, QObject *parent) :
            QObject(parent), m_stats(new EpidemicStatistics(population)), m_days_elapsed(0)
    {

    }



    //// SLOTS
    //////////

    void City::step() {

    }

    void City::changeDiseaseDuration(ratioType rt) {
        clampToZero(rt);
        m_stats->disease_duration = rt;
    }

    void City::changeInfectionRate(ratioType rt) {
        clampToZero(rt);
        m_stats->infection_rate = rt;
    }

    void City::changeQuarantineRate(ratioType rt) {
        clampToZero(rt);
        m_stats->quarantine_rate = rt;
    }

    void City::changeSurvivalRate(ratioType rt) {
        clampToZero(rt);
        m_stats->survival_rate = rt;
    }

}
