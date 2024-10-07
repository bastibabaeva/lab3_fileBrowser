#ifndef BYFOLDERCALCSTRATEGY_H
#define BYFOLDERCALCSTRATEGY_H
#include "calculationstrategy.h"
#include <QFileInfo> //предоставляет информацию об имени файла, размере, пути, позиции в файловой системе
#include <QDir> //обеспечивает доступ к структурам каталогов(папок) и их содержимомму

class ByFolderCalculationStrategy : public CalculationStrategy
{
public:
    QList<QPair<QString, QPair<int, int>>> calculate(QString path, int level) { //метод для вычисления общего размера файлов, сгруппированных по папкам,
        // на вход которого передается ссылка path - путь к текущей директории и уровень вложенности level в файловой системе
        QList<QPair<QString, QPair<int, int>>> list; //Создаем список для хранения типа файлов и их размеров, а также общего размера директории
        QFileInfo file(path); // Создаем объект типа QFileInfo для проверки информации о его текущей позиции в файловой системе
        int total_size = 0; // Общий размер файлов директории

        if (file.isDir()) {// Проверка, является ли текущий файл директорией(папкой), с помощью метода isDir()
            QDir dir(path); // Создаем объект типа QDir, передавая в него текущий путь, для работы с директориями
            QFileInfoList listInfo = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks); // Создаем объект типа QFileInfoList и
            //получаем список файлов и информацию о них с помощью метода entryInfoList() для текущей директории с учетом флагов:
            // Files - включаются только файлы (а не директории) в список-результат, Hidden - включаются скрытые файлы
            //(начинающиеся с точки), NoSymLinks - исключаются символьные ссылки (например ярлыки, указывающие на реальный путь файла)

            for (const QFileInfo& fileInfo : listInfo) { // Проходимся по каждому файлу в текущей директории
                int file_size = fileInfo.size(); // Получаем размер текущего файла
                total_size += file_size; // Добавляем размер текущего файла к общему размеру директории
                list.append(QPair<QString, QPair<int, int>>(fileInfo.fileName(), qMakePair(file_size, total_size))); // Добавляем информацию о файле
                //в список т.е. с помощью метода append() добавляем пару(имя файла и (размер текущего файла, общий размер файлов в текущей директории.)).
            }

            QStringList listDir = dir.entryList(QDir::NoDotAndDotDot | QDir::Dirs); // Создаем список строк и получаем список поддиректорий
            //(папок внутри текущей директории), используя фильтры NoDotAndDotDot (исключает символы "." - текущая директория и ".." -
            // - родительская директория) и Dirs (включает только директории)
            for (const QString& subDirName : listDir) { // Проходимся по каждой поддиректории (папке) в текущей директории
                QString subDirPath = path + "/" + subDirName; // Формируем полный путь к поддиректории
                QList<QPair<QString, QPair<int, int>>> subDirList = this->calculate(subDirPath, level + 1); // Рекурсивно вызываем метод calculate для поддиректории,
                // увеличивая уровень вложенности на 1

                for (const auto& fileInfo : subDirList) { // Проходимся по всем элементам из текущей поддиректории
                    QString type = fileInfo.first; // Получаем имя файла
                    int size = fileInfo.second.first; // Получаем размер текущего файла
                    int subDirTotalSize = fileInfo.second.second; // Получаем общий размер файлов в поддиректории
                    list.append(QPair<QString, QPair<int, int>>(type, qMakePair(size, total_size + subDirTotalSize))); //Добавляем имя, размер
                    //текущего элемента и добавляем размер файла к общему размеру директории в список
                }
            }
        }
        return list;

    }
};


#endif // BYFOLDERCALCSTRATEGY_H
