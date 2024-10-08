#include <iostream>
#include "calculator.h"
#include "byFolderCalcStrategy.h"
#include "byTypeCalcStrategy.h"
#include <memory>
#include <QString>
#include <QDir>

using namespace std;

QString percentageCalc(int size, int total_size) { //Функция для вычисления процентного соотношения текущего файла/папки от общего размера директории
    if (total_size != 0 && size != 0) { //Если общий размер директории не равен 0, то
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
    cin.ignore(); // Игнорируем символ новой строки после ввода стратегии

    unique_ptr<Calculator> calculator; // Создаем умный указатель на объект-стратегию
    switch (strategy) {
    case 1:
        calculator = make_unique<Calculator>(make_unique<ByFolderCalculationStrategy>());
        break;
    case 2:
        calculator = make_unique<Calculator>(make_unique<ByTypeCalculationStrategy>());
        break;
    default:
        cout << "Wrong strategy." << endl;
        return 1;
    }

    QString qPath = QString::fromStdString(path); // Конвертация стандартной строки в объект QString
    QDir dir(qPath); // Создаем объект типа QDir, передавая в него текущий путь, для работы с директориями.
    if (!dir.exists()) { // Если текущей директории не существует
        cout << "Directory doesn't exist." << endl;
        return 1;
    }
    QMap<QString, QPair<int, int>> result = calculator->calculate(qPath); // В зависимости от выбранной стратегии вызывается метод
    // для вычисления размера файлов для текущего пути и результат вносится в список из пары <имя, <размер, общий размер директории>>
    if (result.isEmpty()) { // Если директория пустая
        cout << "Current Directory: " << qPath.toStdString() << "  Size: 0 Кб " << " Percentage: 0.00%" << endl;
    } else {
        int totalSize = 0;
        for (const auto& fileInfo : result) { // Проходимся по файлам из списка и вычисляем общий размер текущей директории
            totalSize += fileInfo.second.first; // fileInfo.second.first - это первое значение из второй пары списка
        }
        for (const auto& fileInfo : result) { // Также проходимся по элементам из списка для вывода информации
            int size = fileInfo.second.first; // размер файла/папки
            QString percentage = percentageCalc(size, totalSize); // Параметр инциализируется значением функции percentageCalc, в
            // которую передается текущий размер файла/папки и общий размер директории
            cout << "Name: " << fileInfo.first.toStdString() // Конвертация строки Qt в стандартную для вывода имени файла/папки
                 << "  Size: " << size / 1024 << " Кб" // вывод размера каждого файла/папки
                 << "  Percentage: " << percentage.toStdString() << endl; // вывод процентного соотношения от общего размера директории
        }
    }
    return 0;
}
