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
#include "picojson.h"
#include <QMap>

namespace Ui {
    class Form;
}

class WeatherWidget : public QWidget
{
    Q_OBJECT
private:
    Ui::Form*       ui;
    Downloader*     pdl_;
    Downloader*     pdlImg_;

    QString urlRoot_;
    QString weatherQuery_;
    QString weatherImgQuery_;
    typedef QMap<QString, QByteArray> ImgCache;
    ImgCache    cache_;
private:
    void JSONParseAndFill(picojson::value::object& obj);
    void JSONParseAndFill(picojson::value::array& obj);
    void displayError(const QString& title, const QString& message);
    void setTest(const QString& widgetName, const QString& value);
    void downloadImg(const QString& imgName);
    const QByteArray* checkCache(const QString& key);

public:
    explicit WeatherWidget(QWidget *parent = 0);
    ~WeatherWidget();
public slots:
    void slotGo         (const QString&);
    void slotClearFroms();
private slots:
    void slotError      ();
    void slotDone       (const QUrl&, const QByteArray&);
    void slotImgDone       (const QUrl&, const QByteArray&);

};

#endif // WEATHERWIDGET_H
