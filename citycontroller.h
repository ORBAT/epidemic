#ifndef CITYCONTROLLER_H
#define CITYCONTROLLER_H

#include <QObject>
#include <QList>
#include <QHash>
#include <QStringList>

#include "constants.h"

class QTimer;
class QPointF;

namespace QtEpidemy {

    class City;

    class CityController : public QObject
    {
        Q_OBJECT
    public:
        explicit CityController(QObject *parent = 0);
        ~CityController();

        inline TickType getTicks() const {
            return m_ticks;
        }

        inline City * getCity(const QString &name) const {
            return m_cities.value(name);
        }

        inline City * getCity(const QString &name) {
            return m_cities.value(name);
        }

        inline QStringList getCityNames() const {
            return m_cities.keys();
        }

        inline QList<City*> getCities() const {
            return m_cities.values();
        }


    signals:
        void cityAdded(const QString&);
        void cityRemoved(const QString&);

        //// SLOTS
        //////////
    public slots:

        void start();

        void pause();

        // changes the delay between steps
        void changeStepDelay(int);

        void createCity(const QString &name, AmountType population, const QPointF &position);
        void removeCity(const QString &name);

        // runs step() on each City
        void step();

        //// PROTECTED MEMBERS
        //////////////////////
    protected:
        // all cities are stored by name. ***Names MUST BE UNIQUE***
        QHash<QString, City*> m_cities;
        QTimer* m_stepTimer;

        // milliseconds between steps. Defaults to 300
        int m_stepDelay;
        // how many ticks have elapsed
        quint32 m_ticks;


    };

}

#endif // CITYCONTROLLER_H
