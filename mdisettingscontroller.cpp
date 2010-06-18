#include <QGridLayout>
#include <QSignalMapper>
#include <QVariant>
#include "mdisettingscontroller.h"
#include "mdisettingscontroller_p.h"

namespace QtEpidemy {

    MdiSettingsController::MdiSettingsController(QGridLayout *&grid, QObject *parent) :
            QObject(parent), m_settingsGrid(grid),
            pd(new MdiSettingsControllerPrivate(this)),
            MAXCOLUMNS(3)
    {

        QCheckBox *temp;
        for(int i = 0; i < CS_MAX_STATS; ++i) {
            DPR(tr("Creating checkbox for %1").arg(CS_NAMES[i]));

            temp = new QCheckBox(CS_NAMES[i], qobject_cast<QWidget*>(grid->parent()));

            m_settingsGrid->addWidget(temp, i/MAXCOLUMNS, i%MAXCOLUMNS);

            m_checkboxes.append(temp);
            /* every checkbox is associated with a certain CityStat, so might
               as well set it as a property */
            temp->setProperty("citystat", QVariant::fromValue(i));
            connect(temp, SIGNAL(toggled(bool)), pd, SLOT(checkBoxToggled(bool)));
        }
    }


    void MdiSettingsController::setChecked(CityStats cs, bool state) {
        DPR(tr("Setting %1 to %2").arg(CS_NAMES[cs]).arg(state));
        m_checkboxes.at(cs)->setChecked(state);
    }

}
