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
            m_survivalRate((ratioType)0),      // from the pathogen
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
            m_quarantined(0),
            m_amtMembers((int)CS_MAX_STATS)
    {
        qDebug() << tr("City() %1 constructed with population %2").arg(name).arg(population);

        // zero the array
        for(int i = 0; i < CS_MAX_STATS; ++i) {
            m_amtMembers[i] = (amountType)0;
        }

        m_amtMembers[CS_D_INF_DEATHS] = &m_dailyInfectedDeaths;
        m_amtMembers[CS_D_QUAR_DEATHS] = &m_dailyQuarantinedDeaths;
        m_amtMembers[CS_D_INF_RECOVER] = &m_dailyInfectedRecoveries;
        m_amtMembers[CS_D_QUAR_RECOVER] = &m_dailyQuarantinedRecoveries;
        m_amtMembers[CS_D_INFECTIONS] = &m_dailyInfections;
        m_amtMembers[CS_D_QUARANTINES] = &m_dailyQuarantines;
        m_amtMembers[CS_SUSCEPTIBLE] = &m_susceptible;
        m_amtMembers[CS_INFECTED] = &m_infected;
        m_amtMembers[CS_RECOVERED] = &m_recovered;
        m_amtMembers[CS_DEAD] = &m_dead;
        m_amtMembers[CS_QUARANTINED] = &m_quarantined;
        m_amtMembers[CS_QUAR_RATE] = &m_quarantineRate;


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

        calcPopulation();
    }

    void City::setPathogen(Pathogen *pp) {
        {
            qDebug() << "City" << m_name <<  "City::setPathogen()" << pp;
            QReadLocker rl(&lock);
            if(m_pathogen != NULL) {
                // disconnect us from all of the previous pathogen's signals
                disconnect(m_pathogen, 0, this,0);
            }
        }
        QWriteLocker wl(&lock);
        m_pathogen = pp;
        this->connect(m_pathogen, SIGNAL(statChanged(PathogenStats,ratioType)),
                      SLOT(pathogenStatChanged(PathogenStats,ratioType)));

    }

    void City::setBonus(PathogenStats pst, ratioType rt) {
        QWriteLocker wl(&lock);
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
        QWriteLocker wl(&lock);
        switch(cs) {
        case CS_D_INF_DEATHS:
            qDebug() << "City" << m_name <<  "City::emitStat() CS_D_INF_DEATHS";
            emit dailyInfectedDeaths(m_dailyInfectedDeaths);
            break;
        case CS_D_QUAR_DEATHS:
            qDebug() << "City" << m_name <<  "City::emitStat() CS_D_QUAR_DEATHS";
            emit dailyQuarantinedDeaths(m_dailyQuarantinedDeaths);
            break;
        case CS_D_INF_RECOVER:
            qDebug() << "City" << m_name <<  "City::emitStat() CS_D_INF_RECOVER";
            emit dailyInfectedRecoveries(m_dailyInfectedRecoveries);
            break;
        case CS_D_QUAR_RECOVER:
            qDebug() << "City" << m_name <<  "City::emitStat() CS_D_QUAR_RECOVER";
            emit dailyQuarantinedRecoveries(m_dailyQuarantinedRecoveries);
            break;
        case CS_D_INFECTIONS:
            qDebug() << "City" << m_name <<  "City::emitStat() CS_D_INFECTIONS";
            emit dailyInfections(m_dailyInfections);
            break;
        case CS_D_QUARANTINES:
            qDebug() << "City" << m_name <<  "City::emitStat() CS_D_QUARANTINES";
            emit dailyQuarantines(m_dailyQuarantines);
            break;
        case CS_SUSCEPTIBLE:
            qDebug() << "City" << m_name <<  "City::emitStat() CS_SUSCEPTIBLE";
            emit susceptible(m_susceptible);
            break;
        case CS_INFECTED:
            qDebug() << "City" << m_name <<  "City::emitStat() CS_INFECTED";
            emit infected(m_infected);
            break;
        case CS_RECOVERED:
            qDebug() << "City" << m_name <<  "City::emitStat() CS_RECOVERED";
            emit recovered(m_recovered);
            break;
        case CS_DEAD:
            qDebug() << "City" << m_name <<  "City::emitStat() CS_DEAD";
            emit dead(m_dead);
            break;
        case CS_QUARANTINED:
            qDebug() << "City" << m_name <<  "City::emitStat() CS_QUARANTINED";
            emit quarantined(m_quarantined);
            break;
        case CS_QUAR_RATE:
            qDebug() << "City" << m_name <<  "City::emitStat() CS_QUAR_RATE";
            emit quarantineRate(m_quarantineRate);
            break;
        default:
            qDebug() << "City" << m_name <<  "City::emitStat() got weird cs" << cs;
        }
    }

    void City::pathogenStatChanged(PathogenStats ps, ratioType rt) {
        QWriteLocker wl(&lock);
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



}
