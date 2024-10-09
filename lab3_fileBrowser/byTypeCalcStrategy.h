#ifndef BYTYPECALCULATIONSTRATEGY_H
#define BYTYPECALCULATIONSTRATEGY_H
#include "calculationstrategy.h"
#include <QFileInfo> //предоставляет информацию об имени файла, размере, пути, позиции в файловой системе
#include <QDir> //обеспечивает доступ к структурам каталогов(папок) и их содержимому
#include <QMap>
#include <iostream>
using namespace std;

class ByTypeCalculationStrategy : public CalculationStrategy {//Класс, реализующий стратегию вычисления размера файлов, сгруппированных по типам
public:
    QMap<QString, int> calculate(QString path, int level ) { //метод для вычисления размера файлов для текущей директории,
        // на вход которого передается path - путь к текущей директории и уровень вложенности level в файловой системе
        QMap<QString, int> map; //Словарь, содержащий имя файла и его размер
        QFileInfo file(path); // Создаем объект типа QFileInfo для проверки информации о его текущей позиции в файловой системе

        if (file.isDir()) { // Проверка, является ли текущий файл директорией (папкой)
            QDir dir(path); // Создаем объект типа QDir, передавая в него текущий путь, для работы с директориями
            QFileInfoList filelist = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks); //Получаем список файлов из текущей директории

            for (const QFileInfo& fileInfo : filelist) { // Проходимся по файлам из этого списка
                QString type = fileInfo.suffix(); //Вычисляем расширение файла с помощью метода suffix()
                int file_size = fileInfo.size(); //Вычисляем размер текущего файла
                map[type] += file_size; // Заносим в словарь размер файла к соответствующему типу
            }

            QStringList subDirList = dir.entryList(QDir::NoDotAndDotDot | QDir::Dirs); // Получаем список поддиректорий (исключая родительскую
            // и текущую директории)
            for (const QString& subDirName : subDirList) { //Проходимся по каждой поддиректории из списка
                QString subDirPath = path + "/" + subDirName; // Формируем полный путь к поддиректории
                QMap<QString, int> subDirMap = this->calculate(subDirPath, 0); //Рекурсивно вызываем метод calculate для текущей поддиректории
                //с уровнем вложенности 0 и результат записываем в словарь для хранения информации о поддиректориях
                for (auto it = subDirMap.begin(); it != subDirMap.end(); ++it) { //Проходимся по элементам словаря поддиректорий
                    map[it.key()] += it.value(); //Заносим в словарь-результат тип файла и его размер в поддиректории
                }
            }
        }
        return map;
    }
};

#endif // BYTYPECALCULATIONSTRATEGY_H
