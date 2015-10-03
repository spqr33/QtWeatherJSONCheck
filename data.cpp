#include "data.h"
#include <iomanip>
#include <QDebug>
#include <QByteArray>
#include <cassert>

Data::Data()
{
}

double Coord::lon() const
{
    return lon_;
}

double Coord::lat() const
{
    return lat_;
}


std::shared_ptr<City> City::makeCity(const QByteArray& JSONRaw)
{
    City* p = hardCodeParse(JSONRaw);
    if (!p) {
        return std::shared_ptr<City>();
    }

    return std::shared_ptr<City>(p);
}

City::City(unsigned int id, const QString& name, const QString& country, const Coord& coord) :
    id_(id),
    name_(name),
    country_(country),
    coord_(coord)
{
}

unsigned City::id() const
{
    return id_;
}

const QString &City::name() const
{
    return  name_;
}

const QString &City::country() const
{
    return country_;
}

const Coord &City::coord() const
{
    return coord_;
}

City *City::hardCodeParse(const QByteArray& JSONRaw)
{ //TODO    negative lon lat to implement
    City* newCity = new City;
    newCity->id_= 0;
    const char* curr = JSONRaw.data();
    unsigned char divide_counter = 0;
    static double pow_10[] = { 10.0, 100.0, 1000.0, 10000.0, 100000.0, 1000000.0, 10000000.0, 100000000.0, 1000000000.0 };
    QByteArray cityName;
    QByteArray countryName;


    enum STATES {
        start,
        first_colon,
        first_colon_comma,
        second_colon,
        second_colon_open_quote,
        second_colon_close_quote,
        third_colon,
        third_colon_open_quote,
        third_colon_close_quote,
        forth_colon,
        fifth_colon,
        fifth_colon_dot,
        fifth_colon_comma,
        sixth_colon,
        sixth_colon_dot,
        sixth_colon_right_bracket,
        enough
    };

    STATES state = start;
    while (*curr) {
        char ch = *curr;

        switch (state) {
        case start:
            if ( ch == ':' ) {
                state = first_colon;
            }
            break;
        case first_colon:
            if ( ch == ',' ) {
                state = first_colon_comma;
            } else {
                if ( ch >= '0' && ch <= '9' ){
                    newCity->id_ =  newCity->id_ *10 +(ch - '0');
                } else {
                    return nullptr;
                }
            }
            break;
        case first_colon_comma:
            if ( ch == ':' ) {
                state = second_colon;
            }
            break;
        case second_colon:
            if ( ch == '\"' ) {
               state = second_colon_open_quote;
            }
            break;
        case second_colon_open_quote:
            if ( ch == '\"' ){
                state = second_colon_close_quote;
            } else {
                cityName.append(ch);
            }
            break;
        case second_colon_close_quote:
            if ( ch == ':' ) {
                state = third_colon;
            }
            break;
        case third_colon:
            if ( ch == '\"' ) {
               state = third_colon_open_quote;
            }
            break;
        case third_colon_open_quote:
            if ( ch == '\"' ){
                state = third_colon_close_quote;
            } else {
                countryName.append(ch);
            }
            break;
        case third_colon_close_quote:
            if ( ch == ':' ) {
                state = forth_colon;
            }
            break;
        case forth_colon:
            if ( ch == ':' ) {
                state = fifth_colon;
            }
            break;
        case fifth_colon:
            if ( ch == '.' ) {
                state = fifth_colon_dot;
            } else if ( ch >= '0' && ch <= '9' ) {
                newCity->coord_.lon_ = newCity->coord_.lon_*10 +(ch - '0');
            } else {
                return nullptr;
            }
            break;
        case fifth_colon_dot:
            if ( ch == ',' ) {
                state = fifth_colon_comma;
                divide_counter = 0;
            } else if ( ch >= '0' && ch <= '9' ) {
                newCity->coord_.lon_ += (ch - '0') / pow_10[divide_counter++];
            } else {
                return nullptr;
            }
            break;
        case fifth_colon_comma:
            if ( ch == ':' ) {
                state = sixth_colon;
            }
            break;
        case sixth_colon:
            if ( ch == '.' ) {
                state = sixth_colon_dot;
            } else if ( ch >= '0' && ch <= '9' ) {
                newCity->coord_.lat_ = newCity->coord_.lat_*10 +(ch - '0');
            } else {
                return nullptr;
            }
            break;
        case sixth_colon_dot:
            if ( ch == '}' ) {
                state = sixth_colon_right_bracket;
            } else if ( ch >= '0' && ch <= '9' ) {
                newCity->coord_.lat_ += (ch - '0') / pow_10[divide_counter++];
            } else {
                return nullptr;
            }
            break;
        case sixth_colon_right_bracket:
            state = enough;
            break;
        case enough:
            assert("We can't be here");
            break;
        };
        if ( state == enough ) {
            break; // break while
        }

        ++curr;
    }
    newCity->name_.append(cityName);
    newCity->country_.append(countryName);

    return newCity;
}

City::City()
{

}

std::ostream& operator<<(std::ostream &out, const City &city)
{
    qDebug() << "_________Start__________\n";
    qDebug() << "Name: " << city.name();// << std::endl;
    qDebug() << "id: " << city.id();
    qDebug() << "country: " << city.country();
    qDebug() << "coord.lon: " <<  city.coord().lon();
    qDebug() << "coord.lat: " << city.coord().lat();
    qDebug() << "_________Finish__________\n";

    return out;
}
