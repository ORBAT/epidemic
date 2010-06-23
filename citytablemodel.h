#ifndef CITYTABLEMODEL_H
#define CITYTABLEMODEL_H
#include <QAbstractTableModel>
#include <QScopedPointer>
#include <QHash>
#include "constants.h"

namespace QtEpidemy {

    class City;

    class CityTableModel : public QAbstractTableModel
    {
        Q_OBJECT
    public:
        CityTableModel(QObject *parent = 0);
        ~CityTableModel();

        /*
    When subclassing QAbstractTableModel, you must implement rowCount(), columnCount(),
    and data(). Default implementations of the index() and parent() functions are provided by
    QAbstractTableModel. Well behaved models will also implement headerData().
        */
        // from QAbstractItemModel
        virtual int rowCount(const QModelIndex &parent) const;
        virtual int columnCount(const QModelIndex &parent) const;
        virtual QVariant data(const QModelIndex &index, int role) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation,
                                    int role) const;
        //////////////////////////

    public slots:
        virtual void addCity(City* c);
        // receives updates from City instances
        virtual void statUpdated(CityStat, AmountType);

    protected:
        QList<City*> m_cityDataIndex;
        qint8 m_numColumns; // name,population,infected currently

    };

}
#endif // CITYTABLEMODEL_H
