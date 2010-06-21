#include <QGridLayout>
#include <QSignalMapper>
#include <QVariant>
#include "mdisettingscontroller.h"
#include "mdisettingscontroller_p.h"

namespace QtEpidemy {

    MdiSettingsController::MdiSettingsController(QGridLayout *&grid, QWidget *&parentWidget,
                                                 QObject *parent) : QObject(parent),
                                                 pd(new MdiSettingsControllerPrivate(this))
    {
        pd->initCheckBoxes(grid, parentWidget);
    }


    void MdiSettingsController::setChecked(CityStat cs, bool state) {
        pd->setChecked(cs, state);
    }

}
