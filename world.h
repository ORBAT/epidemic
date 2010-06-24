#ifndef WORLD_H
#define WORLD_H

#include <QObject>
#include "constants.h"

class QTimer;

namespace QtEpidemy {

    class CityController;

    class World : public QObject
    {
        Q_OBJECT
    public:
        explicit World(QObject *parent = 0);



    signals:

    public slots:
        // makes time pass
        void start();
        // pauses time
        void pause();

        // changes the delay between steps
        void changeStepDelay(int);
    protected:
        CityController *m_cityController;

        // milliseconds between steps. Defaults to 300
        int m_stepDelay;
        // how many ticks have elapsed
        quint32 m_ticks;
        QTimer* m_stepTimer;

    };

}

#endif // WORLD_H

