#ifndef BYTYPECALCULATIONSTRATEGY_H
#define BYTYPECALCULATIONSTRATEGY_H
#include "calculationstrategy.h"
#include <QFileInfo> //предоставляет информацию об имени файла, размере, пути, позиции в файловой системе
#include <QDir> //обеспечивает доступ к структурам каталогов(папок) и их содержимому
#include <QMap>
#include <iostream>
using namespace std;

class ByTypeCalculationStrategy : public CalculationStrategy {
public:
    QMap<QString, QPair<int, int>> calculate(QString path, int level = 0) override {
        QMap<QString, QPair<int, int>> map;
        QFileInfo file(path);
        int total_size = 0;

        if (file.isDir()) {
            QDir dir(path);
            QFileInfoList listInfo = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
            QMap<QString, QPair<int, int>> typeSizeMap;

            for (const QFileInfo& fileInfo : listInfo) {
                QString type = fileInfo.completeSuffix();
                int file_size = fileInfo.size();

                if (typeSizeMap.contains(type)) {
                    QPair<int, int>& pair = typeSizeMap[type];
                    pair.first += file_size;
                    pair.second += file_size;
                } else {
                    typeSizeMap[type] = qMakePair(file_size, file_size);
                }
            }

            QStringList listDir = dir.entryList(QDir::NoDotAndDotDot | QDir::Dirs);
            for (const QString& subDirName : listDir) {
                QString subDirPath = path + "/" + subDirName;
                QMap<QString, QPair<int, int>> subDirMap = this->calculate(subDirPath, level + 1);

                for (auto it = subDirMap.begin(); it != subDirMap.end(); ++it) {
                    if (typeSizeMap.contains(it.key())) {
                        QPair<int, int>& pair = typeSizeMap[it.key()];
                        pair.first += it.value().first;
                        pair.second += it.value().second;
                    } else {
                        typeSizeMap[it.key()] = it.value();
                    }
                }
            }

            for (auto it = typeSizeMap.constBegin(); it != typeSizeMap.constEnd(); ++it) {
                map.insert(it.key(), it.value());
            }
        }

        return map;
    }
};

#endif // BYTYPECALCULATIONSTRATEGY_H
