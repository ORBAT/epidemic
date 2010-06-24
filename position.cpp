#include "position.h"

namespace QtEpidemy {


    Position::Position(const QPointF &pos) : m_position(pos)
    {
        clampQPointValues(m_position);
    }

}
