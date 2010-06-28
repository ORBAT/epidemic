#ifndef TRANSPORTHUB_H
#define TRANSPORTHUB_H

#include <QObject>

namespace QtEpidemy {

    /**
    Meant to be used in Cities. TransportHubs handle all traffic to and from
    a City.

    Think of them as airports, bus stations etc.
    */


    class TransportHub : public QObject
    {
        Q_OBJECT
    public:
        explicit TransportHub(QObject *parent = 0);

    signals:

    public slots:

    };

}

#endif // TRANSPORTHUB_H
