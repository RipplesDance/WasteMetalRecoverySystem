#ifndef BATTERYMATERIALCONCENTRATION_H
#define BATTERYMATERIALCONCENTRATION_H

#include <QDebug>

class batteryMaterialConcentration
{

public:
    batteryMaterialConcentration(double li,double co,double mn,double ni,double cu,
                                 double compoundRatio,double positiveMaterialsRatio);
    ~batteryMaterialConcentration();

    public:
    double li;
    double co;
    double mn;
    double ni;
    double cu;
    double compoundRatio;
    double positiveMaterialsRatio;

    //static
    double li_recycleRatio = 0.85;
    double co_recycleRatio = 0.95;
    double mn_recycleRatio = 0.95;
    double ni_recycleRatio = 0.95;
    double cu_recycleRatio = 0.98;

    double otherCost = 700.0; //include dismantle fee, recycling fee, neturalise fee, and labor fee. Count in CNY.
};

#endif // BATTERYMATERIALCONCENTRATION_H
