#include <iostream>
#include "calculator.h"
#include "byFolderCalcStrategy.h"
#include "byTypeCalcStrategy.h"
#include <memory>
#include <windows.h> // Для использования функции SetConsoleOutputCP
using namespace std;

int main()
{
    SetConsoleOutputCP(CP_UTF8); // Установка кодировки консоли для корректного отображения русских символов
    cout << "Введите путь к директории: ";
    string path;
    getline(cin, path);

    cout << "Выберите стратегию вычисления размеров файлов: 1 - по папкам, 2 - по типам. ";
    int strategy; cin >> strategy;

    unique_ptr<Calculator> calculator;
    switch(strategy) {
    case 1: {
        calculator = make_unique<Calculator>(new ByFolderCalculationStrategy);
        break;
    }
    case 2: {
        calculator = make_unique<Calculator>(new ByTypeCalculationStrategy);
        break;
    }
    default: {
        cout << "Неверная стратегия." << endl;
        return 1;
    }
    }

    QList<QPair<QString, QPair<int, int>>> result = calculator->calculate(QString::fromStdString(path));

    if (result.isEmpty()) {
        cout << "В директории ничего не найдено или доступ к директории невозможен." << endl;
    }
    else { // Вывод результата
        for (const auto& fileInfo : result) {
            cout << "Имя файла: " << fileInfo.first.toStdString()
                 << ", Размер файла: " << fileInfo.second.first / 1024 << " Кб"
                 << ", Общий размер директории: " << fileInfo.second.second / 1024 << " Кб" << endl;
        }
    }
    return 0;
}
