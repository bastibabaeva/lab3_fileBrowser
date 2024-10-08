#ifndef BYTYPECALCSTRATEGY_H
#define BYTYPECALCSTRATEGY_H
#include "calculationstrategy.h"
#include <QFileInfo> //предоставляет информацию об имени файла, размере, пути, позиции в файловой системе
#include <QDir> //обеспечивает доступ к структурам каталогов(папок) и их содержимому
#include <iostream>
using namespace std;

class ByTypeCalculationStrategy : public CalculationStrategy //Класс, реализующий стратегию вычисления размера файлов, сгруппированных по типам
{
public:
    QList<QPair<QString, QPair<int, int>>> calculate(QString path, int level) { // метод для вычисления общего размера файлов,
        //сгруппированных по типам (расширениям файлов), на вход которого передается path - путь к текущей директории и уровень
        //вложенности level в файловой системе. Тип возвращаемого значения это список пар(тип файла, пара(размер файла, общий размер директории))
        QList<QPair<QString, QPair<int, int>>> list; // Создаем список для хранения типа файлов, его размеров и их общего размера

        QFileInfo file(path); // Создаем объект типа QFileInfo для проверки информации о текущей позиции в файловой системе.
        int total_size = 0; // Общий размер директории

        if (file.isDir()) { // Проверка, является ли текущий путь директорией (папкой) с помощью метода isDir().
            QDir dir(path); // Создаем объект типа QDir, передавая в него текущий путь, для работы с директориями.
            QFileInfoList listInfo = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks); //Создаем объект типа QFileInfoList и
            // получаем список файлов и информацию о них с помощью метода entryInfoList() для текущей директории с учетом флагов:
            // Files - включаются только файлы (а не директории) в список-результат, Hidden - включаются скрытые файлы (начинающиеся с точки),
            // NoSymLinks - исключаются символьные ссылки (например ярлыки).

            for (const QFileInfo& fileInfo : listInfo) { // Проходимся по каждому файлу в текущей директории.
                total_size += fileInfo.size(); //сохраняем общий размер файлов для текущей директории
                QString type = fileInfo.suffix(); // Получаем расширение файла (тип файла) с помощью метода suffix()
                int file_size = fileInfo.size(); // Получаем размер текущего файла
                list.append(QPair<QString, QPair<int, int>>(type, qMakePair(file_size, total_size))); // Добавляем информацию о файле в список,
                //т.е. с помощью метода append() добавляем пару(тип файла и (размер текущего файла, общий размер файлов в текущей директории)).
            }
            QStringList listDir = dir.entryList(QDir::NoDotAndDotDot | QDir::Dirs); //Создаем список строк и получаем список поддиректорий
            //(папок внутри текущей директории), используя фильтры NoDotAndDotDot (исключает символы "." - текущая директория и ".." -
            // - родительская директория) и Dirs (включает только директории)
            for (const QString& subDirName : listDir) { // Проходимся по каждой поддиректории(папке) в списке listDir.
                QString subDirPath = path + "/" + subDirName; // Формируем полный путь к поддиректории.
                QList<QPair<QString, QPair<int, int>>> subDirList = this->calculate(subDirPath, level + 1); // Добавляем информацию о каждом файле
                //из поддиректории в список. Рекурсивно вызываем метод calculate, увеличивая уровень вложенности на 1.

                for (const auto& fileInfo : subDirList) { // Проходимся по всем элементам из списка поддиректории
                    QString type = fileInfo.first; // Получаем тип файла (расширение) из первого элемента пары
                    int size = fileInfo.second.first; // Получаем размер текущего файла из первой части второй пары
                    int subDirTotalSize = fileInfo.second.second; // Получаем общий размер файлов в поддиректории из второй части второй пары
                    list.append(QPair<QString, QPair<int, int>>(type, qMakePair(size, subDirTotalSize))); //Добавляем информацию об элементе
                    //в список, т.е. с помощью метода append() добавляем пару(тип файла и (размер текущего файла, общий размер файлов в текущей директории.)).
                }
            }
        }
        return list;
    }
};
#endif // BYTYPECALCSTRATEGY_H
