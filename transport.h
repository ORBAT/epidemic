#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <QObject>
#include <QTime>
#include "constants.h"
#include "position.h"


namespace QtEpidemy {

    class City;


    class Transport : public QObject
    {
        Q_OBJECT
    public:
        explicit Transport(TransportType, City* origin, City* destination,
                           QObject *parent = 0);


    signals:
        void left();
        void arrived();
        void position(const Position&);

    public slots:

    protected:
        const City* m_origin;
        City* m_destination;
        Position m_position;
        const TransportType m_type;
        const qint16 m_speed; // get this from constants.h

        QTime m_travelTimeLeft;



    };

}

#endif // TRANSPORT_H
