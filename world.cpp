#include "world.h"
#include "citycontroller.h"
namespace QtEpidemy {

    World::World(QObject *parent) :
            QObject(parent), m_cityController(new CityController()),
            m_stepDelay(300),
            m_ticks(0), m_stepTimer(new QTimer(this))
    {
        // have CityController run each City's step() when the timer fires
        connect(m_stepTimer, SIGNAL(timeout()), m_cityController, SLOT(step()));
    }

    World::~World() {
    }


    void World::start() {
        DPR("Starting");
        m_stepTimer->start(m_stepDelay);
    }

    void World::changeStepDelay(int sd) {
        DPR(tr("Step delay changed to %1").arg(sd));
        m_stepDelay = sd;
        m_stepTimer->start(m_stepDelay);
    }

    void World::pause() {
        DPR("Pausing");
        m_stepTimer->stop();
    }

}
