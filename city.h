#ifndef CITY_H
#define CITY_H

#include <math.h>
#include <qdebug.h>
#include <QObject>
#include <QVarLengthArray>
#include "constants.h"


namespace QtEpidemy {

    class Pathogen;

    /*
  Disease models blatantly stolen from http://mvhs1.mbhs.edu/mvhsproj/epidemic/epidemic.html
  with slight modifications: I combined the second and third models so that quarantines are
  also taken into account along with deaths and recoveries
  */



    class City : public QObject
    {
        Q_OBJECT
    public:
        explicit City(QString name, amountType population, QObject *parent = 0);

    signals:
        // announce the city's daily status


        void dailyInfectedDeaths(amountType);
        void dailyQuarantinedDeaths(amountType);
        void dailyInfectedRecoveries(amountType);
        void dailyQuarantinedRecoveries(amountType);
        void dailyInfections(amountType);
        void dailyQuarantines(amountType);

        void susceptible(amountType);
        void infected(amountType);
        void recovered(amountType);
        void dead(amountType);
        void quarantined(amountType);
        void quarantineRate(amountType);
        void population(amountType);

        void statUpdate(CityStats, amountType);
        void statChanged(CityStats);

        void stepped();

        //// PROTECTED
        //////////////
    protected:

        //// PROTECTED MEMBERS
        //////////////////////


        Pathogen *m_pathogen;        // our friendly affliction, if any

        QString m_name;

        // City-specific bonuses (can be positive OR negative) to some stats + quarantine rate
        ratioType m_bonusDuration;
        ratioType m_bonusInfectionRate;
        ratioType m_bonusSurvivalRate;

        ratioType m_quarantineRate;  /* PERCENTAGE (0-100) of infected who are
                                         quarantined daily */


        ratioType m_survivalRate;    // percentage of infected who survive (0-1)
        ratioType m_infectionRate;   // percentage of susceptible infected daily (0-1)
        ratioType m_diseaseDuration; // duration of the disease in days (>0)


        // infected * (1 - survival_rate) / disease_duration
        ratioType m_dailyInfectedDeaths; // daily deaths among non-quarantined lation

        // quarantined * (1 - survival_rate) / disease_duration
        ratioType m_dailyQuarantinedDeaths;// daily deaths among the quarantined

        // infected * survival_rate / disease_duration
        ratioType m_dailyInfectedRecoveries;// amount of infected who recover daily

        // quarantined * survival_rate / disease_duration
        ratioType m_dailyQuarantinedRecoveries;// amount of quarantined who recover daily


        // infected * susceptible * infection_rate
        ratioType m_dailyInfections;   // amount of daily infections

        // infected * quarantine_rate
        ratioType m_dailyQuarantines;  // amount of people quarantined daily

        // susceptible(t) = susceptible(t - dt) + (-daily_infections) * dt
        ratioType m_susceptible;        // amount of people susceptible to the disease

        /* population(t) = susceptible(t) + infected(t) + quarantined(t) + recovered(t) +
           dead(t) */
        ratioType m_population;         // the current population of the city

        /* infected(t) = infected(t - dt) + (daily_infections - daily_infected_recoveries -
           daily_quarantines - daily_infected_deaths) * dt */
        // Starts out at 1
        ratioType m_infected;         // amount of peolpe who are infected

        /* recovered(t) = recovered(t - dt) + (daily_infected_recoveries +
       daily_quarantined_recoveries) * dt */
        ratioType m_recovered;          /* amount of people who have recovered or are
                                          otherwise resistant */

        /* dead(t) = dead(t - dt) + (daily_infected_deaths +
           daily_quarantined_deaths) * dt */
        ratioType m_dead;               // amount of people who have died


        /* quarantined(t) = quarantined(t - dt) + (daily_quarantines -
           daily_quarantined_deaths - daily_quarantined_recoveries) * dt */
        ratioType m_quarantined;        // amount of people quarantined

        /* pointers to all the member variables. This can be used to make setting/getting
           statistics easier (no need for a bazillion setter/getter functions, slots etc */


        QVarLengthArray<ratioType*, (int)CS_MAX_STATS> m_memberPointers;



        //// PROTECTED METHODS
        //////////////////////



        /*
         See the EpidemicStatistics struct for explanations of these
         */

        /*
         calculates the amount of daily deaths in the given group
         */
        inline ratioType calcDailyDeathsForGroup(const ratioType &grp) {
            return grp * (1.0 - m_survivalRate) / m_diseaseDuration;
        }

        /*
         calculates the amount of daily recoveries in the given group
         */
        inline ratioType calcDailyRecoveriesForGroup(const ratioType &grp) {
            return grp * m_survivalRate/m_diseaseDuration;
        }

        /* if the new value is different from the old value, change the old value and
           emit the statChanged signal */
        inline void emitAndChangeIfDiff(const ratioType &newVal, ratioType &orig,
                                        const CityStats &cs) {
            if(newVal != orig) {
                orig = newVal;
                emit statChanged(cs);
            }
        }

        inline void calcDailyInfectedDeaths() {
            ratioType val = 0;

            if(m_infected != 0) {
                val = calcDailyDeathsForGroup(m_infected);
            }

            CDPR_STAT(val, m_dailyInfectedDeaths);

            emitAndChangeIfDiff(val, m_dailyInfectedDeaths, CS_D_INF_DEATHS);
//            if(val != m_dailyInfectedDeaths) {
//                m_dailyInfectedDeaths = val;
//                emit statChanged(CS_D_INF_DEATHS);
//            }

        }

        inline void calcDailyQuarantinedDeaths() {
            ratioType val = 0;

            if(m_quarantined != 0) {
                val = calcDailyDeathsForGroup(m_quarantined);
            }

            emitAndChangeIfDiff(val, m_dailyQuarantinedDeaths, CS_D_QUAR_DEATHS);
//            if(val != m_dailyQuarantinedDeaths) {
//                m_dailyQuarantinedDeaths = val;
//                emit statChanged(CS_D_QUAR_DEATHS);
//            }
        }

        inline void calcDailyInfectedRecoveries() {
            ratioType val = 0;

            if(m_infected != 0) {
                val = m_infected *
                               m_survivalRate / m_diseaseDuration;
                qDebug() << tr("%3 new %1, old %2").arg(val).arg(m_dailyInfectedRecoveries).arg("calcDailyInfectedRecoveries();");
            }

            if(val != m_dailyInfectedRecoveries) {
                m_dailyInfectedRecoveries = val;
                emit statChanged(CS_D_INF_RECOVER);
            }
        }

        inline void calcDailyQuarantinedRecoveries() {
            ratioType val = 0;
            if(m_quarantined != 0) {
                val = m_quarantined *
                         m_survivalRate / m_diseaseDuration;
            }

            if(val != m_dailyQuarantinedRecoveries) {
                m_dailyQuarantinedRecoveries = val;
                emit statChanged(CS_D_QUAR_RECOVER);
            }
        }

        inline void calcDailyInfections() {
            ratioType val = 0;

            if(m_infected != 0 &&  m_susceptible != 0) {
                val = m_infected * m_susceptible * m_infectionRate;
            }
            /* make sure the amount of daily infections is never larger than the amount of
               people who are susceptible */
            val = std::min<ratioType>(val, m_susceptible);
            CDPR(tr("%1").arg(val));
            if(val != m_dailyInfections) {

                m_dailyInfections = val;
                emit statChanged(CS_D_INFECTIONS);
            }

        }

        inline void calcDailyQuarantines() {
            ratioType val = 0;

            if(m_quarantineRate != 0 && m_infected != 0) {
                val = m_infected * m_quarantineRate;
                CDPR_STAT(val, m_dailyQuarantines);
            }

            emitAndChangeIfDiff(val,m_dailyQuarantines,CS_D_QUARANTINES);
        }

        inline void calcSusceptible() {

            if(m_dailyInfections != 0) {

                m_susceptible = m_susceptible -
                                (m_dailyInfections * DT);

                clampToZero<ratioType>(m_susceptible);
                CDPR(QString::number(m_susceptible));
                emit statChanged(CS_SUSCEPTIBLE);
            }


        }

        inline void calcInfected() {

            ratioType val = m_infected +
                            ((m_dailyInfections -
                              m_dailyInfectedRecoveries -
                              m_dailyQuarantines -
                              m_dailyInfectedDeaths) * DT);

            CDPR_STAT(val,m_infected);
            clampToZero<ratioType>(val);
            emitAndChangeIfDiff(val,m_infected,CS_INFECTED);
        }

        inline void calcRecovered() {
            ratioType val = 0;

            if(m_dailyInfectedRecoveries != 0 ||
               m_dailyQuarantinedRecoveries != 0) {

                val = m_recovered + ((m_dailyInfectedRecoveries +
                                      m_dailyQuarantinedRecoveries) * DT);

            }
            CDPR_STAT(val,m_recovered);
            emitAndChangeIfDiff(val,m_recovered, CS_INFECTED);
        }

        inline void calcDead() {
            ratioType val = 0;
            if(m_dailyInfectedDeaths != 0 || m_dailyQuarantinedDeaths != 0) {
                val = m_dead + (m_dailyInfectedDeaths +
                                m_dailyQuarantinedDeaths) * DT;
            }
            CDPR_STAT(val,m_dead);
            emitAndChangeIfDiff(val, m_dead, CS_DEAD);

        }

        inline void calcQuarantined() {

            ratioType val =m_quarantined + (m_dailyQuarantines -
                                            m_dailyQuarantinedDeaths -
                                            m_dailyQuarantinedRecoveries)
                                         * DT;
            CDPR_STAT(val,m_quarantined);

            clampToZero<ratioType>(val);

            emitAndChangeIfDiff(val,m_quarantined,CS_QUARANTINED);
        }

        inline void calcPopulation() {
            // Count every person who's still alive
            ratioType val = m_susceptible + m_infected + m_recovered +
                             m_quarantined;
            CDPR_STAT(val,m_population);
            emitAndChangeIfDiff(val, m_population, CS_POPULATION);
        }







    public slots:
        void step();                        // calculate the statistics for the next day

        void setPathogen(Pathogen*);

        void pathogenStatChanged(const PathogenStats&, const ratioType&);

        void setBonus(const PathogenStats&, const ratioType&);

        void addInfected(const amountType&);

        void emitStat(const CityStats&);                    // forces an emit of a statistic


    };

}
#endif // CITY_H
