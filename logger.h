#ifndef LOGGER_H
#define LOGGER_H

#include <QFile>
//#include

class Logger
{
private:
    QString logFileName_;
    QFile   log_;
public:
    struct mode {
        enum ENUM { clear, append };
    };
    explicit Logger( mode::ENUM );
    ~Logger();

    Logger &operator<<(const QString& logThis);
private:
    Logger(const Logger& orig) = delete;
    Logger& operator=(const Logger& rhs) = delete;
    void markStartMessage();
    void markEndMessage();

    void addNextLine();
};

#endif // LOGGER_H
