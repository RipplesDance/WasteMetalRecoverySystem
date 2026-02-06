#ifndef QUOTATION_H
#define QUOTATION_H

#include<QDebug>
#include<QMap>
#include"batteryMaterialConcentration.h"

class quotation
{
    Q_OBJECT
public:
    quotation();
    ~quotation();
    double quotationCaculator(QString type, int weight, double percentage);

private:
    QMap<QString, batteryMaterialConcentration*> batteryMap;
};

#endif // QUOTATION_H
