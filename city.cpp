#include <QDebug>
#include "city.h"

namespace QtEpidemy {

    City::City(QString name, amountType population, QObject *parent) :
            QObject(parent),
            m_pathogen(NULL),   // no pathogens when we start out
            m_name(name),
            m_bonusDuration(0),
            m_bonusInfectionRate(0),
            m_bonusSurvivalRate(0),
            m_quarantineRate(0),
            m_survivalRate((ratioType)1),      // from the pathogen
            m_infectionRate((ratioType)0),     // ditto
            m_diseaseDuration((ratioType)0),   // same here
            m_dailyInfectedDeaths(0),
            m_dailyQuarantinedDeaths(0),
            m_dailyInfectedRecoveries(0),
            m_dailyQuarantinedRecoveries(0),
            m_dailyInfections(0),
            m_dailyQuarantines(0),
            m_susceptible(population),
            m_infected(0),
            m_recovered(0),
            m_dead(0),
            m_quarantined(0)
    {
        qDebug() << tr("City() %1 constructed with population %1").arg(name).arg(population);
    }



    //// SLOTS
    //////////

    void City::step() {

        /*
         These calculations are done in this order to prevent people who get infected
         from getting quarantined or dying during the same step
        */

        // calculate daily amounts at the start of each step
        calcDaily_infected_deaths();
        calcDaily_quarantined_deaths();

        calcDaily_infected_recoveries();
        calcDaily_quarantined_recoveries();

        calcDaily_infections();
        calcDaily_quarantines();

        // then kill off some infected
        calcDead();

        // then a little quarantining
        calcQuarantined();

        // next the new values of susceptible/infected (order doesn't matter)
        calcSusceptible();
        calcInfected();
    }

    void City::setPathogen(Pathogen *pp) {
        m_pathogen = pp;

    }

    void City::setBonus(PathogenStats pst, ratioType rt) {
        switch(pst) {
        case SURVIVAL:
            m_bonusSurvivalRate = rt;
            break;
        case INFECTION:
            m_bonusInfectionRate = rt;
            break;
        case DURATION:
            m_bonusDuration = rt;
            break;
        default:
            qDebug() << "changeBonus() got strange PathogenStats value" << pst;
        }
    }



}
