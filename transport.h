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
        explicit Transport(TransportType, City* origin, City* destination,
                           const QList<quint16> passengerList,
                           QObject *parent = 0);

        /**
        Convenience class for keeping track of time. Unlike QTime, Time can
        go over 24 hours.
        */
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

            /**
            Returns the day part of the stored time (so for a Time that contains
            16d 19h 15min 4s Time::day() will return 16
            */
            int day() const {
                return m_seconds / SECS_PER_DAY;
            }

            /**
            Returns the hour part of the stored time. For a Time of 16d 19h
            15min 4s this would return 19
            */
            int hour() const {
                return (m_seconds % SECS_PER_DAY) / SECS_PER_HOUR;
            }

            /**
            Returns the minute part of the time. A Time of 16d 19h 15min 4s
            would return 15
            */
            int min() const {
                return (m_seconds % SECS_PER_HOUR) / SECS_PER_MIN;
            }

            /**
            Returns the seconds part of the time. A Time of 16d 19h 15min 4s
            would return 4
            */
            int sec() const {
                return m_seconds % SECS_PER_MIN;
            }

            /**
            Converts a Time to seconds. A Time of 16d 19h 15min 4s would return
            1 451 704.*/
            int toSeconds() const {
                return m_seconds;
            }

            /**
            Adds s seconds to a Time and returns a reference to this. The point
            of returning a reference is to enable something like
            int hours = time.addSecs(10).hour();
            */
            Time& addSecs(int s) {
                m_seconds += s;
                clampToZero(m_seconds);
                return *this;
            }

            /**
            Adds m minutes to a Time and returns a reference to this. The point
            of returning a reference is to enable something like
            int hours = time.addSecs(10).hour();
            */
            Time& addMinutes(int m) {
                m_seconds += m * SECS_PER_MIN;
                clampToZero(m_seconds);
                return *this;
            }

            /**
            Adds h hours to a Time and returns a reference to this. The point
            of returning a reference is to enable something like
            int hours = time.addSecs(10).hour();
            */
            Time& addHours(int h) {
                m_seconds += h * SECS_PER_HOUR;
                clampToZero(m_seconds);
                return *this;
            }

            /**
            Adds d days to a Time and returns a reference to this. The point
            of returning a reference is to enable something like
            int hours = time.addSecs(10).hour();
            */
            Time& addDays(int d) {
                m_seconds += d * SECS_PER_DAY;
                clampToZero(m_seconds);
                return *this;
            }

            /**
            Converts a Time into a QString with the format 00d 00h 00min
            */
            QString toString() const {
                return QString("%1d %2h %3min").
                        arg(day(), 2, 10, QLatin1Char('0')).
                        arg(hour(), 2, 10, QLatin1Char('0')).
                        arg(min(), 2, 10, QLatin1Char('0'));
            }



        private:
            int m_seconds;
        };


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
        // Where the Transport is coming from. Should never change.
        const City* const m_origin;

        /* Where the Transport is headed. This might change, depending on the
           situation (say some City stops receiving passengers etc.) */
        City* m_destination;

        // Where am I? DEAR GOD WHERE AM I? I'M LOST! HALP!
        Position m_position;
        const TransportType m_type;
        const qint16 m_speed; // get this from constants.h

        /* all the different types of passengers (infected etc). Currently
           passengers can't for example get infected or die while traveling.
         */
        quint16 m_passengers[PT_MAX_TYPES];

        Transport::Time m_travelTimeLeft;


        static quint32 s_transportId;




    };

}

#endif // TRANSPORT_H
