#include "position.h"

namespace QtEpidemy {


    Position::Position(const QPointF &pos) : m_position(pos)
    {
        clampQPointValues(m_position);
    }

    Position::Position(qreal x, qreal y) : m_position(QPointF(x,y)) {
        clampQPointValues(m_position);
    }

    qreal Position::distanceTo(const Position &a,
                            const Position &b) {
        /*

         dlon = lon2 - lon1
         dlat = lat2 - lat1
         a = (sin(dlat/2))^2 + cos(lat1) * cos(lat2) * (sin(dlon/2))^2
         c = 2 * atan2(sqrt(a), sqrt(1-a))
         d = R * c

         lon = y axis
         lat = x axis

         */

        qreal lat1 = a.latRad();
        qreal lat2 = b.latRad();
        qreal dlon = b.lonRad() - a.lonRad();
        qreal dlat = lat2 - lat1;

        qreal ah = pow(sin(dlat / 2.0), 2.0) + cos(lat1) *
                  cos(lat2) * pow(sin(dlon / 2.0), 2.0);
//            qreal ch = 2.0 * atan2(sqrt(ah), sqrt(1.0-ah));
        return EARTH_RADIUS * 2.0 * atan2(sqrt(ah), sqrt(1.0-ah));
    }

}
