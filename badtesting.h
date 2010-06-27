#ifndef QT_NO_DEBUG
#ifndef BADTESTING_H
#define BADTESTING_H

#include <QObject>

namespace QtEpidemy {
    class BadTesting : public QObject
    {
        Q_OBJECT
    public:
        explicit BadTesting(QObject *parent = 0);
        void doPositionTesting();
        void doTests();

    signals:

    public slots:

    };

}

#endif // BADTESTING_H
#endif // QT_NO_DEBUG
