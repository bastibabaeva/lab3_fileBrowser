#ifndef CALCULATIONSTRATEGY_H
#define CALCULATIONSTRATEGY_H
#include <QString>
#include <QMap>
#include <QPair>

class CalculationStrategy //Абстрактный класс, который определяет общий интерфейс для всех конкретных стратегий вычислений
{
public:
    virtual QMap<QString, int> calculate(QString path, int level = 0) = 0; //чисто виртуальный метод,  который будет переопределен для всех
    //стратегий и у которого тип возвращаемого значения это словарь, который хранит пару <имя файла/папки, размер файла/папки>
};

#endif // CALCULATIONSTRATEGY_H
