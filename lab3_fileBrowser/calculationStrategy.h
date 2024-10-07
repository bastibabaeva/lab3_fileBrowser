#ifndef CALCULATIONSTRATEGY_H
#define CALCULATIONSTRATEGY_H
#include <QString>
#include <QMap>
#include <QList>
#include <QPair>

class CalculationStrategy //Абстрактный класс, который определяет общий интерфейс для всех конкретных стратегий вычислений
{
public:
    virtual QList<QPair<QString, QPair<int, int>>> calculate(QString path, int level = 0) = 0; //общий интерфейс - чисто виртуальный метод,
    // который будет переопределен для всех стратегий и у которого тип возвращаемого значения это класс-словарь QMap,
    // который хранит пару <ключ-значение>, т.е. <Имя папки, размер в КБ>
};

#endif // CALCULATIONSTRATEGY_H
