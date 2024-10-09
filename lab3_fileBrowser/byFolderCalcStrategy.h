#ifndef BYFOLDERCALCULATIONSTRATEGY_H
#define BYFOLDERCALCULATIONSTRATEGY_H
#include "calculationstrategy.h"
#include <QFileInfo> //предоставляет информацию об имени файла, размере, пути, позиции в файловой системе
#include <QDir> //обеспечивает доступ к структурам каталогов(папок) и их содержимому
#include <iostream>
using namespace std;



int calcDirSize(QDir dir) { //Функция для вычисления общего размера текущей директории, передаваемой в качестве параметра
    int dirSize = 0; //общий размер текущей директории
    QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot); //С помощью метода entryInfoList(), который содержит
    //флаги - Files(включает файлы), Dirs(включает директории), NoDotAndDotDot(исключает текущую - "." и родительскую - ".." директории),
    //получаем список всех файлов и поддиректорий

    for (auto it = list.constBegin(); it != list.constEnd(); ++it) { //Проходим по каждому элементу в списке файлов и поддиректорий
        //методы constBegin() и constEnd() используются только для чтения информации из элементов списка без возможности её изменения
        const QFileInfo& fileInfo = *it; // Ссылке на объект QFileInfo присваиваем операцию разыменования указателя на элемент из списка для
        //получения информации о текущем файле или поддиректории
        if (fileInfo.isDir()) {//Если текущий элемент является поддиректорией, то
            QString subDirPath = dir.path() + "/" + fileInfo.fileName(); //Получаем полный путь для текущей директории
            dirSize += calcDirSize(subDirPath); // Рекурсивный вызов метода для подсчета размера поддиректории
        }
        else {
            dirSize += fileInfo.size(); // Увеличиваем общий размер директории на размер текущего файла
        }
    }
    return dirSize;
}

class ByFolderCalculationStrategy : public CalculationStrategy//Класс, реализующий стратегию вычисления размера файлов, сгруппированных по папкам
{
public:
    QMap<QString, int> calculate(QString path, int level) { // метод для вычисления размера папок для текущей директории,
        // на вход которого передается path - путь к текущей директории и уровень вложенности level в файловой системе
        Q_UNUSED(level); // Указывает, что параметр level не используется
        QMap<QString, int> map; // Словарь для хранения имени папок и их размеров
        QFileInfo file(path); // Создаем объект типа QFileInfo для проверки информации о его текущей позиции в файловой системе
        int folder_size = 0; // Размер текущей папки

        if (file.isDir()) { // Проверка, является ли текущий файл директорией (папкой)
            QDir dir(path); // Создаем объект типа QDir, передавая в него текущий путь, для работы с директориями
            QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks); // Создаем объект типа QFileInfoList и
            // получаем список файлов и информацию о них с помощью метода entryInfoList() для текущей директории с учетом флагов:
            // Files - включаются только файлы (а не директории) в список-результат, Hidden - включаются скрытые файлы
            // (начинающиеся с точки), NoSymLinks - исключаются символьные ссылки (например ярлыки, указывающие на реальный путь файла)

            for (const QFileInfo& fileInfo : fileList) { // Проходимся по каждому файлу из текущей папки
                folder_size += fileInfo.size(); // Вычисляем размер папки, суммируя все файлы в ней
            }

            QString currentDirName = dir.dirName(); // Получаем имя текущей директории
            map.insert(currentDirName, folder_size); // Заносим в словарь имя и размер текущей папки

            QStringList subDirList = dir.entryList(QDir::NoDotAndDotDot | QDir::Dirs); // Создаем список поддиректорий
            // (папок внутри текущей директории), используя фильтры NoDotAndDotDot (исключает символы "." - текущая директория и ".." -
            // - родительская директория) и Dirs (включает только директории)
            for (const QString& subDirName : subDirList) { // Проходимся по каждой поддиректории (папке) в текущей директории
                QString subDirPath = path + "/" + subDirName; // Формируем полный путь к поддиректории
                int subDirSize = calcDirSize(subDirPath); // Рекурсивно вызываем метод calcDirSize() и вычисляем общий размер поддиректории
                map.insert(subDirName, subDirSize); // Добавляем имя и размер поддиректории в словарь
            }
        }
        return map;
    }
};



#endif // BYFOLDERCALCULATIONSTRATEGY_H
