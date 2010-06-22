#ifndef CITYTABLEMODEL_H
#define CITYTABLEMODEL_H
#include <QAbstractTableModel>
#include <QScopedPointer>
#include <QHash>
#include "constants.h"

namespace QtEpidemy {

    struct CityData {
        QScopedArrayPointer<AmountType> stats;
        CityData() : stats(new AmountType[CS_MAX_STATS]) {
            for(int i = 0; i < CS_MAX_STATS; ++i) {
                stats[i] = 0;
            }
        }
    };

    class CityTableModel : public QAbstractTableModel
    {
        Q_OBJECT
    public:
        CityTableModel(QObject *parent = 0);
        /*
    When subclassing QAbstractTableModel, you must implement rowCount(), columnCount(),
    and data(). Default implementations of the index() and parent() functions are provided by
    QAbstractTableModel. Well behaved models will also implement headerData().
        */
        // from QAbstractItemModel
        virtual int rowCount(const QModelIndex &parent) const;
        virtual int columnCount(const QModelIndex &parent) const;
        virtual QVariant data(const QModelIndex &index, int role) const;
        //////////////////////////

    protected:
        QScopedPointer< QHash<QString, CityData> > m_cityData;
        qint8 m_numColumns; // defaults to name,population,infected

    };

}
#endif // CITYTABLEMODEL_H
