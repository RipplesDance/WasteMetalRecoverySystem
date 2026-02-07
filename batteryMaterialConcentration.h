#ifndef BATTERYMATERIALCONCENTRATION_H
#define BATTERYMATERIALCONCENTRATION_H

#include <QDebug>

class batteryMaterialConcentration
{

public:
    batteryMaterialConcentration(double li,double co,double mn,double ni,double cu,
                                 double compoundRatio,double positiveMaterialsRatio);
    ~batteryMaterialConcentration();

    void setRecycleRatio(double li_recycleRatio, double co_recycleRatio, double mn_recycleRatio,
                         double ni_recycleRatio, double cu_recycleRatio, double positiveMaterial_recycleRatio);

    void setProperty(double energyDensity, double unitPrice_80, double unitPrice_90);
    public:
    //material raito
    double li;
    double co;
    double mn;
    double ni;
    double cu;
    double compoundRatio;
    double positiveMaterialsRatio;

    //recycle ratio
    double li_recycleRatio;
    double co_recycleRatio;
    double mn_recycleRatio;
    double ni_recycleRatio;
    double cu_recycleRatio;
    double positiveMaterial_recycleRatio;

    //property
    double energyDensity;// Wh/kg
    double unitPrice_80;//CNY/Wh. Depends on market
    double unitPrice_90;//CNY/Wh. Depends on market

    double otherCost = 700.0; //include dismantle fee, transportation fee, and neturalise fee. Count in CNY
};

#endif // BATTERYMATERIALCONCENTRATION_H
