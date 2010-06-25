#include "transport.h"
#include "constants.h"
#include "city.h"

namespace QtEpidemy {

    Transport::Transport(TransportType t, City* origin, City* destination,
                         QObject *parent) :
            QObject(parent), m_origin(origin),
            m_destination(destination), m_position(origin->getPosition()),
            m_type(t), m_speed(TRANSPORT_SPEEDS[t])
    {
        DPR(tr("Transport %1 speed %2").arg((quintptr)this).arg(m_speed));
    }


}
