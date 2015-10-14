#include "logger.h"
#include <QFileDialog>
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <cassert>
#include <QDateTime>

Logger::Logger( mode::ENUM m, QObject* parrent ) :
    QObject(parrent)
{
    assert( (m == mode::append || m == mode::clear) && "Unknown mode");
    QString currDir = QDir::currentPath();
    bool isFileSelected = false;
    while ( !isFileSelected ) {
        do {
            logFileName_ =  QFileDialog::getOpenFileName(0, "Select the log file", currDir);
        }
        while( logFileName_.isEmpty() );

        log_.setFileName(logFileName_);
        QIODevice::OpenMode openMode;
        openMode = (m == mode::append) ? QIODevice::Append : QIODevice::Truncate;
        log_.open(QIODevice::WriteOnly|openMode);
        if ( !log_.isWritable() ) {
            QMessageBox::critical(0, "Error",
                                  "Can not write to the file, please chose another file"
                                  );
            log_.close();
        } else {
            isFileSelected = true;
        }
    } //end while (!isFileSelected)
    markStartMessage();
}

Logger::~Logger()
{
    markEndMessage();
}

Logger& Logger::operator<<(const QString &logThis)
{
    QDateTime now = QDateTime::currentDateTime();
    QString date = now.toString() + " ";
    //qDebug() << date;
    log_.write(date.toStdString().c_str(), date.toStdString().size());

    log_.write(logThis.toStdString().c_str(), logThis.toStdString().size());
    addNextLine();

    return *this;
}

void Logger::markStartMessage()
{
    QString message = " Writing started";

    (*this) << message;
    addNextLine();
}

void Logger::markEndMessage()
{
    QString message = " Writing finished";

    (*this) << message;
    addNextLine();
    log_.flush();
}

void Logger::addNextLine()
{
#if defined(Q_OS_WIN) || defined(Q_OS_WIN32) || defined(Q_OS_OS2)
    log_.write("\r\n", 2);
#elif defined Q_OS_MAC
   log_.write("\n", 1);
#endif

}

void Logger::slotLog(const QUrl& url,const QByteArray& ba)
{
    slotLog(url);
    (*this) << QString(ba);
}

void Logger::slotLog(const QUrl& url)
{
    (*this) << url.toString();
}
