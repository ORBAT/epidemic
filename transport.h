#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <QObject>
#include <QDateTime>
#include "constants.h"
#include "position.h"


namespace QtEpidemy {

    class City;



    class Transport : public QObject
    {
        Q_OBJECT

    public:
        class Time {
            static const int SECS_PER_MIN = 60;
            static const int SECS_PER_HOUR = 3600;
            static const int SECS_PER_DAY = 86400;

        public:
            Time(qint32 seconds) : m_seconds(seconds) {}

            //allows using Time in a bool context like, say, an if()
            operator bool() const {
                return m_seconds; //ie. true if > 0 and false if 0
            }

            int day() const {
                return m_seconds / SECS_PER_DAY;
            }

            int hour() const {
                return (m_seconds % SECS_PER_DAY) / SECS_PER_HOUR;
            }

            int min() const {
                return (m_seconds % SECS_PER_HOUR) / SECS_PER_MIN;
            }

            int sec() const {
                return m_seconds % SECS_PER_MIN;
            }

            int toSeconds() const {
                return m_seconds;
            }

            Time& addSecs(int s) {
                m_seconds += s;
                clampToZero(m_seconds);
                return *this;
            }

            Time& addMinutes(int m) {
                m_seconds += m * SECS_PER_MIN;
                clampToZero(m_seconds);
                return *this;
            }

            Time& addHours(int h) {
                m_seconds += h * SECS_PER_HOUR;
                clampToZero(m_seconds);
                return *this;
            }

            Time& addDays(int d) {
                m_seconds += d * SECS_PER_DAY;
                clampToZero(m_seconds);
                return *this;
            }

            QString toString() const {
                return QString("%1d %2h %3min").
                        arg(day(), 2, 10, QLatin1Char('0')).
                        arg(hour(), 2, 10, QLatin1Char('0')).
                        arg(min(), 2, 10, QLatin1Char('0'));
            }



        private:
            int m_seconds;
        };

        explicit Transport(TransportType, City* origin, City* destination,
                           QObject *parent = 0);


    signals:
        void left(City*);
        void arrived(City*);
        void position(const Position&, const qreal bearing);
        void timeLeft(const Transport::Time&);
        void destinationChanged(City*);

    public slots:
        void step();

    protected slots:
        void changeDestination(City*);

    protected:
        const City* m_origin;
        City* m_destination;
        Position m_position;
        const TransportType m_type;
        const qint16 m_speed; // get this from constants.h

        static quint32 s_transportId;

        Transport::Time m_travelTimeLeft;



    };

}

#endif // TRANSPORT_H
