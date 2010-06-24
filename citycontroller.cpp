#include <QTimer>
#include "city.h"
#include "citycontroller.h"
#include "position.h"



namespace QtEpidemy {



    CityController::CityController(QObject *parent) :
            QObject(parent), m_tableModel(new CityTableModel(this))
    {


        connect(this, SIGNAL(cityAdded(City*)), m_tableModel,
                SLOT(addCity(City*)));
    }


    CityController::~CityController() {
    }

    void CityController::createCity(const QString &name,
                                    AmountType population,
                                    const Position &position) {

        City* c = new City(name, population, position, this);
        Q_ASSERT_X(!m_cities.contains(name), Q_FUNC_INFO,
                   "Trying to insert a city with duplicate name");

        m_cities[name] = c;
        emit cityAdded(c);

    }

    void CityController::removeCity(const QString &name) {
        City *c = m_cities.take(name);
        if(c)  // ie. QHash::take() returns anything but 0
            emit cityRemoved(c);
    }


    void CityController::step() {
        QHash<QString,City*>::iterator it;
        QHash<QString,City*>::iterator end = m_cities.end();
        for(it = m_cities.begin(); it != end; ++it) {
            it.value()->step();
        }
    }

}
