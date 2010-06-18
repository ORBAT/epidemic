#ifndef MDISETTINGSCONTROLLER_P_H
#define MDISETTINGSCONTROLLER_P_H

#include <QObject>
#include <QVariant>
#include "constants.h"
#include "mdisettingscontroller.h"

namespace QtEpidemy {

    class MdiSettingsControllerPrivate : public QObject {
        Q_OBJECT
    private:
        MdiSettingsControllerPrivate(MdiSettingsController *parent) : QObject(parent),
        qp(parent) {
            DPR("Inited");
        }

        MdiSettingsController *qp;

    private slots:
        void checkBoxToggled(bool checked) {
            QVariant qv = sender()->property("citystat");
            DPR(tr("Checkbox with stat %1 toggled to %2").arg(CS_NAMES[qv.toInt()]).arg(checked));

            /* if the sender actually has a "citystat" property. Note that this DOESN'T
               check whether the property is an int or not */
            if(qv.isValid()) {
                emit qp->checked(qv.toInt(), checked);
            }
        }


        friend MdiSettingsController::MdiSettingsController(QGridLayout *&, QObject *);
    };
}

#endif // MDISETTINGSCONTROLLER_P_H
