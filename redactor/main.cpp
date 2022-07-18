#include "redactor.h"

int main(int argc, char *argv[])
{
    QStringList paths(QApplication::libraryPaths());
    paths.append(".");

    QApplication a(argc, argv);
    a.setLibraryPaths(paths);
    Redactor w;
    w.show();
    return a.exec();
}
