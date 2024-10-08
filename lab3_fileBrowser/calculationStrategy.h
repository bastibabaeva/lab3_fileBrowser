#ifndef CALCULATIONSTRATEGY_H
#define CALCULATIONSTRATEGY_H
#include <QString>
#include <QMap>
#include <QPair>

class CalculationStrategy //Абстрактный класс, который определяет общий интерфейс для всех конкретных стратегий вычислений
{
public:
    virtual QMap<QString, QPair<int, int>> calculate(QString path, int level = 0) = 0; //общий интерфейс - чисто виртуальный метод,
    // который будет переопределен для всех стратегий и у которого тип возвращаемого значения это список, который хранит пару <имя файла/папки,
    // размер файла/папки, общий размер директории>
};

#endif // CALCULATIONSTRATEGY_H
