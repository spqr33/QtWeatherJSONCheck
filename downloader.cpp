#include "downloader.h"
#include <QNetworkRequest>

QString Downloader::getStringError(QNetworkReply::NetworkError err)
{
    QString error;

    switch (err) {
    case QNetworkReply::ConnectionRefusedError: error = "the remote server refused the connection (the server is not accepting requests)"; break;
    case QNetworkReply::RemoteHostClosedError: error = "the remote server closed the connection prematurely, before the entire reply was received and processed"; break;
    case QNetworkReply::HostNotFoundError: error = "the remote host name was not found (invalid hostname)"; break;
    case QNetworkReply::TimeoutError: error = "the connection to the remote server timed out"; break;
    case QNetworkReply::OperationCanceledError: error = "the operation was canceled via calls to abort() or close() before it was finished"; break;
    case QNetworkReply::SslHandshakeFailedError: error = "the SSL/TLS handshake failed and the encrypted channel could not be established. The sslErrors() signal should have been emitted"; break;
    case QNetworkReply::TemporaryNetworkFailureError: error = "the connection was broken due to disconnection from the network, however the system has initiated roaming to another access point. The request should be resubmitted and will be processed as soon as the connection is re-established."; break;
    case QNetworkReply::ProxyConnectionRefusedError: error = "the connection to the proxy server was refused (the proxy server is not accepting requests)"; break;
    case QNetworkReply::ProxyConnectionClosedError: error = "the proxy server closed the connection prematurely, before the entire reply was received and processed"; break;
    case QNetworkReply::ProxyNotFoundError: error = "the proxy host name was not found (invalid proxy hostname)"; break;
    case QNetworkReply::ProxyTimeoutError: error = "the connection to the proxy timed out or the proxy did not reply in time to the request sent"; break;
    case QNetworkReply::ProxyAuthenticationRequiredError: error = "the proxy requires authentication in order to honour the request but did not accept any credentials offered (if any)"; break;
    case QNetworkReply::ContentAccessDenied: error = "the access to the remote content was denied (similar to HTTP error 401)"; break;
    case QNetworkReply::ContentOperationNotPermittedError: error = "the operation requested on the remote content is not permitted"; break;
    case QNetworkReply::ContentNotFoundError: error = "the remote content was not found at the server (similar to HTTP error 404)"; break;
    case QNetworkReply::AuthenticationRequiredError: error = "the remote server requires authentication to serve the content but the credentials provided were not accepted (if any)"; break;
    case QNetworkReply::ContentReSendError: error = "the request needed to be sent again, but this failed for example because the upload data could not be read a second time."; break;
    case QNetworkReply::ProtocolUnknownError: error = "the Network Access API cannot honor the request because the protocol is not known"; break;
    case QNetworkReply::ProtocolInvalidOperationError: error = "the requested operation is invalid for this protocol"; break;
    case QNetworkReply::UnknownNetworkError: error = "an unknown network-related error was detected"; break;
    case QNetworkReply::UnknownProxyError: error = "an unknown proxy-related error was detected"; break;
    case QNetworkReply::UnknownContentError: error = "an unknown error related to the remote content was detected"; break;
    case QNetworkReply::ProtocolFailure: error = "a breakdown in protocol was detected (parsing error, invalid or unexpected responses, etc.)"; break;
    default:
        error = "unkown error occured";
        break;
    }

    return error;
}

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
}

void Downloader::slotFinished(QNetworkReply *reply)
{
    if(reply->error() != QNetworkReply::NoError){
        emit error(getStringError(reply->error()));
    } else {
        emit done(reply->url(), reply->readAll());
    }
    reply->deleteLater();
}
