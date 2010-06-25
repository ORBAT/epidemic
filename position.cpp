#include "position.h"

namespace QtEpidemy {


    Position::Position(const QPointF &pos) : m_position(pos)
    {
        clampQPointValues(m_position);
    }

    Position::Position(qreal latitude, qreal longitude) :
            m_position(QPointF(longitude,latitude)) {
        clampQPointValues(m_position);
    }

    qreal Position::distanceBetween(const Position &a,
                            const Position &b) {
        /*

         dlon = lon2 - lon1
         dlat = lat2 - lat1
         a = (sin(dlat/2))^2 + cos(lat1) * cos(lat2) * (sin(dlon/2))^2
         c = 2 * atan2(sqrt(a), sqrt(1-a))
         d = R * c
         */

        qreal lat1 = a.latRad();
        qreal lat2 = b.latRad();
        qreal lon1 = a.lonRad();
        qreal lon2 = b.lonRad();
        qreal dLat = lat2 - lat1;
        qreal dLon = lon2 - lon1;

        qreal ah = pow(sin(dLat / 2.0), 2.0) + cos(lat1) *
                  cos(lat2) * pow(sin(dLon / 2.0), 2.0);
        qreal ch = 2.0 * atan2(sqrt(ah), sqrt(1.0-ah));
        return EARTH_RADIUS * ch;
    }

    qreal Position::distanceTo(const Position &b) const {
        return Position::distanceBetween(*this, b);
    }

}
