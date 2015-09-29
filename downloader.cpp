#include "downloader.h"
#include <QNetworkRequest>

Downloader::Downloader(QObject *parent) :
    QObject(parent)
{
    pnam_ = new QNetworkAccessManager(this);
    connect(pnam_, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(slotFinished(QNetworkReply*))
            );
}

void Downloader::download(const QUrl & url)
{
    QNetworkRequest request(url);

    QNetworkReply* reply = pnam_->get(request);
//    connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
//            this, SIGNAL(downloadProgress(qint64,qint64))
//            );

}

void Downloader::slotFinished(QNetworkReply *reply)
{
    if(reply->error() != QNetworkReply::NoError){
        emit error();
    } else {
        emit done(reply->url(), reply->readAll());
    }
    reply->deleteLater();
}
