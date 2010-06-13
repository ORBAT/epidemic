#ifndef CITY_H
#define CITY_H

#include <math.h>
#include <QObject>
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

        void quarantineRate(amountType);    // NOTE: this will emit m_quarantineRate*100

        void population(amountType);

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
        ratioType m_quarantineRate;  /* percentage of infected who are
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




        //// PROTECTED METHODS
        //////////////////////



        /*
         See the EpidemicStatistics struct for explanations of these
         */

        inline void calcDaily_infected_deaths() {
            amountType newDInfDeaths = 0;
            if(m_infected != 0) {
                newDInfDeaths = (ratioType)m_infected *
                                ((ratioType)1 - m_survivalRate);
            }

            if(newDInfDeaths != m_dailyInfectedDeaths) {
                m_dailyInfectedDeaths = newDInfDeaths;
                emit dailyInfectedDeaths(newDInfDeaths);
            }

        }

        inline void calcDaily_quarantined_deaths() {
            amountType newDQDeaths = 0;
            if(m_quarantined != 0) {
                newDQDeaths = (ratioType)m_quarantined *
                              ((ratioType)1 - m_survivalRate);
            }

            if(newDQDeaths != m_dailyQuarantinedDeaths) {
                m_dailyQuarantinedDeaths = newDQDeaths;
                emit dailyQuarantinedDeaths(newDQDeaths);
            }
        }

        inline void calcDaily_infected_recoveries() {
            amountType newDIRecover = 0;
            if(m_infected != 0) {
                newDIRecover = (ratioType)m_infected *
                               m_survivalRate / m_diseaseDuration;
            }

            if(newDIRecover != m_dailyInfectedRecoveries) {
                m_dailyInfectedRecoveries = newDIRecover;
                emit dailyInfectedRecoveries(m_dailyInfectedRecoveries);
            }
        }

        inline void calcDaily_quarantined_recoveries() {
            m_dailyQuarantinedRecoveries = m_quarantined == 0 ? 0 :
                                                    (ratioType)m_quarantined *
                                                    m_survivalRate / m_diseaseDuration;
            emit dailyQuarantinedRecoveries(m_dailyQuarantinedRecoveries);
        }

        inline void calcDaily_infections() {
            amountType newDInfections = 0;
            if(m_infected != 0 &&  m_susceptible != 0) {
                newDInfections = ceil((ratioType)m_infected *
                                      ((ratioType)m_susceptible *
                                       m_infectionRate));
            }

            if(newDInfections != m_dailyInfections) {
                m_dailyInfections = newDInfections;
                emit dailyInfections(newDInfections);
            }

        }

        inline void calcDaily_quarantines() {
            amountType newDQuarantines = 0;
            if(m_quarantineRate != 0 && m_infected != 0) {
                newDQuarantines = (ratioType)m_infected *
                                  m_quarantineRate;
            }

            if(newDQuarantines != m_dailyQuarantines) {
                m_dailyQuarantines = newDQuarantines;
                emit dailyQuarantines(newDQuarantines);
            }
        }

        inline void calcSusceptible() {


            if(m_dailyInfections != 0) {
                /* people are susceptible if they're not infected */
                m_susceptible = (ratioType)m_susceptible -
                                       (ratioType)m_dailyInfections * DT;
                clampToZero(m_susceptible);
                emit susceptible(m_susceptible);
            }


        }

        inline void calcInfected() {
            amountType newinfected = m_infected +
                                     (m_dailyInfections -
                                      m_dailyInfectedRecoveries -
                                      m_dailyQuarantines -
                                      m_dailyInfectedDeaths) * DT;
            clampToZero(newinfected);
            if(newinfected != m_infected) {
                m_infected = newinfected;
                emit infected(newinfected);
            }
        }

        inline void calcRecovered() {
            amountType newRecovered = 0;
            if(m_dailyInfectedRecoveries != 0 &&
               m_dailyQuarantinedRecoveries != 0) {

                newRecovered = ceil(m_recovered +
                                    (m_dailyInfectedRecoveries +
                                     m_dailyQuarantinedRecoveries) * DT);
            }

            if(newRecovered != m_recovered) {
                m_recovered = newRecovered;
                emit recovered(newRecovered);
            }

        }

        inline void calcDead() {
            amountType newDead = m_dead + (m_dailyInfectedDeaths +
                                                  m_dailyQuarantinedDeaths) * DT;
            if(newDead != m_dead) {
                m_dead = newDead;
                emit dead(newDead);
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
            amountType newPopulation = m_susceptible + m_infected + m_recovered +
                                       m_quarantined + m_dead;
            if(newPopulation != m_population) {
                m_population = newPopulation;
                emit population(newPopulation);
            }
        }







    public slots:
        void step();                        // calculate the statistics for the next day

        void setPathogen(Pathogen*);

        void pathogenStatChanged(PathogenStats, ratioType);

        void setBonus(PathogenStats, ratioType);

        void emitStat(CityStats);                    // forces an emit of a statistic


    };

}
#endif // CITY_H
