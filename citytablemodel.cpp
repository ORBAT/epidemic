#include "citytablemodel.h"
#include "city.h"

namespace QtEpidemy {

    CityTableModel::CityTableModel(QObject *parent) : QAbstractTableModel(parent),
    m_numColumns(3)
    {
    }

    CityTableModel::~CityTableModel() {

    }

    int CityTableModel::rowCount(const QModelIndex &parent) const {
        Q_UNUSED(parent);
        /* Qt docs for QAbstractItemModel:
               "Note: When implementing a table based model, columnCount() should
               return 0 when the parent is valid." */
        if(parent.isValid())
            return 0;
        return m_cityDataIndex.size();
    }

    int CityTableModel::columnCount(const QModelIndex &parent) const {
        /* Qt docs for QAbstractItemModel:
               "Note: When implementing a table based model, columnCount() should
               return 0 when the parent is valid." */
        if(parent.isValid())
            return 0;
        return m_numColumns;
    }

    QVariant CityTableModel::data(const QModelIndex &index, int role) const {
        if(!index.isValid())
            return QVariant();
        if(index.row() >= m_cityDataIndex.size())
            return QVariant();

        if(role == Qt::DisplayRole) {
            int row = index.row();
            int col = index.column();
            switch(col) {
            case 0: // name
                return m_cityDataIndex.at(row)->getName();
            case 1: // population
                return m_cityDataIndex.at(row)->getStat(CS_POPULATION);
            case 2: // infected
                return m_cityDataIndex.at(row)->getStat(CS_INFECTED);
            default:
                return QVariant();
            }
        }
    }

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const {
        if(role != Qt::DisplayRole)
            return QVariant();
        if(orientation == Qt::Horizontal) {
            switch(section) {
            case 0:
                return tr("Name");
            case 1:
                return tr(CS_NAMES[CS_POPULATION]);
            case 2:
                return tr(CS_NAMES[CS_INFECTED]);
            default:
                return QVariant();
            }
        } else { // Qt::Vertical
            return section+1;
        }

    }


    //// SLOTS
    //////////

    void CityTableModel::addCity(City *c, const QString& name) {
        m_cityDataIndex.append(c);
        connect(c, SIGNAL(statChanged(CityStat,AmountType)),
                SLOT(statUpdated(CityStat,AmountType)));
    }

    void CityTableModel::statUpdated(CityStat cs, AmountType at) {
        Q_UNUSED(cs);
        Q_UNUSED(at);
        emit dataChanged(/*TODO*/);
    }

}
