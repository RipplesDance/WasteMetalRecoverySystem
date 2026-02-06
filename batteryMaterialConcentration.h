#ifndef BATTERYMATERIALCONCENTRATION_H
#define BATTERYMATERIALCONCENTRATION_H

#include <QObject>

class batteryMaterialConcentration
{
    Q_OBJECT
public:
    batteryMaterialConcentration(double li,double co,double mn,double ni,double fe);
    ~batteryMaterialConcentration();

private:
    double li;
    double co;
    double mn;
    double ni;
    double fe;
};

#endif // BATTERYMATERIALCONCENTRATION_H
