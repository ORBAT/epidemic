#include <QDebug>
#include "city.h"
#include "pathogen.h"

namespace QtEpidemy {

    City::City(const QString &name, AmountType population, const Position &pos,
               QObject *parent) :
            QObject(parent),
            m_pathogen(NULL),   // no pathogens when we start out
            m_bonusDuration(0.0),
            m_bonusInfectionRate(0.0),
            m_bonusSurvivalRate(0.0),
            m_bonusImmunityLoss(0.0),
            m_quarantineRate(0.0),
            m_survivalRate(0.0),      // from the pathogen
            m_contactRate(0.0),       // ditto
            m_diseaseDuration(0.0),   // same here
            m_immunityLossRate(0.0),  // yup
            m_dailyInfectedDeaths(0.0),
            m_dailyQuarantinedDeaths(0.0),
            m_dailyInfectedRecoveries(0.0),
            m_dailyQuarantinedRecoveries(0.0),
            m_dailyImmunityLoss(0.0),
            m_dailyInfections(0.0),
            m_dailyQuarantines(0.0),
            m_susceptible((RatioType)population),
            m_population((RatioType)population),
            m_infected(0.0),
            m_recovered(0.0),
            m_dead(0.0),
            m_quarantined(0.0),
            m_memberPointers((int)CS_MAX_STATS),
            m_pathStatPointers((int)PS_MAX_STATS),
            m_position(pos)
    {
        //qDebug() << tr("City() %1 constructed with population %2").arg(name).arg(population);
        DPR(tr("%1 constructed with population %2").arg(name).arg(population));

        setObjectName(name);

        // zero the member pointer array
        for(int i = 0; i < CS_MAX_STATS; ++i) {
            m_memberPointers[i] = NULL;
        }

        // zero the array
        for(int i = 0; i < PS_MAX_STATS; ++i) {
            m_pathStatPointers[i] = NULL;
        }

        m_memberPointers[CS_D_INF_DEATHS] = &m_dailyInfectedDeaths;
        m_memberPointers[CS_D_QUAR_DEATHS] = &m_dailyQuarantinedDeaths;
        m_memberPointers[CS_D_INF_RECOVER] = &m_dailyInfectedRecoveries;
        m_memberPointers[CS_D_QUAR_RECOVER] = &m_dailyQuarantinedRecoveries;
        m_memberPointers[CS_D_INFECTIONS] = &m_dailyInfections;
        m_memberPointers[CS_D_QUARANTINES] = &m_dailyQuarantines;
        m_memberPointers[CS_D_IMMUNITYLOSS] = &m_dailyImmunityLoss;
        m_memberPointers[CS_SUSCEPTIBLE] = &m_susceptible;
        m_memberPointers[CS_INFECTED] = &m_infected;
        m_memberPointers[CS_RECOVERED] = &m_recovered;
        m_memberPointers[CS_DEAD] = &m_dead;
        m_memberPointers[CS_QUARANTINED] = &m_quarantined;
        m_memberPointers[CS_QUAR_RATE] = &m_quarantineRate;
        m_memberPointers[CS_POPULATION] = &m_population;

        m_pathStatPointers[PS_CONTACTRATE] = &m_contactRate;
        m_pathStatPointers[PS_DURATION] = &m_diseaseDuration;
        m_pathStatPointers[PS_IMMUNITYLOSSRATE] = &m_immunityLossRate;
        m_pathStatPointers[PS_SURVIVALRATE] = &m_survivalRate;


    }

    City::~City() {
        CDPR("Imma dyin!");
    }

    //// SLOTS
    //////////

    void City::step() {

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
        calcDailyImmunityLoss();

        for(int i = 0; i < CS_MAX_STATS; ++i) { // emit all stats
//            emitStat((CityStats)i);
            emit statUpdate((CityStat)i, *m_memberPointers[i]);
        }

        CDPR(tr("Sum of alive+dead people (should be the same as original pop) %1")
             .arg(m_susceptible+m_infected+m_recovered+m_dead+m_quarantined));

        emit stepped(); /* so that plots know when they need to redraw their data */
    }

    void City::setPathogen(Pathogen *pp) {
        {
            //DPR(tr("City name %1, pathogen %2").arg(m_name).arg((quint32)pp));
            CDPR(tr("Got pathogen %1"));
            if(m_pathogen != NULL) {
                // disconnect us from all of the previous pathogen's signals
                disconnect(m_pathogen, 0, this,0);
            }
        }

        m_pathogen = pp;
        this->connect(m_pathogen, SIGNAL(statChanged(PathogenStat,RatioType)),
                      SLOT(pathogenStatChanged(PathogenStat,RatioType)));
        for(int i = 0; i < PS_MAX_STATS; ++i) {
            CDPR(tr("Setting %1 to %2").arg(PS_NAMES[i]).arg(pp->getStat((PathogenStat)i)));
            pathogenStatChanged((PathogenStat)i, pp->getStat((PathogenStat)i));
        }


    }

    void City::setBonus(const PathogenStat &pst, const RatioType &rt) {

        switch(pst) {
        case PS_SURVIVALRATE:
            m_bonusSurvivalRate = rt;
            break;
        case PS_CONTACTRATE:
            m_bonusInfectionRate = rt;
            break;
        case PS_DURATION:
            m_bonusDuration = rt;
            break;
        case PS_IMMUNITYLOSSRATE:
            m_bonusImmunityLoss = rt;
        default:
            DPR(tr("City %2 got strange ps value %1").arg(pst).arg(objectName()));
        }
    }



//    void City::emitStat(const CityStats &cs) {
////        CDPR(tr("Forced emit of %1").arg(CS_NAMES[cs]));
//        emit statUpdate(cs, *m_memberPointers[cs]);
//    }

    void City::pathogenStatChanged(const PathogenStat &ps, const RatioType &rt) {
//        DPR(tr("%1 stat %2 (%3)").arg(m_name).arg(PS_NAMES[ps]).arg(rt));
        CDPR(tr("Pathogen\'s %1 changed to %2").arg(PS_NAMES[ps]).arg(rt));
        *m_pathStatPointers[ps] = rt;
//        switch(ps) {
//        case PS_DURATION:
//            m_diseaseDuration = rt;
//            break;
//        case PS_CONTACTRATE:
//            m_contactRate = rt;
//            break;
//        case PS_SURVIVALRATE:
//            m_survivalRate = rt;
//            break;
//        case PS_IMMUNITYLOSSRATE:
//            m_immunityLossRate = rt;
//        default:
//            DPR(tr("City %1 got weird ps %2 (%3)").arg(m_name).arg(ps).arg(rt));
//        }
    }

    void City::addInfected(const AmountType &at) {
        m_infected += (RatioType)at;
    }


}
