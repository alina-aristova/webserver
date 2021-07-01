#ifndef READINGTRANSMITTER_CLASS_HPP
# define READINGTRANSMITTER_CLASS_HPP

# include "ATransmitterClass.hpp"
# include "ReadingBodyFunctionENUM.hpp"

class ReadingTransmitterClass : public ATransmitterClass {
private:
    int _contentLength;
    ReadingBodyFunction _readingBodyFunType;
    std::string _host;
    std::string _readingBuffer;
    std::string _bufferToBeProcessed;
    // ---------------------------- основные вспомогательные функции ------------------------------------------- //
    void _readIntoBuffer();
    void _processingFirstLine();
    void _processingHeaders();
    void _readingBody();
    // ----------------------------- различные способы чтения ------------------------------------------ //
    void _classicReading();
    void _contentLengthReading();
    void _chunkedEncodingReading(int & unchunkedPartLength);
public:
    ReadingTransmitterClass(int socket, std::string & responseStatus, ConnectionState & connectionState, std::string & writingBuffer, bool & closeConnection);
    void operate();
    virtual ~ReadingTransmitterClass();
    const std::string & getReadingBuffer() const;
};

#endif