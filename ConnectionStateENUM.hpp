#ifndef CONNECTION_STATE_ENUM_HPP
# define CONNECTION_STATE_ENUM_HPP

enum ConnectionState {
    IS_PROCESSING_FIRST_LINE = 0,
    IS_PROCESSING_HEADERS = 1,
    IS_READING_BODY = 2,
    IS_FORMING_RESPONSE = 3,
    IS_WRITING_RESPONSE = 4,
    ERROR_WHILE_READING = 5,
    CLOSE = 6
};

#endif