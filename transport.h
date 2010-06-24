#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <QObject>

class QPointF;

namespace QtEpidemy {

    class City;


    class Transport : public QObject
    {
        Q_OBJECT
    public:
        explicit Transport(QObject *parent = 0);


    signals:
        void left();
        void arrived();
        void position(const QPointF&);

    public slots:

    protected:



    };

}

#endif // TRANSPORT_H
