#ifndef METALPRICE_H
#define METALPRICE_H

#include<QDataStream>
#include<QtDebug>

class metalPrice
{
public:
    metalPrice();

    friend QDataStream &operator<<(QDataStream &out, const metalPrice &data);
    friend QDataStream &operator>>(QDataStream &in, metalPrice &data);

public:
    double liPrice;
    double mnPrice;
    double coPrice;
    double niPrice;
    double cuPrice;
    bool isUpdated;
};

#endif // METALPRICE_H
