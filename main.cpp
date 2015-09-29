#include <QApplication>
#include "lo_treewidget.h"
#include "weatherwidget.h"
#include <QHBoxLayout>
#include <QWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget central;
    LO_TreeWidget tree(":/allcities/city.list1.json");
    if ( !tree.loadData()) {
        return -1;
    }
    tree.show();

    WeatherWidget weather;
    weather.show();

    QObject::connect(&tree, SIGNAL(activated(QString)),
                     &weather, SLOT(slotGo(QString))
                     );

    QHBoxLayout* phbl = new QHBoxLayout();
    phbl->addWidget(&tree);
    phbl->addWidget(&weather);

    central.setLayout(phbl);
    central.show();

    return a.exec();
}
