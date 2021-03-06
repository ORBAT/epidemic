#ifndef CITYCONTROLLER_H
#define CITYCONTROLLER_H

#include <QObject>
#include <QList>
#include <QHash>
#include <QStringList>
#include <QAbstractItemModel>
#include "citytablemodel.h"

#include "constants.h"

class QTimer;

namespace QtEpidemy {

    class City;
    class CityTableModel;
    class Position;

    /**
      This class is in charge of all City-related stuff, like adding and
      removing cities and calling City::step() when needed.
    */
    class CityController : public QObject
    {
        Q_OBJECT
    public:
        explicit CityController(QObject *parent = 0);
        ~CityController();

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

        inline QAbstractItemModel* getModel() const {
            return static_cast<QAbstractItemModel*>(m_tableModel);
        }

        void createCity(const QString &name, AmountType population,
                        const Position &position);
        void removeCity(const QString &name);

    signals:
        void cityAdded(City*);
        void cityRemoved(City*);

        //// SLOTS
        //////////
    public slots:

        // runs step() on each City
        void step();

        //// PROTECTED MEMBERS
        //////////////////////
    protected:
        // all cities are stored by name. ***Names MUST BE UNIQUE***
        QHash<QString, City*> m_cities;

        /* For Views, and when I get up to it all plots should get their
           data from this too */
        CityTableModel *m_tableModel;

    };

}

#endif // CITYCONTROLLER_H
