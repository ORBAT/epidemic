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
        /* Qt docs for QAbstractItemModel:
               "Note: When implementing a table based model, columnCount() should
               return 0 when the parent is valid." */
        DPR(tr("Row count currently %1").arg(m_cityDataIndex.size()));
        if(parent.isValid()) {
            DPR("Parent valid");
            return 0;
        }

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
        int row = index.row();

        if(row >= m_cityDataIndex.size())
            return QVariant();


        if(role == Qt::DisplayRole) {
            DPR(tr("Data wanted for row %1").arg(row));
            int col = index.column();
            switch(col) {
            case 0: // name
                return m_cityDataIndex.at(row)->getName();
            case 1: // population
                return QLocale::system().toString(m_cityDataIndex.at(row)->getStat(CS_POPULATION));
            case 2: // infected
                return QLocale::system().toString(m_cityDataIndex.at(row)->getStat(CS_INFECTED));
            default:
                return QVariant();
            }
        } else
            return QVariant(); // all other roles
    }

    QVariant CityTableModel::headerData(int section, Qt::Orientation orientation,
                        int role) const {
        if(role != Qt::DisplayRole)
            return QVariant();
        if(orientation == Qt::Horizontal) {
            switch(section) {
            case 0:
                return tr("Name");
            case 1:
                return CS_NAMES[CS_POPULATION];
            case 2:
                return CS_NAMES[CS_INFECTED];
            default:
                return QVariant();
            }
        } else { // Qt::Vertical
            if(section < m_cityDataIndex.size())
                return section+1;
            else
                return QVariant();
        }

    }


    //// SLOTS
    //////////
    void CityTableModel::addCity(City* c) {
        DPR(tr("Adding %1 to model").arg(c->getName()));

        int idxOfNewCity = m_cityDataIndex.size();
        beginInsertRows(QModelIndex(), idxOfNewCity,idxOfNewCity);
        m_cityDataIndex.append(c);
        endInsertRows();
        connect(c, SIGNAL(statChanged(CityStat,AmountType)),
                SLOT(statUpdated(CityStat,AmountType)));

    }

    void CityTableModel::statUpdated(CityStat cs, AmountType at) {
        Q_UNUSED(cs);
        Q_UNUSED(at);
        City* c = qobject_cast<City*>(sender());
//        DPR(tr("%1 stat %2 changed to %3").arg(c->getName()).arg(CS_NAMES[cs]).arg(at));
        /* if the sender wasn't a City, we can forget about searching for it
           in our index */
        int row = c ? m_cityDataIndex.indexOf(c) : -1;

        if(row != -1) { /* if the sender was a City and the City was found in
                           the index */
            QModelIndex i;
            switch(cs) {
            case CS_POPULATION:
                i = this->index(row, 1);
                emit dataChanged(i,i);
                break;
            case CS_INFECTED:
                i = this->index(row, 2);
                emit dataChanged(i,i);
                break;
            default:
                break;
            }
        }
    }
}
