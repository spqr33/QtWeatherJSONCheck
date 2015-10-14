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
#include "logger.h"

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
    QString imgExtention_;
    const QString APPID_;
    typedef QMap<QString, QByteArray> ImgCache;
    ImgCache    cache_;
private:
    void JSONParseAndFill(picojson::value::object& obj);
    void JSONParseAndFill(picojson::value::array& obj);
    void displayError(const QString& title, const QString& message);
    void setText(const QString& widgetName, const QString& value);
    void downloadImg(const QString& imgName);
    const QByteArray* checkCache(const QString& key);
public:
    explicit WeatherWidget(QWidget *parent = 0);
    ~WeatherWidget();

    void connectLogger(const Logger& logger);
public slots:
    void slotGo         (const QString&);
    void slotClearFroms ();
private slots:
    void slotError      ();
    void slotDone       (const QUrl&, const QByteArray&);
    void slotImgDone    (const QUrl&, const QByteArray&);
    void slotResetToDefault (bool);
    void slotError          (QString errStatus);
};

#endif // WEATHERWIDGET_H
