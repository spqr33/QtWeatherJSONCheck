#include "lo_treewidget.h"
#include "data.h"
#include <QByteArray>
#include <QFile>
#include <memory>
#include <iostream>
#include <QDebug>
#include <QStringList>
#include <cassert>

LO_TreeWidget::LO_TreeWidget(const QString& sourceFileName, QWidget *parent) :
    QTreeWidget(parent),
    sourceFileName_(sourceFileName)
{
    setMinimumWidth(200);
    QStringList lst;
    lst << "Name" << "Id" << "Country" << "Lon" << "Lat";
    setHeaderLabels(lst);
    setSortingEnabled(true);

}

bool LO_TreeWidget::loadData()
{
    QFile file(sourceFileName_);
   if ( !file.open(QIODevice::ReadOnly)) {
       return  false;
   } else {
       QHash<QChar, QTreeWidgetItem*> firstSymbolHash;
       QByteArray b_array(1024, '\0');
       std::shared_ptr<City> sp_city;
       while ( file.readLine(b_array.data(), 1024) != -1 ) {

           sp_city = City::makeCity(b_array);
           if ( sp_city.get() != nullptr){
              QChar firstCh = sp_city->name().at(0);

              QTreeWidgetItem* p_topLevel = 0;
              auto iter = firstSymbolHash.find(firstCh);
              if ( iter == firstSymbolHash.cend()) {
                  p_topLevel = new QTreeWidgetItem(this);
                  p_topLevel->setText(0, firstCh);
                  firstSymbolHash[firstCh] = p_topLevel;
              } else {
                  p_topLevel = iter.value();
              }
              assert(p_topLevel!= 0 &&
                      "bool LO_TreeWidget::loadData(),  QTreeWidgetItem* p_topLevel = 0"
                      );
              QTreeWidgetItem* p_twgItem = new QTreeWidgetItem(p_topLevel);
              p_twgItem->setText(0, sp_city->name());
              p_twgItem->setText(1, QString::number(sp_city->id()));
              p_twgItem->setText(2, sp_city->country());
              p_twgItem->setText(3, QString::number(sp_city->coord().lon()));
              p_twgItem->setText(4, QString::number(sp_city->coord().lat()));
           }
       }
       sortByColumn(0,Qt::AscendingOrder);
   }
   connect(this,
           SIGNAL(itemActivated(QTreeWidgetItem*,int)),
           SLOT(slot_emitActivated(QTreeWidgetItem*,int))
                  );
           return true;
}

void LO_TreeWidget::slot_emitActivated(QTreeWidgetItem* item ,int col) {
    emit activated(item->text(0));
}
