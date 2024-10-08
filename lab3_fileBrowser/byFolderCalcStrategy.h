#ifndef BYFOLDERCALCULATIONSTRATEGY_H
#define BYFOLDERCALCULATIONSTRATEGY_H
#include "calculationstrategy.h"
#include <QFileInfo> //предоставляет информацию об имени файла, размере, пути, позиции в файловой системе
#include <QDir> //обеспечивает доступ к структурам каталогов(папок) и их содержимому
#include <iostream>
using namespace std;

class ByFolderCalculationStrategy : public CalculationStrategy//Класс, реализующий стратегию вычисления размера файлов, сгруппированных по папкам
{
public:
    QMap<QString, QPair<int, int>> calculate(QString path, int level) { // метод для вычисления общего размера файлов, сгруппированных
        // по папкам, на вход которого передается path - путь к текущей директории и уровень вложенности level в файловой системе
        QMap<QString, QPair<int, int>> map; // Создаем карту для хранения имени папок и их размеров, а также общего размера директории
        QFileInfo file(path); // Создаем объект типа QFileInfo для проверки информации о его текущей позиции в файловой системе
        int folder_size = 0; // Размер всех файлов в текущей папке

        if (file.isDir()) { // Проверка, является ли текущий файл директорией (папкой), с помощью метода isDir()
            QDir dir(path); // Создаем объект типа QDir, передавая в него текущий путь, для работы с директориями

            QFileInfoList listInfo = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks); // Создаем объект типа QFileInfoList и
            // получаем список файлов и информацию о них с помощью метода entryInfoList() для текущей директории с учетом флагов:
            // Files - включаются только файлы (а не директории) в список-результат, Hidden - включаются скрытые файлы
            // (начинающиеся с точки), NoSymLinks - исключаются символьные ссылки (например ярлыки, указывающие на реальный путь файла)

            for (const QFileInfo& fileInfo : listInfo) { // Проходимся по каждому файлу в текущей директории
                folder_size += fileInfo.size(); // Вычисляем размер текущей папки, суммируя все файлы в ней
            }

            QStringList listDir = dir.entryList(QDir::NoDotAndDotDot | QDir::Dirs); // Создаем список строк и получаем список поддиректорий
            // (папок внутри текущей директории), используя фильтры NoDotAndDotDot (исключает символы "." - текущая директория и ".." -
            // - родительская директория) и Dirs (включает только директории)
            int sub_total_size = 0; // Общий размер всех поддиректорий

            for (const QString& subDirName : listDir) { // Проходимся по каждой поддиректории (папке) в текущей директории
                QString subDirPath = path + "/" + subDirName; // Формируем полный путь к поддиректории
                QMap<QString, QPair<int, int>> subDirMap = this->calculate(subDirPath, level + 1); // Рекурсивно вызываем метод calculate для поддиректории,
                // увеличивая уровень вложенности на 1

                for (auto it = subDirMap.begin(); it != subDirMap.end(); ++it) { // Проходимся по всем элементам из карты поддиректорий
                    sub_total_size += it.value().first; // Добавляем размер поддиректории к общему размеру
                    map.insert(it.key(), it.value()); // Добавляем информацию из поддиректорий в общую карту
                }
            }

            int total_size = folder_size + sub_total_size; // Общий размер текущей директории (включая поддиректории)
            map.insert(dir.dirName(), qMakePair(folder_size, total_size)); // Добавляем информацию о текущей директории в карту
        }
        return map;
    }
};

#endif // BYFOLDERCALCULATIONSTRATEGY_H
