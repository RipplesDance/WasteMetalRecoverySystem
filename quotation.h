#ifndef QUOTATION_H
#define QUOTATION_H

#include<QDebug>
#include<QMap>
#include<QMessageBox>
#include<QDataStream>
#include<QDir>
#include<QFile>
#include<QList>
#include"batteryMaterialConcentration.h"

class quotation
{

public:
    quotation();
    ~quotation();
    double quotationCaculator(QString type, double energyDensity, double weight, double SOH, QMap<QString, double> metalPriceMap);
    void init();
    void setProperty(double transitionRatio, double unitPrice_80, double unitPrice_90,
                     double price_per_kilo, double profit);

    void saveAllBatteryToLocal();
    void readAllBatteryFromLocal();
    bool addBatteryType(QString key, batteryMaterialConcentration* data);
    bool saveBatteryToLocal(QString key, batteryMaterialConcentration* data);
    bool removeBatteryFromLocal(QString key);
    QList<QString> readAllBatteryType();
    QList<batteryMaterialConcentration*> readAllBatteryMaterialConcentration();
    void batteryChangedHandler(QString key, batteryMaterialConcentration* value);
    bool batteryNameChangedHandler(QString newKey, QString oldKey);
    bool removeBatteryByName(QString key);
    bool renameLocalBattery(QString origin, QString name);

    friend QDataStream &operator<<(QDataStream &out, const quotation &data);
    friend QDataStream &operator>>(QDataStream &in, quotation &data);

private:
    QMap<QString, batteryMaterialConcentration*> batteryMap;

public:

    //coefficient
     double Li_to_LCE;
   double Ni_to_NiSo4; // 6H₂O  NiSO₄·6H₂O
   double Co_to_CoSo4; // 7H₂O  CoSO₄·7H₂O
    double Mn_to_MnSo4;// 1H₂O MnSO₄·H₂O


    //property
    double transitionRatio; //cost, efficiency, etc..
    double unitPrice_80;//CNY/Wh. Depends on market
    double unitPrice_90;//CNY/Wh. Depends on market

    double price_per_kilo; //include electricity fee， labor fee, and chemicals fee. Count in CNY.
    double profit;
};

#endif // QUOTATION_H
