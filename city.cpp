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
            m_population(population),
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
        if(m_pathogen != NULL) {
            // disconnect us from all of the previous pathogen's signals
            disconnect(m_pathogen, 0, this,0);
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
            qDebug() << "changeBonus() got strange PathogenStats value" << pst;
        }
    }



    void City::emitStat(CityStats cs) {
        switch(cs) {
        case CS_D_INF_DEATHS:
            qDebug() << "City::emitStat() CS_D_INF_DEATHS";
            emit dailyInfectedDeaths(m_dailyInfectedDeaths);
            break;
        case CS_D_QUAR_DEATHS:
            qDebug() << "City::emitStat() CS_D_QUAR_DEATHS";
            emit dailyQuarantinedDeaths(m_dailyQuarantinedDeaths);
            break;
        case CS_D_INF_RECOVER:
            qDebug() << "City::emitStat() CS_D_INF_RECOVER";
            emit dailyInfectedRecoveries(m_dailyInfectedRecoveries);
            break;
        case CS_D_QUAR_RECOVER:
            qDebug() << "City::emitStat() CS_D_QUAR_RECOVER";
            emit dailyQuarantinedRecoveries(m_dailyQuarantinedRecoveries);
            break;
        case CS_D_INFECTIONS:
            qDebug() << "City::emitStat() CS_D_INFECTIONS";
            emit dailyInfections(m_dailyInfections);
            break;
        case CS_D_QUARANTINES:
            qDebug() << "City::emitStat() CS_D_QUARANTINES";
            emit dailyQuarantines(m_dailyQuarantines);
            break;
        case CS_SUSCEPTIBLE:
            qDebug() << "City::emitStat() CS_SUSCEPTIBLE";
            emit susceptible(m_susceptible);
            break;
        case CS_INFECTED:
            qDebug() << "City::emitStat() CS_INFECTED";
            emit infected(m_infected);
            break;
        case CS_RECOVERED:
            qDebug() << "City::emitStat() CS_RECOVERED";
            emit recovered(m_recovered);
            break;
        case CS_DEAD:
            qDebug() << "City::emitStat() CS_DEAD";
            emit dead(m_dead);
            break;
        case CS_QUARANTINED:
            qDebug() << "City::emitStat() CS_QUARANTINED";
            emit quarantined(m_quarantined);
            break;
        case CS_QUAR_RATE:
            qDebug() << "City::emitStat() CS_QUAR_RATE";
            // Note the *100 used to convert the rate to a percentage. Stupid, huh?
            emit quarantineRate((amountType)(m_quarantineRate*100));
            break;
        default:
            qDebug() << "City::emitStat() got weird cs" << cs;
        }
    }

    void City::pathogenStatChanged(PathogenStats ps, ratioType rt) {
        switch(ps) {
        case PS_DURATION:
            qDebug() << "City::pathogenStatChanged() PS_DURATION"<<rt;
            m_diseaseDuration = rt;
            break;
        case PS_INFECTION:
            qDebug() << "City::pathogenStatChanged() PS_INFECTION"<<rt;
            m_infectionRate = rt;
            break;
        case PS_SURVIVAL:
            qDebug() << "City::pathogenStatChanged() PS_SURVIVAL"<<rt;
            m_survivalRate = rt;
            break;
        default:
            qDebug() << "City::pathogenStatChanged() got weird ps" << ps << "value"<<rt;
        }
    }



}
