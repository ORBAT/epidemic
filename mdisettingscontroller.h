#ifndef MDISETTINGSCONTROLLER_H
#define MDISETTINGSCONTROLLER_H

#include <QObject>
#include <QCheckBox>
#include <QList>
#include "constants.h"


class QGridLayout;

namespace QtEpidemy {

    class MdiSettingsController : public QObject
    {
        Q_OBJECT
    public:
        explicit MdiSettingsController(QGridLayout *&settingsGrid, QObject *parent = 0);

    signals:
        // emitted when a checkbox is poked
        void checked(CityStats, bool);

    public slots:
        // set the checked state of a specific stat's checkbox
        void setChecked(CityStats, bool state = true);

    protected:

        // the grid we need to populate
        QGridLayout *m_settingsGrid;
        QList<QCheckBox*> m_checkboxes;




    };

}

#endif // MDISETTINGSCONTROLLER_H
