#include "batteryMaterialConcentration.h"

batteryMaterialConcentration::batteryMaterialConcentration(double li,double co,double mn,double ni,double cu,
                                                           double compoundRatio,double positiveMaterialsRatio)
    :li(li),co(co),mn(mn),ni(ni),cu(cu), compoundRatio(compoundRatio), positiveMaterialsRatio(positiveMaterialsRatio)
{

}
batteryMaterialConcentration::~batteryMaterialConcentration()
{}

void batteryMaterialConcentration::setRecycleRatio(double li_recycleRatio, double co_recycleRatio,
                                                   double mn_recycleRatio, double ni_recycleRatio,
                                                   double cu_recycleRatio, double positiveMaterial_recycleRatio)
{
    this->li_recycleRatio = li_recycleRatio;
    this->co_recycleRatio = co_recycleRatio;
    this->mn_recycleRatio = mn_recycleRatio;
    this->ni_recycleRatio = ni_recycleRatio;
    this->cu_recycleRatio = cu_recycleRatio;
    this->positiveMaterial_recycleRatio = positiveMaterial_recycleRatio;
}


