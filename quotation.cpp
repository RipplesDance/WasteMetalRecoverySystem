#include "quotation.h"

quotation::quotation()
{
    batteryMap.insert("Lico2", new batteryMaterialConcentration(0,0,0,0,0));
}

quotation:: ~quotation()
{
}

double quotation::quotationCaculator(QString type, int weight, double percentage)
{
    if(!batteryMap.contains(type))
        return 0,0;
}
