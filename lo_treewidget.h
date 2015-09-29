#ifndef LO_TREEWIDGET_H
#define LO_TREEWIDGET_H

#include <QTreeWidget>
#include <QObject>
#include <QWidget>
#include <QHash>

class LO_TreeWidget : public QTreeWidget
{
    Q_OBJECT
private:
    QString sourceFileName_;

public:
    explicit LO_TreeWidget(const QString& sourceFileName, QWidget *parent = 0);
    bool loadData();
signals:
    void activated(const QString& cityName);
protected slots:
    void slot_emitActivated(QTreeWidgetItem*,int);
};

#endif // LO_TREEWIDGET_H
