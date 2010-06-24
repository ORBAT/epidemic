#ifndef POSITION_H
#define POSITION_H

#include <algorithm>
#include <math.h>
#include <QPointF>


namespace QtEpidemy {

    class Position
    {
    public:
        // degrees / radian
        const static qreal DEG_TO_RAD = M_PI/180;
        const static qreal EARTH_RADIUS = 6367.5; //in km

        Position(const QPointF &pos);
        Position(qreal x, qreal y);



    protected:
        QPointF m_position;

        /**
          Used to clamp x and y coordinates to the range -90 .. 90
          */
        inline void clampVal(qreal& val) {
            val = std::min(90.0, std::max(-90.0, val));
        }

        /**
          clamps a QPointF's X and Y coordinates to the range -90..90
          */
        inline void clampQPointValues(QPointF &point) {
            clampVal(point.rx());
            clampVal(point.ry());
        }

        /**
          returns the longtitude in degrees
          */
        inline qreal lon() const {
            return m_position.y();
        }

        /**
          longtitude but in radians
          */
        inline qreal lonRad() const {
            return m_position.y() * DEG_TO_RAD;
        }


        inline qreal lat() const {
            return m_position.x();
        }

        inline qreal latRad() const {
            return m_position.x() * DEG_TO_RAD;
        }

        /**
          converts degrees to radians
          */
        inline qreal degToRad(qreal deg) {
            return deg * DEG_TO_RAD;
        }

        /**
          calculates the distance between Position a and Position b using
          Haversine formula
        */
        static qreal distanceTo(const Position &a, const Position &b);

    };

}

#endif // POSITION_H
