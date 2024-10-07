#ifndef CALCULATIONSTRATEGY_H
#define CALCULATIONSTRATEGY_H
#include <QString>

class CalculationStrategy: //Абстрактный класс, который определяет общий интерфейс для всех конкретных стратегий вычислений
{
public:
    virtual ~CalculationStrategy(){}  //деструктор
    virtual void calculate(const QString& path, int level=0)=0; //общий интерфейс - чисто виртуальный метод, который будет переопределен для всех стратегий
};

#endif // CALCULATIONSTRATEGY_H
