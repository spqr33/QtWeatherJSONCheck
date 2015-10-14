#ifndef LOGGER_H
#define LOGGER_H

#include <QFile>
#include <QObject>

class Logger : public QObject
{
    Q_OBJECT
private:
    QString logFileName_;
    QFile   log_;
public:
    struct mode {
        enum ENUM { clear, append };
    };
    explicit Logger( mode::ENUM, QObject* parrent = 0 );
    ~Logger();

    Logger &operator<<(const QString& logThis);
private:
    Logger(const Logger& orig) = delete;
    Logger& operator=(const Logger& rhs) = delete;
    void markStartMessage();
    void markEndMessage();

    void addNextLine();
public slots:
    void slotLog(const QUrl&,const QByteArray&);
    void slotLog(const QUrl&);
};

#endif // LOGGER_H
