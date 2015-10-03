#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class Downloader : public QObject
{
    Q_OBJECT
private:
    QNetworkAccessManager*  pnam_;
public:
    explicit Downloader(QObject *parent = 0);

    void download(const QUrl&);
signals:
    void done(const QUrl&, const QByteArray&);
    void error();

private slots:
    void slotFinished(QNetworkReply*);
};

#endif // DOWNLOADER_H
