#include "transport.h"
#include "constants.h"
#include "city.h"

namespace QtEpidemy {

    Transport::Transport(TransportType t, City* origin, City* destination,
                         const QList<quint16> passengers,
                         QObject *parent) :
            QObject(parent), m_origin(origin),
            m_destination(destination), m_position(origin->getPosition()),
            m_type(t), m_speed(TRANSPORT_SPEEDS[t]),
            m_travelTimeLeft((m_position.distanceTo(m_destination->getPosition()) /
                              m_speed) * 60 * 60) // travel time in seconds
    {
        Q_ASSERT_X(passengers.size() == PT_MAX_TYPES, Q_FUNC_INFO,
                   "Amount of passenger types must be exactly PT_MAX_TYPES");
        this->setObjectName(TRANSPORT_NAMES[m_type] %
                            "-" % QString::number(s_transportId++));

        for(int i = 0; i < PT_MAX_TYPES; ++i) {
            m_passengers[i] = passengers.at(i);
            CDPR(tr("Passenger type %1, amount %2").arg(PASSENGER_TYPE_NAMES[i]).
                                                    arg(m_passengers[i]));
        }

        DPR(tr("Transport %1 speed %2 from %3 to %4 (%5->%6). ETA %7 (%8s)").
            arg(objectName()).
            arg(m_speed).
            arg(m_origin->getName()).
            arg(m_destination->getName()).
            arg(m_position.toString()).
            arg(m_destination->getPosition().toString()).
            arg(m_travelTimeLeft.toString()).
            arg(m_travelTimeLeft.toSeconds()));

    }


    void Transport::step() {
        /* DT days elapse each step which makes DT*24 hours.
           m_speed is speed in km/h, so time traveled during each step is
           m_speed * DT*24
        */

        m_travelTimeLeft.addHours(-DT*24);
        if(!m_travelTimeLeft) { // zero travel time left
            CDPR(tr("Arrived at %1").arg(m_destination->getName()));
            emit arrived(m_destination);
        } else {
            qreal travelDist = m_speed*DT*24;
            // travel travelDist km towards destination City's Position
            m_position = m_position.moveTowards(m_destination->getPosition(),
                                                travelDist);
        }
    }

    void Transport::changeDestination(City *c) {
        DPR(tr("Destination changed to %1").arg(c->getName()));
    }


    quint32 Transport::s_transportId = 0;

}
