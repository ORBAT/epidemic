#ifndef QT_NO_DEBUG
#ifndef BADTESTING_H
#define BADTESTING_H

#include <QObject>

namespace QtEpidemy {
    /**
    yeah, yeah. I know. I should use QTestLib, but I just couldn't get it to
    co-operate. */
    class City;
    class BadTesting : public QObject
    {
        Q_OBJECT
    public:
        City *c, *s;
        explicit BadTesting(QObject *parent = 0);
        void doPositionTesting();
        void doTransportTesting();
        void doTests();

    signals:

    public slots:

    };

}

#endif // BADTESTING_H
#endif // QT_NO_DEBUG
