#ifndef CITYTABLEMODEL_H
#define CITYTABLEMODEL_H
#include <QAbstractTableModel>
#include <QHash>
#include "constants.h"

namespace QtEpidemy {

    struct CityData {

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

    protected:
        QList<CityData> m_citydata;

    };

}
#endif // CITYTABLEMODEL_H
