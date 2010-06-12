#include "city.h"

namespace QtEpidemy {

    City::City(amountType population, QObject *parent) :
            QObject(parent), m_stats(new EpidemicStatistics(population))
    {

    }

    void City::step() {

    }

}
