#include "transport.h"
#include "constants.h"

namespace QtEpidemy {

    Transport::Transport(TransportType t, QObject *parent) :
            QObject(parent), m_origin(NULL),
            m_destination(NULL), m_type(t)
    {
        DPR(tr("Transport %1 speed %2").arg((quintptr)this).arg(speed));
    }


}
