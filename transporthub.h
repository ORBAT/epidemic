#ifndef TRANSPORTHUB_H
#define TRANSPORTHUB_H

#include <QObject>

namespace QtEpidemy {



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
