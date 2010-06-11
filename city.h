#ifndef CITY_H
#define CITY_H

#include <QObject>

/*
  Disease models blatantly stolen from http://mvhs1.mbhs.edu/mvhsproj/epidemic/epidemic.html
  with slight modifications. I combined the second and third models so that quarantines are
  also taken into account
  */

class City : public QObject
{
    Q_OBJECT
public:
    explicit City(QObject *parent = 0);


protected:

    float   m_survival_rate;            // percentage of people who survive (0-1)
    float   m_infection_rate;           // the rate at which the infection spreads (0-1)
    float   m_disease_duration;         // duration of the disease in days (>0)


    quint32 m_susceptible;              // total amount of people susceptible to the disease
    quint32 m_infected;                 // amount of peolpe infected
    quint32 m_recovered;                // amount of people who recovered & resistant
    quint32 m_dead;                     // amount of people who have died
    quint32 m_quarantined;              // amount of people quarantined



signals:

public slots:

};

#endif // CITY_H
