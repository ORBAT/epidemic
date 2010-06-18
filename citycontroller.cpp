#include <QTimer>
#include <QPoint>
#include "city.h"
#include "citycontroller.h"


namespace QtEpidemy {

    CityController::CityController(QObject *parent) :
            QObject(parent), m_stepTimer(new QTimer(this)), m_stepDelay(300),
            m_ticks(0)
    {
        connect(m_stepTimer, SIGNAL(timeout()), this, SLOT(step()));
    }

    CityController::~CityController() {
        m_stepTimer->stop();
        QHash<QString,City*>::iterator it;
        QHash<QString,City*>::iterator end = m_cities.end();
        for(it = m_cities.begin(); it != end; ++it) {
            delete *it;
        }
    }

    void CityController::createCity(const QString &name,
                               AmountType population, const QPoint &position) {

        City* c = new City(name, population, position, this);
        Q_ASSERT_X(!m_cities.contains(name), Q_FUNC_INFO,
                   "Trying to insert a city with duplicate name");

        m_cities[name] = c;
    }

    void CityController::start() {
        DPR("Starting");
        m_stepTimer->start(m_stepDelay);
    }

    void CityController::changeStepDelay(int sd) {
        m_stepDelay = sd;
        m_stepTimer->start(m_stepDelay);
    }

    void CityController::pause() {
        DPR("Pausing");
        m_stepTimer->stop();
    }

    void CityController::step() {
        QHash<QString,City*>::iterator it;
        QHash<QString,City*>::iterator end = m_cities.end();
        for(it = m_cities.begin(); it != end; ++it) {
            it.value()->step();
        }
    }

}
