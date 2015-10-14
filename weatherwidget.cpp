#include "weatherwidget.h"
#include <QGridLayout>
#include <QDebug>
#include <QMessageBox>
#include <iostream>
#include "ui_weather_display.h"
#include <QWidget>
#include <QList>

WeatherWidget::WeatherWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form),
    APPID_("f8bbb1a030af9c566c1a284abbeb7db8")
{
    ui->setupUi(this);
    urlRoot_ = "http://api.openweathermap.org/";
    weatherQuery_ = "data/2.5/weather/?q=";
    weatherImgQuery_ = "img/w/";
    imgExtention_ = ".png";

    ui->APPID->setText(APPID_);
    pdl_ = new Downloader(this);
    pdlImg_ = new Downloader(this);

    connect(pdl_, SIGNAL(done(const QUrl&,const QByteArray&)), this, SLOT(slotDone(const QUrl&,const QByteArray&)));
    connect(pdlImg_, SIGNAL(done(const QUrl&,const QByteArray&)), this, SLOT(slotImgDone(const QUrl&,const QByteArray&)));
    connect(ui->btn_resetAPPID, SIGNAL(clicked(bool)), this, SLOT(slotResetToDefault(bool)));
    connect(pdl_, SIGNAL(error(QString)), this, SLOT(slotError(QString)));
}

WeatherWidget::~WeatherWidget()
{
    delete ui;
}

void WeatherWidget::connectLogger(const Logger &logger)
{
       connect(pdl_, SIGNAL(done(const QUrl&,const QByteArray&)),
            &logger, SLOT(slotLog(const QUrl&,const QByteArray&))
            );
       connect(pdlImg_, SIGNAL(done(const QUrl&,const QByteArray&)),
            &logger, SLOT(slotLog(const QUrl&))
            );
}

void WeatherWidget::slotGo(const QString & url)
{
    slotClearFroms();
    QString request = urlRoot_ + weatherQuery_ + url +"&APPID=" + ui->APPID->text();
    pdl_->download(QUrl(request));
}

void WeatherWidget::slotClearFroms()
{
    {
        QList<QLineEdit*> list = ui->layoutWidget->findChildren<QLineEdit*>();
        QList<QLineEdit*>::iterator iter = list.begin();
        for( ; iter != list.end(); ++iter ) {
            if ( (*iter)->objectName() == QString("APPID") ){
                continue;
            }
            (*iter)->clear();
        }
    }
    {
        QList<QTextEdit*> list = ui->layoutWidget->findChildren<QTextEdit*>();
        QList<QTextEdit*>::iterator iter = list.begin();
        for( ; iter != list.end(); ++iter ) {
            (*iter)->clear();
        }
    }
    {
        ui->image->clear();
    }
}

void WeatherWidget::slotError()
{
    displayError("Error", "An error while download occured");
}

void WeatherWidget::slotDone(const QUrl &url, const QByteArray &ba)
{
    ui->textEdit->setPlainText(ba);
    ui->requestLine->setText(url.toString());

    picojson::value         v;
    std::string             error;

    picojson::parse(v, ba.constData(), ba.constData()+ ba.size(), &error);
    if (!error.empty()) {
        displayError("Error", QString("Error occured ") + QString().fromStdString(error));
        return;
    }
    if (!v.is<picojson::object>()) {
        displayError("Error", "It is not JSON object");
        return;
    } else {
        JSONParseAndFill(v.get<picojson::object>());
    }
}

void WeatherWidget::slotImgDone(const QUrl &, const QByteArray &ba)
{
    QPixmap pix;
    pix.loadFromData(ba);
    ui->image->setPixmap(pix);

    QVariant qvar = ui->image->property("fileName");
    QString name = qvar.value<QString>();

    cache_[name] = ba;
}

void WeatherWidget::slotResetToDefault(bool)
{
    ui->APPID->setText(APPID_);
}

void WeatherWidget::slotError(QString errStatus)
{
    qDebug() << "Debug message void WeatherWidget::slotError(QString errStatus)";
    displayError("Error", errStatus);
}

void WeatherWidget::JSONParseAndFill(picojson::value::object &obj)
{
    picojson::value::object::const_iterator i = obj.begin();
    for ( ; i != obj.end(); ++i ) {
        picojson::value v =  i->second;
        if (i->second.is<picojson::object>()) {
            JSONParseAndFill(v.get<picojson::object>());
        } else if (i->second.is<picojson::array>()) {
            JSONParseAndFill(v.get<picojson::array>());
        } else {
            setText(QString().fromStdString(i->first),
                    QString().fromStdString(i->second.to_str())
                    );
        }
    }
}

void WeatherWidget::JSONParseAndFill(picojson::value::array &obj)
{
    picojson::value::array::iterator i = obj.begin();
    for ( ; i != obj.end(); ++i ) {
        picojson::value v = *i;

        if ((*i).is<picojson::object>()) {
             JSONParseAndFill(v.get<picojson::object>());
        } else if ( (*i).is<picojson::array>() ) {
            JSONParseAndFill(v.get<picojson::array>());
        }
    }
}

void WeatherWidget::displayError(const QString &title, const QString& message)
{
    QMessageBox::critical(0, title, message);
}

void WeatherWidget::setText(const QString &widgetName, const QString &value)
{
        QWidget* curr = ui->layoutWidget->findChild<QWidget*>(widgetName);

        if (curr && curr->metaObject()->className() == QString("QLineEdit") ) {
            if (curr->objectName() == "icon") {
                ui->image->setProperty("fileName", QVariant(value));
                downloadImg(value);
            }
            qobject_cast<QLineEdit*>(curr)->setText(value);
        }
}

void WeatherWidget::downloadImg(const QString &imgName)
{
    QString request = urlRoot_ + weatherImgQuery_ + imgName + imgExtention_;

    qDebug() << request;

    const QByteArray* ba = checkCache(imgName);
    if( ba != nullptr ) {
        qDebug() << request << "From cache";
        slotImgDone(QUrl(request), *ba);
    } else {
        pdlImg_->download(QUrl(request));
    }
}

const QByteArray *WeatherWidget::checkCache(const QString &key)
{
    ImgCache::const_iterator i = cache_.find(key);

    if ( i != cache_.end() ) {
        return &(*i);
    }

    return nullptr;
}
