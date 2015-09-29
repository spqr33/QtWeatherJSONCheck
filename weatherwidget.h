#ifndef WEATHERWIDGET_H
#define WEATHERWIDGET_H

#include <QWidget>
#include <QUrl>
#include "downloader.h"
#include <QProgressBar>
#include <QLineEdit>
#include <QPushButton>
#include <QByteArray>
#include <QTextEdit>
#include <QString>

class WeatherWidget : public QWidget
{
    Q_OBJECT
private:
    Downloader*     pdl_;

    QLineEdit*      ple_;
    QTextEdit*      pte_;

    QString urlRoot_;
    QString weatherQuery_;
    QString weatherImgQuery_;
    //void showData(const QString&);
public:
    explicit WeatherWidget(QWidget *parent = 0);
public slots:
    void slotGo(const QString&);

private slots:
    void slotError      ();
    void slotDone       (const QUrl&, const QByteArray&);
};

#endif // WEATHERWIDGET_H
