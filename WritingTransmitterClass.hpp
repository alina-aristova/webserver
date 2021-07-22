#ifndef WRITINGTRANSMITTER_CLASS_HPP
# define WRITINGTRANSMITTER_CLASS_HPP

# include "ATransmitterClass.hpp"
# include "unistd.h"

class WritingTransmitterClass : public ATransmitterClass {
public:
    WritingTransmitterClass(int socket, std::string & responseStatus, ConnectionState & connectionState,
                            std::string & writingBuffer, bool & closeConnection);
    virtual ~WritingTransmitterClass();
    void operate();
};

#endif