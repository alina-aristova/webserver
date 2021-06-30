NAME 	= parser

-include sabra.mak
SRCS	= $(SABRA)\
ParseRequest/srcs/ParseRequest.cpp\
ParseRequest/response/response.cpp\
main.cpp

OBJS 	= $(SRCS:.cpp=.o)

CC 		= clang++
RM		= rm -rf
CFLAGS	= -Wall -Werror -Wextra -g -std=c++98 -pedantic
INCLUDES = -I config_parse/includes 

all:
	@$(MAKE) $(NAME) -j4

%.o:    %.cpp
	@$(CC) $(CFLAGS) 



clean:
	@$(RM) $(OBJS)
	@echo clean .o files

fclean: clean
	@$(RM) $(NAME)
	@echo $(NAME) removed!

re:	fclean $(NAME)