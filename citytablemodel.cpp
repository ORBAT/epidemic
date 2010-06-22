#include "citytablemodel.h"

namespace QtEpidemy {

    CityTableModel::CityTableModel(QObject *parent) : QAbstractTableModel(parent),
    m_cityData(new QHash<QString,CityData>),
    m_numColumns(3)
    {
    }

    int CityTableModel::rowCount(const QModelIndex &parent) const {
        Q_UNUSED(parent);
        /* Qt docs for QAbstractItemModel:
               "Note: When implementing a table based model, columnCount() should
               return 0 when the parent is valid." */
        if(parent.isValid())
            return 0;
        return m_cityData->keys().size();
    }

    int CityTableModel::columnCount(const QModelIndex &parent) const {
        /* Qt docs for QAbstractItemModel:
               "Note: When implementing a table based model, columnCount() should
               return 0 when the parent is valid." */
        if(parent.isValid())
            return 0;
    }

    QVariant CityTableModel::data(const QModelIndex &index, int role) const {

    }

}
