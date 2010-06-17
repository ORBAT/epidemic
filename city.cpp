#include <QDebug>
#include "city.h"
#include "pathogen.h"

namespace QtEpidemy {

    City::City(QString name, amountType population, QObject *parent) :
            QObject(parent),
            m_pathogen(NULL),   // no pathogens when we start out
            m_name(name),
            m_bonusDuration(0.0),
            m_bonusInfectionRate(0.0),
            m_bonusSurvivalRate(0.0),
            m_quarantineRate(0.0),
            m_survivalRate(0.0),      // from the pathogen
            m_infectionRate(0.0),     // ditto
            m_diseaseDuration(0.0),   // same here
            m_dailyInfectedDeaths(0.0),
            m_dailyQuarantinedDeaths(0.0),
            m_dailyInfectedRecoveries(0.0),
            m_dailyQuarantinedRecoveries(0.0),
            m_dailyInfections(0.0),
            m_dailyQuarantines(0.0),
            m_susceptible((ratioType)population),
            m_population((ratioType)population),
            m_infected(0.0),
            m_recovered(0.0),
            m_dead(0.0),
            m_quarantined(0.0),
            m_memberPointers((int)CS_MAX_STATS)
    {
        //qDebug() << tr("City() %1 constructed with population %2").arg(name).arg(population);
        DPR(tr("%1 constructed with population %2").arg(name).arg(population));

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

        DPR("start step");

        /* calculations of statistics should be done so that total statistics are calculated
           first and daily statistics last */
        calcDead();
        calcQuarantined();
        calcSusceptible();
        calcInfected();
        calcRecovered();
        calcPopulation();

        calcDailyInfectedDeaths();
        calcDailyQuarantinedDeaths();
        calcDailyInfectedRecoveries();
        calcDailyQuarantinedRecoveries();
        calcDailyInfections();
        calcDailyQuarantines();

        for(int i = 0; i < CS_MAX_STATS; ++i) { // emit all stats
            emitStat((CityStats)i);
        }

        CDPR(tr("Sum of alive+dead people (should be the same as original pop) %1")
             .arg(m_susceptible+m_infected+m_recovered+m_dead+m_quarantined));

        emit stepped(); /* so that plots know when they need to redraw their data */
    }

    void City::setPathogen(Pathogen *pp) {
        {

            DPR(tr("City name %1, pathogen %2").arg(m_name).arg((quint32)pp));

            if(m_pathogen != NULL) {
                // disconnect us from all of the previous pathogen's signals
                disconnect(m_pathogen, 0, this,0);
            }
        }

        m_pathogen = pp;
        this->connect(m_pathogen, SIGNAL(statChanged(PathogenStats,ratioType)),
                      SLOT(pathogenStatChanged(PathogenStats,ratioType)));

    }

    void City::setBonus(const PathogenStats &pst, const ratioType &rt) {

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
            DPR(tr("City %2 got strange ps value %1").arg(pst).arg(m_name));
        }
    }



    void City::emitStat(const CityStats &cs) {
        CDPR(tr("Forced emit of %1").arg(CS_NAMES[cs]));
        emit statUpdate(cs, *m_memberPointers[cs]);
    }

    void City::pathogenStatChanged(const PathogenStats &ps, const ratioType &rt) {
        DPR(tr("%1 stat %2 (%3)").arg(m_name).arg(PS_NAMES[ps]).arg(rt));
        switch(ps) {
        case PS_DURATION:
            m_diseaseDuration = rt;
            break;
        case PS_INFECTION:
            m_infectionRate = rt;
            break;
        case PS_SURVIVAL:
            m_survivalRate = rt;
            break;
        default:
            DPR(tr("City %1 got weird ps %2 (%3)").arg(m_name).arg(ps).arg(rt));
        }
    }

    void City::addInfected(const amountType &at) {
        m_infected += (ratioType)at;
    }


}
