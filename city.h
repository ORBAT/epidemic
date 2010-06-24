#ifndef CITY_H
#define CITY_H

#include <math.h>
#include <qdebug.h>
#include <QObject>
#include <QVarLengthArray>
#include "constants.h"
#include "position.h"


namespace QtEpidemy {

    class Pathogen;

    /*
      Epidemic model mostly from http://en.wikipedia.org/wiki/Epidemic_model with some
      modifications
    */





    class City : public QObject
    {
        Q_OBJECT

    //// PUBLIC METHODS
    ///////////////////
    public:
        explicit City(const QString &name, AmountType population,
                      const Position &pos = Position(1.0,1.0), QObject *parent = 0);

        ~City();

        // thread-safe since m_position is immutable
        inline const Position getPosition() const {
            return m_position;
        }

        inline const Pathogen* getPathogen() const {
            return m_pathogen;
        }

        inline AmountType getStat(CityStat cs) const {
            return *m_memberPointers[cs];
        }

        // thread-safe since m_name is immutable
        inline QString getName() const {
            return m_name;
        }


        //// SIGNALS
        ////////////
    signals:
        // announce the city's daily status
        void statUpdate(CityStat, AmountType);
        // only emitted if a stat actually changes
        void statChanged(CityStat, AmountType);

        void stepped();

        //// PROTECTED
        //////////////
    protected:

        //// PROTECTED MEMBERS
        //////////////////////


        Pathogen *m_pathogen;        // our friendly affliction, if any

        const QString m_name;

        // City-specific bonuses (can be positive OR negative) to some stats
        RatioType m_bonusDuration;
        RatioType m_bonusInfectionRate;
        RatioType m_bonusSurvivalRate;
        RatioType m_bonusImmunityLoss;

        RatioType m_quarantineRate;  /* probability of an infected getting
                                        quarantined */


        /*
         We get these from the Pathogen. They're just stored here for
         faster access */
        RatioType m_survivalRate;    // probability of survival (0-1)
        RatioType m_contactRate;     // probability of infection (0-1)
        RatioType m_diseaseDuration; // duration of the disease in days (>0)
        RatioType m_immunityLossRate;// avg. recovered loss of immunity rate (0-1)


        // infected * (1 - survival_rate) / disease_duration
        RatioType m_dailyInfectedDeaths; // daily deaths among non-quarantined lation

        // quarantined * (1 - survival_rate) / disease_duration
        RatioType m_dailyQuarantinedDeaths;// daily deaths among the quarantined

        // infected * survival_rate / disease_duration
        RatioType m_dailyInfectedRecoveries;// amount of infected who recover daily

        // quarantined * survival_rate / disease_duration
        RatioType m_dailyQuarantinedRecoveries;// amount of quarantined who recover daily

        // recovered * m_immunityLossRate
        RatioType m_dailyImmunityLoss; // amount of recovered who lose immunity daily



        // infected * susceptible * infection_rate
        RatioType m_dailyInfections;   // amount of daily infections

        // infected * quarantine_rate
        RatioType m_dailyQuarantines;  // amount of people quarantined daily

        /* susceptible(t) = susceptible(t - dt) - daily_infections +
           dailyImmunityLoss * dt */
        RatioType m_susceptible;        // amount of people susceptible to the disease

        /* population(t) = susceptible(t) + infected(t) + quarantined(t) + recovered(t) +
           dead(t) */
        RatioType m_population;         // the current population of the city

        /* infected(t) = infected(t - dt) + (daily_infections - daily_infected_recoveries -
           daily_quarantines - daily_infected_deaths) * dt */
        // Starts out at 1
        RatioType m_infected;         // amount of peolpe who are infected

        /* recovered(t) = recovered(t - dt) + (daily_infected_recoveries +
       daily_quarantined_recoveries - dailyImmunityLoss) * dt */
        RatioType m_recovered;          /* amount of people who have recovered.
                                           Recovered people are always immune.*/

        /* dead(t) = dead(t - dt) + (daily_infected_deaths +
           daily_quarantined_deaths) * dt */
        RatioType m_dead;               // amount of people who have died


        /* quarantined(t) = quarantined(t - dt) + (daily_quarantines -
           daily_quarantined_deaths - daily_quarantined_recoveries) * dt */
        RatioType m_quarantined;        // amount of people quarantined

        /* pointers to all the member variables. This can be used to make setting/getting
           statistics easier (no need for a bazillion setter/getter functions, slots etc */
        QVarLengthArray<RatioType*, (int)CS_MAX_STATS> m_memberPointers;
        // same as above but for Pathogens.
        QVarLengthArray<RatioType*, (int)PS_MAX_STATS> m_pathStatPointers;

        const Position m_position;



        //// PROTECTED METHODS
        //////////////////////



        /*
         See the EpidemicStatistics struct for explanations of these
         */

        /*
         calculates the amount of daily deaths in the given group
         */
        inline RatioType calcDailyDeathsForGroup(const RatioType &grp) {
            return grp * (1.0 - m_survivalRate) / m_diseaseDuration;
        }

        /*
         calculates the amount of daily recoveries in the given group
         */
        inline RatioType calcDailyRecoveriesForGroup(const RatioType &grp) {
            return grp * m_survivalRate/m_diseaseDuration;
        }

        /* if the new value is different from the old value, change the old value and
           emit the statChanged signal */
        inline void emitAndChangeIfDiff(const RatioType &newVal, RatioType &orig,
                                        const CityStat &cs) {

            if(newVal != orig) {
//                CDPR_STAT(newVal, orig);
                CDPR(tr("%1 old %2 new %3").arg(CS_NAMES[cs]).arg(orig).arg(newVal));
                orig = newVal;
                emit statChanged(cs, (AmountType)newVal);
            }
        }

        inline void calcDailyInfectedDeaths() {
            RatioType val = 0;

            if(m_infected != 0) {
                val = calcDailyDeathsForGroup(m_infected);
            }

            emitAndChangeIfDiff(val, m_dailyInfectedDeaths, CS_D_INF_DEATHS);
        }

        inline void calcDailyQuarantinedDeaths() {
            RatioType val = 0;

            if(m_quarantined != 0) {
                val = calcDailyDeathsForGroup(m_quarantined);
            }

            emitAndChangeIfDiff(val, m_dailyQuarantinedDeaths, CS_D_QUAR_DEATHS);
        }

        inline void calcDailyInfectedRecoveries() {
            RatioType val = 0;

            if(m_infected != 0) {
                val = m_infected * m_survivalRate / m_diseaseDuration;
            }
            emitAndChangeIfDiff(val,m_dailyInfectedRecoveries,CS_D_INF_RECOVER);
        }

        inline void calcDailyQuarantinedRecoveries() {
            RatioType val = 0;
            if(m_quarantined != 0) {
                val = m_quarantined * m_survivalRate / m_diseaseDuration;
            }
            emitAndChangeIfDiff(val, m_dailyQuarantinedRecoveries, CS_D_QUAR_RECOVER);
        }

        inline void calcDailyInfections() {
            RatioType val = 0;

            if(m_infected != 0 &&  m_susceptible != 0) {
                val = m_infected * m_susceptible * m_contactRate;
            }
            /* make sure the amount of daily infections is never larger than the amount of
               people who are susceptible */
            val = std::min<RatioType>(val, m_susceptible);

            emitAndChangeIfDiff(val, m_dailyInfections, CS_D_INFECTIONS);
        }

        inline void calcDailyQuarantines() {
            RatioType val = 0;

            if(m_quarantineRate != 0 && m_infected != 0) {
                val = m_infected * m_quarantineRate;
            }

            emitAndChangeIfDiff(val,m_dailyQuarantines,CS_D_QUARANTINES);
        }

        inline void calcDailyImmunityLoss() {
            RatioType val = 0;

            if(m_immunityLossRate != 0) {
                val = m_recovered * m_immunityLossRate;
            }
            emitAndChangeIfDiff(val, m_dailyImmunityLoss, CS_D_IMMUNITYLOSS);
        }

        inline void calcSusceptible() {
            if(m_dailyInfections != 0) {
                RatioType val = 0;
                val  = m_susceptible -
                       m_dailyInfections + m_dailyImmunityLoss * DT;

                clampToZero<RatioType>(val);
                emitAndChangeIfDiff(val, m_susceptible, CS_SUSCEPTIBLE);
            }


        }

        inline void calcInfected() {

            RatioType val = m_infected +
                            ((m_dailyInfections -
                              m_dailyInfectedRecoveries -
                              m_dailyQuarantines -
                              m_dailyInfectedDeaths) * DT);

            clampToZero<RatioType>(val);
            emitAndChangeIfDiff(val,m_infected,CS_INFECTED);
        }

        inline void calcRecovered() {
            RatioType val = 0;

            if(m_dailyInfectedRecoveries != 0 ||
               m_dailyQuarantinedRecoveries != 0) {

                val = m_recovered + ((m_dailyInfectedRecoveries +
                                      m_dailyQuarantinedRecoveries -
                                      m_dailyImmunityLoss) * DT);

            }
            clampToZero<RatioType>(val);
            emitAndChangeIfDiff(val,m_recovered, CS_RECOVERED);
        }

        inline void calcDead() {
            RatioType val = 0;
            if(m_dailyInfectedDeaths != 0 || m_dailyQuarantinedDeaths != 0) {
                val = m_dead + (m_dailyInfectedDeaths +
                                m_dailyQuarantinedDeaths) * DT;
            }
            emitAndChangeIfDiff(val, m_dead, CS_DEAD);

        }

        inline void calcQuarantined() {

            RatioType val =m_quarantined + (m_dailyQuarantines -
                                            m_dailyQuarantinedDeaths -
                                            m_dailyQuarantinedRecoveries)
                                         * DT;

            clampToZero<RatioType>(val);

            emitAndChangeIfDiff(val,m_quarantined,CS_QUARANTINED);
        }

        inline void calcPopulation() {
            // Count every person who's still alive
            RatioType val = m_susceptible + m_infected + m_recovered +
                             m_quarantined;
            emitAndChangeIfDiff(val, m_population, CS_POPULATION);
        }





        //// SLOTS
        //////////

    public slots:
        void step();                        // calculate the statistics for the next day

        void setPathogen(Pathogen*);

        void pathogenStatChanged(const PathogenStat&, const RatioType&);

        void setBonus(const PathogenStat&, const RatioType&);

        void addInfected(const AmountType&);

//        void emitStat(const CityStats&);                    // forces an emit of a statistic

    };

}
#endif // CITY_H
