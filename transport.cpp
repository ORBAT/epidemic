#include "transport.h"
#include "constants.h"
#include "city.h"

namespace QtEpidemy {

    Transport::Transport(TransportType t, City* origin, City* destination,
                         QObject *parent) :
            QObject(parent), m_origin(origin),
            m_destination(destination), m_position(origin->getPosition()),
            m_type(t), m_speed(TRANSPORT_SPEEDS[t]),
            m_travelTimeLeft((m_position.distanceTo(m_destination->getPosition()) /
                              m_speed) * 60 * 60) // travel time in seconds
    {
        DPR(tr("Transport %1 speed %2 from %3 to %4 (%5->%6). ETA %7 (%8s)").
            arg((quintptr)this).
            arg(m_speed).
            arg(m_origin->getName()).
            arg(m_destination->getName()).
            arg(m_position.toString()).
            arg(m_destination->getPosition().toString()).
            arg(m_travelTimeLeft.toString()).
            arg(m_travelTimeLeft.toSeconds()));
    }


    Transport::step() {
        m_travelTimeLeft.addHours();
    }


}
