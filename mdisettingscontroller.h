#ifndef MDISETTINGSCONTROLLER_H
#define MDISETTINGSCONTROLLER_H

#include <QObject>
#include <QCheckBox>
#include <QList>
#include "constants.h"


class QGridLayout;
class QSignalMapper;

namespace QtEpidemy {

    class MdiSettingsControllerPrivate;

    class MdiSettingsController : public QObject
    {
        Q_OBJECT
    public:
        explicit MdiSettingsController(QGridLayout *&settingsGrid, QWidget *&parentWidget,
                                       QObject *parent = 0);

    signals:
        // emitted when a checkbox is toggled
        void checked(CityStat, bool);

    public slots:
        // set the checked state of a specific stat's checkbox
        void setChecked(CityStat, bool state = true);


    private:
        /* This little feller is used to hide implementation details. I couldn't figure out
           a sensible way to make slots private (ie. so that no outside class can connect to
           them, so I put all the internal logic into a separate class that can only be
           instantiated by MdiSettingsController.

           This "solution" is not exactly optimal.
        */
        MdiSettingsControllerPrivate * const pd;

        friend class MdiSettingsControllerPrivate;


    };

}

#endif // MDISETTINGSCONTROLLER_H
