#ifndef WORLD_H
#define WORLD_H

#include <QObject>
#include "constants.h"
#include "citycontroller.h"
#include "city.h"

class QTimer;

namespace QtEpidemy {

    class World : public QObject
    {
        Q_OBJECT
    public:
        explicit World(QObject *parent = 0);
        ~World();

        inline TickType getTicks() const {
            return m_ticks;
        }

        inline CityController* getCityController() const {
            return m_cityController;
        }

        void createCities();

        inline QList<City*> getCities() {
            return m_cityController->getCities();
        }

        inline City* getCity(const QString& name) const {
            return m_cityController->getCity(name);
        }


    signals:

    public slots:
        // makes time pass
        void start();
        // pauses time
        void pause();

        // changes the delay between steps
        void changeStepDelay(int);

        void step();



    protected:
        /* the CityController class is the one actually in charge of all
           City-related stuff */
        CityController *m_cityController;

        // milliseconds between steps. Defaults to 300
        int m_stepDelay;
        // how many ticks have elapsed
        TickType m_ticks;
        QTimer* m_stepTimer;

    };

}

#endif // WORLD_H

