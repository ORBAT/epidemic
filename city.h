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
        /* I think these are better kept as separate signals. If a single signal like
           statUpdated(CityStats, amountType) was used exclusively, even listeners who don't
           want, say, information about daily infections would receive the signal anyhow */

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

        void statUpdated(CityStats, amountType);

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

        amountType m_quarantineRate;  /* PERCENTAGE (0-100) of infected who are
                                         quarantined daily */


        ratioType m_survivalRate;    // percentage of infected who survive (0-1)
        ratioType m_infectionRate;   // percentage of susceptible infected daily (0-1)
        ratioType m_diseaseDuration; // duration of the disease in days (>0)


        // infected * (1 - survival_rate) / disease_duration
        amountType m_dailyInfectedDeaths; // daily deaths among non-quarantined lation

        // quarantined * (1 - survival_rate) / disease_duration
        amountType m_dailyQuarantinedDeaths;// daily deaths among the quarantined

        // infected * survival_rate / disease_duration
        amountType m_dailyInfectedRecoveries;// amount of infected who recover daily

        // quarantined * survival_rate / disease_duration
        amountType m_dailyQuarantinedRecoveries;// amount of quarantined who recover daily


        // infected * susceptible * infection_rate
        amountType m_dailyInfections;   // amount of daily infections

        // infected * quarantine_rate
        amountType m_dailyQuarantines;  // amount of people quarantined daily

        // susceptible(t) = susceptible(t - dt) + (-daily_infections) * dt
        amountType m_susceptible;        // amount of people susceptible to the disease

        /* population(t) = susceptible(t) + infected(t) + quarantined(t) + recovered(t) +
           dead(t) */
        amountType m_population;         // the current population of the city

        /* infected(t) = infected(t - dt) + (daily_infections - daily_infected_recoveries -
           daily_quarantines - daily_infected_deaths) * dt */
        // Starts out at 1
        amountType m_infected;         // amount of peolpe who are infected

        /* recovered(t) = recovered(t - dt) + (daily_infected_recoveries +
       daily_quarantined_recoveries) * dt */
        amountType m_recovered;          /* amount of people who have recovered or are
                                          otherwise resistant */

        /* dead(t) = dead(t - dt) + (daily_infected_deaths +
           daily_quarantined_deaths) * dt */
        amountType m_dead;               // amount of people who have died


        /* quarantined(t) = quarantined(t - dt) + (daily_quarantines -
           daily_quarantined_deaths - daily_quarantined_recoveries) * dt */
        amountType m_quarantined;        // amount of people quarantined

        /* pointers to all the member variables. This can be used to make setting/getting
           statistics easier (no need for a bazillion setter/getter functions, slots etc */


        QVarLengthArray<amountType*, (int)CS_MAX_STATS> m_amtMembers;



        //// PROTECTED METHODS
        //////////////////////



        /*
         See the EpidemicStatistics struct for explanations of these
         */

        inline void calcDailyInfectedDeaths() {
            amountType val = 0;

            if(m_infected != 0) {
                val = (ratioType)m_infected *
                                ((ratioType)1 - m_survivalRate);
                qDebug() << tr("%3 new %1, old %2").arg(val).arg(m_dailyInfectedDeaths).arg("calcDailyInfectedDeaths();");
            }

            if(val != m_dailyInfectedDeaths) {

                m_dailyInfectedDeaths = val;
                emit dailyInfectedDeaths(val);
            }

        }

        inline void calcDailyQuarantinedDeaths() {
            amountType val = 0;

            if(m_quarantined != 0) {
                val = (ratioType)m_quarantined *
                              ((ratioType)1 - m_survivalRate);
            }

            if(val != m_dailyQuarantinedDeaths) {

                m_dailyQuarantinedDeaths = val;
                emit dailyQuarantinedDeaths(val);
            }
        }

        inline void calcDailyInfectedRecoveries() {
            amountType val = 0;

            if(m_infected != 0) {
                val = (ratioType)m_infected *
                               m_survivalRate / m_diseaseDuration;
                qDebug() << tr("%3 new %1, old %2").arg(val).arg(m_dailyInfectedRecoveries).arg("calcDailyInfectedRecoveries();");
            }

            if(val != m_dailyInfectedRecoveries) {

                m_dailyInfectedRecoveries = val;
                emit dailyInfectedRecoveries(m_dailyInfectedRecoveries);
            }
        }

        inline void calcDailyQuarantinedRecoveries() {
            amountType val = 0;
            if(m_quarantined != 0) {
                val = (ratioType)m_quarantined *
                         m_survivalRate / m_diseaseDuration;
            }

            if(val != m_dailyQuarantinedRecoveries) {

                m_dailyQuarantinedRecoveries = val;
                emit dailyQuarantinedRecoveries(m_dailyQuarantinedRecoveries);
            }
        }

        inline void calcDailyInfections() {
            amountType val = 0;

            if(m_infected != 0 &&  m_susceptible != 0) {
                val = (ratioType)m_infected *
                                      ((ratioType)m_susceptible *
                                       m_infectionRate);
                qDebug() << tr("%3 new %1, old %2").arg(val).arg(m_dailyInfections).arg("calcDailyInfections();");
            }

            if(val != m_dailyInfections) {

                m_dailyInfections = val;
                emit dailyInfections(val);
            }

        }

        inline void calcDailyQuarantines() {
            amountType val = 0;


            ratioType qRate = (ratioType)m_quarantineRate / 100.0;

            if((qRate/100.0) != 0 && m_infected != 0) {
                val = (ratioType)m_infected *
                                  qRate;
            }

            if(val != m_dailyQuarantines) {

                m_dailyQuarantines = val;
                emit dailyQuarantines(val);
            }
        }

        inline void calcSusceptible() {

            if(m_dailyInfections != 0) {
                /* people are susceptible if they're not infected */
                m_susceptible = (ratioType)m_susceptible -
                                (ratioType)m_dailyInfections * DT;
                clampToZero(m_susceptible);

                qDebug() << tr("%2 new %1").arg(m_susceptible).arg("calcSusceptible();");

                emit susceptible(m_susceptible);
            }


        }

        inline void calcInfected() {

            amountType val = m_infected +
                             (m_dailyInfections -
                              m_dailyInfectedRecoveries -
                              m_dailyQuarantines -
                              m_dailyInfectedDeaths) * DT;
            clampToZero(val);
            qDebug() << tr("%3 new %1, old %2").arg(val).arg(m_infected).arg("calcInfected();");
            if(val != m_infected) {
                m_infected = val;
                emit infected(val);
            }
        }

        inline void calcRecovered() {
            amountType val = 0;

            if(m_dailyInfectedRecoveries != 0 &&
               m_dailyQuarantinedRecoveries != 0) {

                val = m_recovered +
                           (m_dailyInfectedRecoveries +
                            m_dailyQuarantinedRecoveries) * DT;
                qDebug() << tr("%3 new %1, old %2").arg(val).arg(m_recovered).arg("calcRecovered();");
            }

            if(val != m_recovered) {
                m_recovered = val;
                emit recovered(val);
            }

        }

        inline void calcDead() {
            amountType val = 0;
            if(m_dailyInfectedDeaths != 0 || m_dailyQuarantinedDeaths != 0) {
                val = m_dead + (m_dailyInfectedDeaths +
                                m_dailyQuarantinedDeaths) * DT;
                qDebug() << tr("%3 new %1, old %2").arg(val).arg(m_dead).arg("calcDead();");
            }

            if(val != m_dead) {
                m_dead = val;
                emit dead(val);
            }

        }

        inline void calcQuarantined() {

            amountType newQuarantined =m_quarantined + (m_dailyQuarantines -
                                                        m_dailyQuarantinedDeaths-
                                                        m_dailyQuarantinedRecoveries)
                    * DT;
            clampToZero(newQuarantined);
            if(newQuarantined != m_quarantined) {

                m_quarantined = newQuarantined;
                emit quarantined(newQuarantined);
            }
        }

        inline void calcPopulation() {

            amountType val = m_susceptible + m_infected + m_recovered +
                             m_quarantined + m_dead;
            qDebug() << tr("%3 new %1, old %2").arg(val).arg(m_population).arg("calcPopulation();");
            if(val  != m_population) {
                m_population = val ;
                emit population(val );
            }
        }







    public slots:
        void step();                        // calculate the statistics for the next day

        void setPathogen(Pathogen*);

        void pathogenStatChanged(PathogenStats, ratioType);

        void setBonus(PathogenStats, ratioType);

        void addInfected(amountType);

        void emitStat(CityStats);                    // forces an emit of a statistic


    };

}
#endif // CITY_H
