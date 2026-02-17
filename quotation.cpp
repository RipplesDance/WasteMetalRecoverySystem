#include "quotation.h"

quotation::quotation()
{
    init();


    readAllBatteryFromLocal();
    batteryMaterialConcentration* LCO = new batteryMaterialConcentration(0.071,0.602,0,0,0.1, 0.93, 0.35);
    LCO->setRecycleRatio(0.85, 0.95,0,0,0.98,0.95,0.95);
    batteryMap.insert("钴酸锂电池", LCO);

    setProperty(0.8, 0.3, 0.35,45,0.2);

}

quotation:: ~quotation()
{
}

void quotation::init()
{

    Li_to_LCE = 73.89/ (6.94*2); // Li₂CO₃ = 73.89, Li = 6.94
    Ni_to_NiSo4 = 262.87/ 58.69; // 6H₂O  NiSO₄·6H₂O = 262.87, Ni = 58.69
    Co_to_CoSo4 = 281.10/ 58.93; // 7H₂O  CoSO₄·7H₂O = 281.10, Co = 58.93
    Mn_to_MnSo4 = 169.02/ 54.94;// 1H₂O MnSO₄·H₂O = 169.02, Mn = 54.94
}

void quotation::batteryChangedHandler(QString key, batteryMaterialConcentration* value)
{
    if (value == nullptr) return;

    if (batteryMap.contains(key)) {
        // exist
        batteryMaterialConcentration *oldData = batteryMap.take(key);
        if(oldData != value)
            delete oldData;
    }
    batteryMap.insert(key,value);
}

bool quotation::batteryNameChangedHandler(QString newKey, QString oldKey)
{
    if(!batteryMap.contains(oldKey)) return false;
    if(batteryMap.contains(newKey)) return false;

    batteryMaterialConcentration* data = batteryMap.take(oldKey);
    batteryMap.insert(newKey, data);
    return  true;
}

bool quotation::removeBatteryByName(QString key)
{
    if (!batteryMap.contains(key)) {
        return false;
    }
    delete batteryMap.take(key); //delete original value
    batteryMap.remove(key);
    return true;
}

bool quotation::renameLocalBattery(QString origin, QString name)
{
    QString oldPath = "bin/quotation_model/" + origin.toUtf8().toBase64() + ".dat";
    QString newPath = "bin/quotation_model/" + name.toUtf8().toBase64() + ".dat";

    qDebug()<<origin << oldPath;
    qDebug() << name << newPath;

    if (!QFile::exists(oldPath))
        return false;

    QFile::rename(oldPath, newPath);
    return true;
}

QList<QString> quotation::readAllBatteryType()
{
    return batteryMap.keys();
}

QList<batteryMaterialConcentration*> quotation::readAllBatteryMaterialConcentration()
{
    return batteryMap.values();
}

bool quotation::saveBatteryToLocal(QString key, batteryMaterialConcentration* data)
{
    if (data == nullptr) return false;
    QString fileName =key.toUtf8().toBase64();

    QFile file("bin/quotation_model/" + fileName + ".dat");
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug()<<"无法打开文件"+fileName;
        return false;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_14);
    out<<*(data);
    return true;
}

bool quotation::removeBatteryFromLocal(QString key)
{
    QString fileName =key.toUtf8().toBase64();

    QString filePath = "bin/quotation_model/" + fileName + ".dat";
    QFile file(filePath);
    if(!QFile::exists(filePath))
    {
        qDebug()<<"文件不存在";
        return false;
    }
    if (file.remove())
        return true;
    else
        return false;
}

bool quotation::addBatteryType(QString key, batteryMaterialConcentration* data)
{
    if(batteryMap.contains(key)) return false;
    batteryMap.insert(key, data);
    return true;
}

void quotation::saveAllBatteryToLocal()
{
    QDir dir;
    if (!dir.exists("bin/quotation_model")) dir.mkdir("bin/quotation_model");

    QMap<QString, batteryMaterialConcentration*>::iterator it = batteryMap.begin();
    while(it != batteryMap.end())
    {
        QString fileName =it.key().toUtf8().toBase64();

        QFile file("bin/quotation_model/" + fileName + ".dat");
        if(!file.open(QIODevice::WriteOnly))
        {
            qDebug()<<"无法打开文件保存batteryMaterialConcentration";
            return;
        }

        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_5_14);
        out<<*(it.value());
        it++;
    }
}

void quotation::readAllBatteryFromLocal()
{
    qDeleteAll(batteryMap);
    batteryMap.clear();

    QDir dir("bin/quotation_model");
    QStringList filters;
    filters << "*.dat";
    dir.setNameFilters(filters);

    QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::NoDot | QDir::NoDotDot);

    qDebug()<<"list.size()"<<list.size();

    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        QFile file(fileInfo.absoluteFilePath());

        if (file.open(QIODevice::ReadOnly)) {
            QDataStream in(&file);
            in.setVersion(QDataStream::Qt_5_14);

            QString fileName = fileInfo.baseName();
            QByteArray base64Data = QByteArray::fromBase64(fileName.toUtf8());
            QString key = QString::fromUtf8(base64Data);

            batteryMaterialConcentration *obj = new batteryMaterialConcentration();
            in >> *obj;

            batteryMap.insert(key, obj);

            file.close();
        }
    }
}

void quotation::setProperty(double transitionRatio,double unitPrice_80, double unitPrice_90,double price_per_kilo, double profit)
{
    this->unitPrice_80 = unitPrice_80;
    this->unitPrice_90 = unitPrice_90;

    this->price_per_kilo = price_per_kilo;
    this->profit = profit;
    this->transitionRatio = transitionRatio;
}

double quotation::quotationCaculator(QString type, double energyDensity, double weight, double SOH,
                                     QMap<QString, double> metalPriceMap)
{
    if(!batteryMap.contains(type))
    {
        QMessageBox::information(nullptr,"提示", "当前暂不支持该电池的报价");
        return 0.0;
    }

    batteryMaterialConcentration* battery = batteryMap.value(type);

    if(SOH >= 0.8 && energyDensity > 0)
    {
        //reusable
        double finalPrice = weight * energyDensity * SOH;
        if(SOH>=0.9)
            return finalPrice * unitPrice_90;
        return finalPrice * unitPrice_80;
    }

    double positiveMaterial = weight * battery->positiveMaterialsRatio * battery->positiveMaterial_recycleRatio;
    double positiveMaterialCompound = positiveMaterial * battery->compoundRatio * battery->compound_recycleRatio;

    //metal amount
    double li_amount =  positiveMaterialCompound * battery->li * battery->li_recycleRatio;
    double co_amount = positiveMaterialCompound * battery->co * battery->co_recycleRatio;
    double mn_amount = positiveMaterialCompound * battery->mn * battery->mn_recycleRatio;
    double ni_amount = positiveMaterialCompound * battery->ni * battery->ni_recycleRatio;

    //metal price
    double li_quotation = li_amount * Li_to_LCE * metalPriceMap.value("Li") * transitionRatio;
    double ni_quotation = ni_amount * Ni_to_NiSo4 * metalPriceMap.value("Ni") * transitionRatio;
    double co_quotation = co_amount * Co_to_CoSo4 * metalPriceMap.value("Co") * transitionRatio;
    double mn_quotation = mn_amount * Mn_to_MnSo4 * metalPriceMap.value("Mn") * transitionRatio;

    double cu_quotation = weight * battery->cu * battery->cu_recycleRatio * metalPriceMap.value("Cu");

    //final price
    double finalPrice = (li_quotation+co_quotation+mn_quotation+ni_quotation+cu_quotation)
            - price_per_kilo * weight;

    return finalPrice > 0 ? finalPrice* (1 - profit) : 0;

}

QDataStream &operator<<(QDataStream &out, const quotation &data)
{
    out<<data.transitionRatio << data.unitPrice_80 << data.unitPrice_90 << data.price_per_kilo << data.profit;
    return out;
}

QDataStream &operator>>(QDataStream &in, quotation &data)
{
    in>>data.transitionRatio >> data.unitPrice_80 >> data.unitPrice_90 >> data.price_per_kilo >> data.profit;
    return in;
}
