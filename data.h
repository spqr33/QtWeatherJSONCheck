#ifndef DATA_H
#define DATA_H
#include <memory>
#include <QString>
#include <QByteArray>
#include <iostream>

class City;
class Coord
{
    friend class City;
public:
    Coord(double lon, double lat) : lon_(lon), lat_(lat){}

    double lon() const;
    double lat() const;
private:
    double lon_;
    double lat_;
    Coord() : lon_(0), lat_(0){}
};

class City {
public:
    static std::shared_ptr<City> makeCity(const QByteArray& JSONRaw);

    City(unsigned int id, const QString& name, const QString& country, const Coord& coord);

    unsigned id() const;
    const QString& name() const;
    const QString& country() const;
    const Coord& coord() const;
private:
    unsigned int    id_;
    QString         name_;
    QString         country_;
    Coord           coord_;

    static City* hardCodeParse(const QByteArray& JSONRaw);
    City();
};

class Data
{
public:
    Data();
};

#endif // DATA_H


std::ostream &operator<<(std::ostream& out, const City& city);


