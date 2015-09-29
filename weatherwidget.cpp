#include "weatherwidget.h"
#include <QGridLayout>
#include <QDebug>
#include <QMessageBox>

WeatherWidget::WeatherWidget(QWidget *parent) :
    QWidget(parent)
{
    urlRoot_ = "http://api.openweathermap.org/";
    weatherQuery_ = "data/2.5/weather/?q=";
    weatherImgQuery_ = "img/w/";

    pdl_ = new Downloader(this);

    ple_ = new QLineEdit;
    pte_ = new QTextEdit;

    connect(pdl_, SIGNAL(done(const QUrl&,const QByteArray&)), this, SLOT(slotDone(const QUrl&,const QByteArray&)));

    QGridLayout* pLayout = new QGridLayout;
    pLayout->addWidget(ple_, 0, 0);
    pLayout->addWidget(pte_, 1, 0, 3, 2);
    setLayout(pLayout);

}

void WeatherWidget::slotGo(const QString & url)
{
    QString request = urlRoot_ + weatherQuery_ + url;
    pdl_->download(QUrl(request));
    ple_->setText(request);
}

void WeatherWidget::slotError()
{
    QMessageBox::critical(0,
    tr("Error"),
    tr("An error while download is occured")
    );
}

void WeatherWidget::slotDone(const QUrl &url, const QByteArray &ba)
{
    qDebug() << "URL: " << url;
    qDebug() << "Data: " << ba;
    pte_->setPlainText(ba);
}
