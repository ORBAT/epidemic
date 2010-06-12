#ifndef CITY_H
#define CITY_H

#include <QObject>


namespace QtEpidemy {

    /*
  Disease models blatantly stolen from http://mvhs1.mbhs.edu/mvhsproj/epidemic/epidemic.html
  with slight modifications: I combined the second and third models so that quarantines are
  also taken into account along with deaths and recoveries
  */

    typedef float ratioType;
    typedef quint32 amountType;


    class City : public QObject
    {
        Q_OBJECT
    public:
        explicit City(amountType population, QObject *parent = 0);


    signals:
        // announce the city's daily status
        void daily_infectious_deaths(amountType);
        void daily_quarantined_deaths(amountType);
        void daily_infectious_recoveries(amountType);
        void daily_quarantined_recoveries(amountType);
        void daily_infections(amountType);
        void daily_quarantines(amountType);

        void susceptible(amountType);
        void infectious(amountType);
        void recovered(amountType);
        void dead(amountType);
        void quarantined(amountType);

    protected:

        struct EpidemicStatistics {

            const float DT = 0.5;  // how many DAYS elapse between steps

            EpidemicStatistics(amountType pop) :
                    days_elapsed(0), survival_rate((ratioType)1),
                    infection_rate((ratioType)0),
                    quarantine_rate((ratioType)0),
                    disease_duration((ratioType)0),
                    daily_infectious_deaths(0),
                    daily_quarantined_deaths(0),
                    daily_infectious_recoveries(0),
                    daily_quarantined_recoveries(0),
                    daily_infections(0),
                    daily_quarantines(0),
                    susceptible(pop),
                    infectious(0),
                    recovered(0),
                    dead(0),
                    quarantined(0)
            {}

            amountType days_elapsed;

            ratioType survival_rate;    // percentage of infected who survive (0-1)
            ratioType infection_rate;   // percentage of susceptible infected daily (0-1)
            ratioType quarantine_rate;  // percentage of infected who are quarantined daily
            ratioType disease_duration; // duration of the disease in days (>0)


            // infected * (1 - survival_rate) / disease_duration
            amountType daily_infectious_deaths; // daily deaths among non-quarantined population

            // quarantined * (1 - survival_rate) / disease_duration
            amountType daily_quarantined_deaths;// daily deaths among the quarantined

            // infectious * survival_rate / disease_duration
            amountType daily_infectious_recoveries;// amount of infectious who recover daily

            // quarantined * survival_rate / disease_duration
            amountType daily_quarantined_recoveries;// amount of quarantined who recover daily

            // infected + susceptible * infection_rate
            amountType daily_infections;   // amount of daily infections

            // infected * quarantine_rate
            amountType daily_quarantines;  // amount of people quarantined daily

            // susceptible(t) = susceptible(t - dt) + (-daily_infections) * dt
            amountType susceptible;        // amount of people susceptible to the disease

            /* infected(t) = infected(t - dt) + (daily_infections - daily_infectious_recoveries -
               daily_quarantines - daily_infectious_deaths) * dt */
            // Starts out at 1
            amountType infectious;         // amount of peolpe who are infectious

            /* recovered(t) = recovered(t - dt) + (daily_infectious_recoveries +
           daily_quarantined_recoveries) * dt */
            amountType recovered;          /* amount of people who have recovered or are
                                              otherwise resistant */

            /* dead(t) = dead(t - dt) + (daily_infectious_deaths +
               daily_quarantined_deaths) * dt */
            amountType dead;               // amount of people who have died

            /* quarantined(t) = quarantined(t - dt) + (daily_quarantines - daily_quarantined_deaths)
           * dt */
            amountType quarantined;        // amount of people quarantined
        };

        EpidemicStatistics *m_stats;

        // calculates the new amount of daily deaths of the infectious
        inline void calcDaily_infectious_deaths() {
            m_stats->daily_infectious_deaths = m_stats->infectious == 0 ? 0 :
                                               (ratioType)m_stats->infectious *
                                               ((ratioType)1 - m_stats->survival_rate);
            emit daily_infectious_deaths(m_stats->daily_infectious_deaths);

        }

        // new amount of daily deaths among the quarantined
        inline void calcDaily_quarantined_deaths() {
            m_stats->daily_quarantined_deaths = m_stats->quarantined == 0 ? 0 :
                                                (ratioType)m_stats->quarantined *
                                                ((ratioType)1 - m_stats->survival_rate);
            emit daily_quarantined_deaths(m_stats->daily_quarantined_deaths);
        }

        inline void calcDaily_infectious_recoveries() {
            m_stats->daily_infectious_recoveries = m_stats->infectious == 0 ? 0 :
                                                   (ratioType)m_stats->infectious *
                                                   m_stats->survival_rate / m_stats->disease_duration;
            emit daily_infectious_recoveries(m_stats->daily_infectious_recoveries);
        }

        inline void calcDaily_quarantined_recoveries() {
            m_stats->daily_quarantined_recoveries = m_stats->quarantined == 0 ? 0 :
                                                    (ratioType)m_stats->quarantined *
                                                    m_stats->survival_rate / m_stats->disease_duration;
            emit daily_quarantined_recoveries(m_stats->daily_quarantined_recoveries);
        }

        inline void calcDaily_infections() {
            m_stats->daily_infections = (m_stats->infectious == 0 ||
                                         m_stats->susceptible == 0)? 0 :
                    (ratioType)m_stats->infectious + ((ratioType)m_stats->susceptible *
                                                      m_stats->infection_rate);
            emit daily_infections(m_stats->daily_infections);
        }

        inline void calcDaily_quarantines() {
            m_stats->daily_quarantines = (m_stats->infectious == 0 ||
                                          m_stats->quarantine_rate == 0) ? 0 :
                    (ratioType)m_stats->infectious * m_stats->quarantine_rate;
            emit daily_quarantines(m_stats->daily_quarantines);
        }

        inline void calcSusceptible() {
            m_stats->susceptible = m_stats->daily_infections == 0 ?
                                   m_stats->susceptible :
                                   m_stats->susceptible -
                                   m_stats->daily_infections * DT;

        }

        inline void calcInfectious() {
            m_stats->infectious = m_stats->infectious +
                                  (m_stats->daily_infections -
                                   m_stats->daily_infectious_recoveries -
                                   m_stats->daily_quarantines -
                                   m_stats->daily_infectious_deaths) * DT;
        }

        inline void calcRecovered() {
            m_stats->recovered = m_stats->recovered +
                                 (m_stats->daily_infectious_recoveries +
                                  m_stats->daily_quarantined_recoveries) * DT;
        }

        inline void calcDead() {
            m_stats->dead = m_stats->dead + (m_stats->daily_infectious_deaths +
                                             m_stats->daily_quarantined_deaths) * DT;
        }

        inline void calcQuarantined() {
            m_stats->quarantined = m_stats->quarantined + (m_stats->daily_quarantines -
                                                           m_stats->daily_quarantined_deaths-
                                                           m_stats->daily_quarantined_recoveries)
                                                           * DT;
        }







    public slots:
        void step();                        // calculate the statistics for the next day


    };

}
#endif // CITY_H
