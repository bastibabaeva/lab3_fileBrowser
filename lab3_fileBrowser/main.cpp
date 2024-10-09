#include <iostream>
#include "calculator.h"
#include "byFolderCalcStrategy.h"
#include "byTypeCalcStrategy.h"
#include <memory>
#include <QString>
#include <QDir>
using namespace std;

QString percentageCalc(int size, int total_size) { //Функция для вычисления процентного соотношения размера текущего файла/папки size от
    // общего размера директории total_size
    if (total_size != 0 && size != 0) { //Если общий размер директории и размер текущей папки/файла не равны 0, то
        double percent = 100.0 * size / total_size;
        if (percent < 0.01) return "< 0.01 %"; //для обработки очень маленьких файлов
        else return QString::number(percent, 'f', 2) + " %"; //возвращаем процент в фиксированном формате(f) с 2 знаками после запятой в виде строки
    }
    else { return "0.00 %"; }
}

int main() {
    SetConsoleOutputCP(CP_UTF8); // Установка кодировки консоли для корректного отображения русских символов
    cout << "Enter the path to the directory: ";
    string path;
    getline(cin, path); // Вводим путь к директории

    cout << "Select a strategy for calculating file sizes: 1 - grouping by folders, 2 - grouping by types. ";
    int strategy;
    cin >> strategy; // Вводим стратегию

    unique_ptr<Calculator> calculator; // Создаем умный указатель на объект Calculator
    switch (strategy) {
    case 1:
        calculator = make_unique<Calculator>(make_unique<ByFolderCalculationStrategy>()); //создаем умный указатель на объект Calculator,
        //в конструктор Calculator передаем в качестве параметра умный указатель на объект конкретной стратегии: по папкам
        break;
    case 2:
        calculator = make_unique<Calculator>(make_unique<ByTypeCalculationStrategy>()); //создаем умный указатель на объект Calculator,
        //в конструктор Calculator передаем в качестве параметра умный указатель на объект конкретной стратегии: по типам
        break;
    default: //если значение strategy не попадает ни в один из случаев, то
        cout << "Wrong strategy." << endl;
        return 1;
    }

    QString qPath = QString::fromStdString(path); // Конвертация стандартной строки в объект QString
    QDir dir(qPath); // Создаем объект типа QDir, передавая в него текущий путь, для работы с директориями
    if (!dir.exists()) { //Если текущей директории не существует
        cout << "Directory doesn't exist." << endl;
        return 1;
    }
    QMap<QString, int> result = calculator->calculate(qPath); // В зависимости от выбранной стратегии вызывается метод
    // для вычисления размера файлов для текущей директории и результат вносится в словарь

    if (result.isEmpty()) { //Если директория пустая
        cout << "Current Directory: " << qPath.toStdString() << "  Size: 0 Кб " << " Percentage: 0.00%" << endl;
    }

    for (auto it = result.begin(); it != result.end(); ++it) { // Проходимся по элементам из списка для вывода информации
        QString name = it.key(); // имя файла/папки
        int size = it.value(); // размер файла/папки
        int totalSize = calcDirSize(dir); // Вычисляем общий размер всех файлов и папок в указанной директории
        QString percentage = percentageCalc(size, totalSize); // Находим процентное соотношение размера текущего файла/папки от общего размера
        //директории. Параметр инициализируется значением функции percentageCalc
        cout << "Name: " << name.toStdString() // Конвертация строки Qt в стандартную для вывода имени файла/папки
             << "  Size: " << size / 1024 << " Кб" // вывод размера каждого файла/папки
             << "  Percentage: " << percentage.toStdString() << endl; // вывод процентного соотношения
    }

    return 0;
}
