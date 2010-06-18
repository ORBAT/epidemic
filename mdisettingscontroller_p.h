#ifndef MDISETTINGSCONTROLLER_P_H
#define MDISETTINGSCONTROLLER_P_H

#include <QObject>
#include <QVariant>
#include <QGridLayout>
#include "constants.h"
#include "mdisettingscontroller.h"


namespace QtEpidemy {

    class MdiSettingsControllerPrivate : public QObject {
        Q_OBJECT
    private:
        MdiSettingsControllerPrivate(MdiSettingsController *parent) : QObject(parent),
        qp(parent), MAXCOLUMNS(3) {
            DPR("Inited");
        }

        MdiSettingsController *qp;
        // the grid we need to populate
        QList<QCheckBox*> m_checkboxes;

        const int MAXCOLUMNS;

        void initCheckBoxes(QGridLayout *&grid, QWidget *&cbParent) {
            QCheckBox *temp;
            for(int i = 0; i < CS_MAX_STATS; ++i) {
                DPR(tr("Creating checkbox for %1").arg(CS_NAMES[i]));

                temp = new QCheckBox(CS_NAMES[i], cbParent);

                grid->addWidget(temp, i/MAXCOLUMNS, i%MAXCOLUMNS);

                m_checkboxes.append(temp);
                /* every checkbox is associated with a certain CityStat, so might
                   as well set it as a property */
                temp->setProperty("citystat", QVariant::fromValue(i));
                connect(temp, SIGNAL(toggled(bool)), this, SLOT(checkBoxToggled(bool)));
            }
        }



    private slots:
        void checkBoxToggled(bool checked) {
            QVariant qv = sender()->property("citystat");
            DPR(tr("Checkbox with stat %1 toggled to %2").arg(CS_NAMES[qv.toInt()]).arg(checked));

            /* if the sender actually has a "citystat" property. Note that this DOESN'T
               check whether the property is an int or not */
            if(qv.isValid()) {
                emit qp->checked((CityStats)qv.toInt(), checked);
            }
        }

        void setChecked(CityStats cs, bool state) {
            DPR(tr("Setting %1 to %2").arg(CS_NAMES[cs]).arg(state));
            m_checkboxes.at(cs)->setChecked(state);
        }


        friend class MdiSettingsController;
    };
}

#endif // MDISETTINGSCONTROLLER_P_H
