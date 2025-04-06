#include "plantcaresystemgui.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PlantCareSystemGUI w;
    w.show();
    return a.exec();
}
