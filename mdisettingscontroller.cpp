#include <QGridLayout>
#include "mdisettingscontroller.h"

namespace QtEpidemy {

    MdiSettingsController::MdiSettingsController(QGridLayout *&grid, QObject *parent) :
            QObject(parent), m_settingsGrid(grid)
    {

    }


    MdiSettingsController::setChecked(CityStats cs, bool state) {

    }

}
