NAME 	= webserv

-include sabra.mak

SRCS	= $(SABRA)\
			ParseRequest/srcs/parseRequest.cpp\
			ParseRequest/response/response.cpp\
			main_reserv.cpp

OBJS 	= $(SRCS:.cpp=.o)

CC 		= clang++
RM		= rm -rf
CFLAGS	= -Wall -Werror -Wextra -g -std=c++98 -pedantic
INCLUDES = -I config_parse/includes 

all:
	@$(MAKE) $(NAME) -j4

%.o:    %.cpp
	@$(CC) $(CFLAGS)  -c $< -o $@

$(NAME):	$(OBJS)
		@$(CC) $(OBJS) -o $(NAME)
		@echo $(NAME) compiled!

clean:
	@$(RM) $(OBJS)
	@echo clean .o files

fclean: clean
	@$(RM) $(NAME)
	@echo $(NAME) removed!

re:	fclean $(NAME)