#ifndef CALCULATOR_H
#define CALCULATOR_H
#include <QString>
#include <QMap>
#include <QPair>
#include "calculationStrategy.h"
#include <windows.h>
#include <memory>
using namespace std;


class Calculator //Этот класс - контекст, который принимает указатель на стратегию, делегируя ей выполнение вычислений
{
public:
    Calculator(unique_ptr<CalculationStrategy> st) : strategy(move(st)) {} //В конструктор передается указатель на стратегию
    void setStrategy(unique_ptr<CalculationStrategy> str) { //сеттер для установки стратегии вычислений и возможности ее смены
        this->strategy = move(str); //т.е. мы указателю на стратегию присваиваем выбранную стратегию
        //move() используется для перемещения уникального указателя str в переменную-член this->strategy.
    }
    QMap<QString, int> calculate(QString path) //метод для передачи вычислений объекту-стратегии
    {
        return strategy->calculate(path); //вызов метода для конкретной стратегии, в который передается путь к директории
    }

private:
    unique_ptr<CalculationStrategy> strategy; //умный указатель на объект-стратегию
    //unique_ptr - это умный указатель, предоставляющий единственное владение объектом и подсчитывает кол-во ссылок на него для автоматического удаления
};
#endif // CALCULATOR_H
