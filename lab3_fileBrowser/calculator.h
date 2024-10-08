#ifndef CALCULATOR_H
#define CALCULATOR_H
#include <QString>
#include <QList>
#include <QPair>
#include "calculationStrategy.h"
#include <windows.h>
#include <memory>

using namespace std;

class Calculator //Этот класс - контекст, который ссылается на стратегию, делегируя ей выполнение вычислений
{
public:
    Calculator(unique_ptr<CalculationStrategy> st) : strategy(move(st)) {} //В конструктор передается указатель на стратегию
    void setStrategy(unique_ptr<CalculationStrategy> str) { //сеттер для установки стратегии вычислений и возможности ее смены
        this->strategy = move(str); //т.е. мы указателю на стратегию присваиваем выбранную стратегию
        //move() используется для перемещения уникального указателя str в переменную-член this->strategy.
    }
    QList<QPair<QString, QPair<int, int>>> calculate(QString path) //метод для передачи вычислений объекту-стратегии
    {
        return strategy->calculate(path); //вызов метода для конкретной стратегии, в который передается путь к директории
    }

private:
    unique_ptr<CalculationStrategy> strategy; //умный указатель на объект-стратегию
};
#endif // CALCULATOR_H
