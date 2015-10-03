#include <QApplication>
#include "lo_treewidget.h"
#include "weatherwidget.h"
#include <QHBoxLayout>
#include <QWidget>
#include "logger.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(QPixmap(":/icons/mainIcon.png")));

    Logger log(Logger::mode::clear);

    QWidget central;
    LO_TreeWidget tree(":/allcities/city.list1.json");
    if ( !tree.loadData()) {
        return -1;
    }
    tree.show();

    //
    WeatherWidget weather;
    weather.show();
    weather.connectLogger(log);
    QObject::connect(&tree, SIGNAL(activated(QString)),
                     &weather, SLOT(slotGo(QString))
                     );

    //Layouts
    QHBoxLayout* phbl = new QHBoxLayout();
    phbl->addWidget(&tree);
    phbl->addWidget(&weather);

    central.setLayout(phbl);
    central.show();

    return a.exec();
}
