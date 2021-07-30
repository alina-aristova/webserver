#ifndef READINGTRANSMITTER_CLASS_HPP
# define READINGTRANSMITTER_CLASS_HPP

# include "ATransmitterClass.hpp"
# include "ReadingBodyFunctionENUM.hpp"
# include "config_parse/includes/Server.hpp"
# include "cgi/includes/cgi.hpp"

class ReadingTransmitterClass : public ATransmitterClass {
private:
    int _contentLength;
    ReadingBodyFunction _readingBodyFunType;
    std::string _host;
    std::string _readingBuffer;
    std::string _bufferToBeProcessed;
    std::map<std::string, Server *> _availableHosts;
    // ---------------------------- основные вспомогательные функции ------------------------------------------- //
    void _readIntoBuffer();
    void _processingFirstLine();
    void _processingHeaders();
    void _readingBody();
    // ----------------------------- различные способы чтения ------------------------------------------ //
    void _classicReading();
    void _contentLengthReading();
    void _chunkedEncodingReading(int & unchunkedPartLength);
    // ----------------------------- поиск правильного хоста -------------------------------------------- //
    Server   *findRightHost();
    // ----------------------------- возвращаем дефолтные значния полей ----------------------------------------- //
    void    returnDefaultValues();
public:
    char **env;
    ReadingTransmitterClass(int socket, std::string & responseStatus, ConnectionState & connectionState,
        std::string & writingBuffer, bool & closeConnection, std::map<std::string, Server *> availableHosts);
    void operate();
    virtual ~ReadingTransmitterClass();
    const std::string & getReadingBuffer() const;
};

#endif