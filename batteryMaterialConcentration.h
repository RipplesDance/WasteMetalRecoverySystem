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

    void setProperty(double unitPrice_80, double unitPrice_90,
                     double price_per_kilo, double profit);
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
    double unitPrice_80;//CNY/Wh. Depends on market
    double unitPrice_90;//CNY/Wh. Depends on market

    double price_per_kilo; //include electricity fee， labor fee, and chemicals fee. Count in CNY.
    double profit;

};

#endif // BATTERYMATERIALCONCENTRATION_H
