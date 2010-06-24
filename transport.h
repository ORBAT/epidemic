#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <QObject>
#include <QTime>
#include <QPointF>
#include "constants.h"

class QPointF;

namespace QtEpidemy {

    class City;


    class Transport : public QObject
    {
        Q_OBJECT
    public:
        // the 747's cruise speed is around 900km/h
        explicit Transport(TransportType, QObject *parent = 0);


    signals:
        void left();
        void arrived();
        void position(const QPointF&);

    public slots:

    protected:
        const City* m_origin;
        City* m_destination;
        QPointF m_position;
        const TransportType m_type;
        const qint16 m_speed; // get this from constants.h

        QTime m_travelTimeLeft;



    };

}

#endif // TRANSPORT_H
