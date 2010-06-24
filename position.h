#ifndef POSITION_H
#define POSITION_H

#include <algorithm>
#include <math.h>
#include <QPointF>


namespace QtEpidemy {


    /**
      convenience class for map coordinates, backed by a QPointF. X and Y
      coordinate values are clamped to the range -90..90.

      Longtitude is the Y-axis (with Southern longtitudes represented as
      negative numbers), and latitude is the X-axis (West being negative)

      Can calculate distances between two points.
      */
    class Position
    {
    public:
        // degrees / radian
        const static qreal DEG_TO_RAD = M_PI/180;
        const static qreal EARTH_RADIUS = 6367.5; //in km

        Position(const QPointF &pos);
        Position(qreal x, qreal y);

        /**
          allows casting a Position to a QPointF
          */
        operator QPointF() const {
            return m_position;
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

        /**
          latitude
          */
        inline qreal lat() const {
            return m_position.x();
        }

        /**
          latitude in radians
          */
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
    };

}

#endif // POSITION_H
