NAME = webserv

-include sabra.mak

SRCS = $(SABRA)\
         ParseRequest/srcs/parseRequest.cpp\
         ParseRequest/response/response.cpp\
         ATransmitterClass.cpp \
         ConnectionClass.cpp \
         ReadingTransmitterClass.cpp \
         WritingTransmitterClass.cpp \
         main.cpp \

OBJS = $(SRCS:.cpp=.o)

CC 		= clang++
RM		= rm -rf
CFLAGS	= -Wall -Werror -Wextra -g -std=c++98 
INCLUDES = -I config_parse/includes -I cgi/includes -I parseRequest/Includes 

all:
	@$(MAKE) $(NAME) -j4

%.o:    %.cpp
	@$(CC) $(CFLAGS) $(INCLUDES)  -c $< -o $@

$(NAME): $(OBJS)
	@$(CC) $(OBJS) -o $(NAME)
	@echo $(NAME) compiled!

clean:
	@$(RM) $(OBJS) *.cache
	@echo clean .o files

fclean: clean
	@$(RM) $(NAME)
	@echo $(NAME) removed!

re: fclean $(NAME)