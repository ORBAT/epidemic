#include <QDebug>
#include "city.h"
#include "pathogen.h"

namespace QtEpidemy {

    City::City(QString name, amountType population, QObject *parent) :
            QObject(parent),
            m_pathogen(NULL),   // no pathogens when we start out
            m_name(name),
            m_bonusDuration(0),
            m_bonusInfectionRate(0),
            m_bonusSurvivalRate(0),
            m_quarantineRate(0),
            m_survivalRate(0),      // from the pathogen
            m_infectionRate(0),     // ditto
            m_diseaseDuration(0),   // same here
            m_dailyInfectedDeaths(0),
            m_dailyQuarantinedDeaths(0),
            m_dailyInfectedRecoveries(0),
            m_dailyQuarantinedRecoveries(0),
            m_dailyInfections(0),
            m_dailyQuarantines(0),
            m_susceptible(population),
            m_population(population),
            m_infected(0),
            m_recovered(0),
            m_dead(0),
            m_quarantined(0),
            m_memberPointers((int)CS_MAX_STATS)
    {
        qDebug() << tr("City() %1 constructed with population %2").arg(name).arg(population);

        // zero the array
        for(int i = 0; i < CS_MAX_STATS; ++i) {
            m_memberPointers[i] = NULL;
        }

        m_memberPointers[CS_D_INF_DEATHS] = &m_dailyInfectedDeaths;
        m_memberPointers[CS_D_QUAR_DEATHS] = &m_dailyQuarantinedDeaths;
        m_memberPointers[CS_D_INF_RECOVER] = &m_dailyInfectedRecoveries;
        m_memberPointers[CS_D_QUAR_RECOVER] = &m_dailyQuarantinedRecoveries;
        m_memberPointers[CS_D_INFECTIONS] = &m_dailyInfections;
        m_memberPointers[CS_D_QUARANTINES] = &m_dailyQuarantines;
        m_memberPointers[CS_SUSCEPTIBLE] = &m_susceptible;
        m_memberPointers[CS_INFECTED] = &m_infected;
        m_memberPointers[CS_RECOVERED] = &m_recovered;
        m_memberPointers[CS_DEAD] = &m_dead;
        m_memberPointers[CS_QUARANTINED] = &m_quarantined;
        m_memberPointers[CS_QUAR_RATE] = &m_quarantineRate;
        m_memberPointers[CS_POPULATION] = &m_population;


    }



    //// SLOTS
    //////////

    void City::step() {

        qDebug("City::step()");
        /*
         These calculations are done in this order to prevent people who get infected
         from getting quarantined or dying during the same step
        */

        // calculate daily amounts at the start of each step
        calcDailyInfectedDeaths();

        calcDailyQuarantinedDeaths();

        calcDailyInfectedRecoveries();

        calcDailyQuarantinedRecoveries();

        calcDailyInfections();

        calcDailyQuarantines();


        // then kill off some infected
        calcDead();

        // then a little quarantining
        calcQuarantined();

        // next the new values of susceptible/infected (order doesn't matter)
        calcSusceptible();

        calcInfected();

        calcPopulation();

        for(int i = 0; i < CS_MAX_STATS; ++i) { // emit all stats
            emitStat((CityStats)i);
        }

        emit stepped(); /* so that plots know when they need to redraw their data */
    }

    void City::setPathogen(Pathogen *pp) {
        {
            qDebug() << "City" << m_name <<  "City::setPathogen()" << pp;

            if(m_pathogen != NULL) {
                // disconnect us from all of the previous pathogen's signals
                disconnect(m_pathogen, 0, this,0);
            }
        }

        m_pathogen = pp;
        this->connect(m_pathogen, SIGNAL(statChanged(PathogenStats,ratioType)),
                      SLOT(pathogenStatChanged(PathogenStats,ratioType)));

    }

    void City::setBonus(PathogenStats pst, ratioType rt) {

        switch(pst) {
        case PS_SURVIVAL:
            m_bonusSurvivalRate = rt;
            break;
        case PS_INFECTION:
            m_bonusInfectionRate = rt;
            break;
        case PS_DURATION:
            m_bonusDuration = rt;
            break;
        default:
            qDebug() << "City" << m_name << "changeBonus() got strange PathogenStats value" << pst;
        }
    }



    void City::emitStat(CityStats cs) {
        qDebug() << tr("%1 emitting cs %2 (%3)").arg("City::emitStat();").arg(cs).arg((amountType)*m_memberPointers[cs]);
        emit statUpdate(cs, *m_memberPointers[cs]);
    }

    void City::pathogenStatChanged(PathogenStats ps, ratioType rt) {

        switch(ps) {
        case PS_DURATION:
            qDebug() << "City" << m_name <<  "City::pathogenStatChanged() PS_DURATION"<<rt;
            m_diseaseDuration = rt;
            break;
        case PS_INFECTION:
            qDebug() << "City" << m_name <<  "City::pathogenStatChanged() PS_INFECTION"<<rt;
            m_infectionRate = rt;
            break;
        case PS_SURVIVAL:
            qDebug() << "City" << m_name <<  "City::pathogenStatChanged() PS_SURVIVAL"<<rt;
            m_survivalRate = rt;
            break;
        default:
            qDebug() << "City" << m_name <<  "City::pathogenStatChanged() got weird ps" << ps << "value"<<rt;
        }
    }

    void City::addInfected(amountType at) {
        m_infected += at;
    }


}
