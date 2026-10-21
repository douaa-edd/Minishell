                                                              
CC=gcc
FLAGS= -Wall -Werror -Wextra
AR=ar crs
RM=rm -rf
FILES= ft_isalpha ft_isdigit ft_isalnum ft_isascii ft_strlen \
	   ft_toupper ft_tolower ft_strchr ft_strrchr ft_strncmp \
	   ft_strnstr ft_strlcpy ft_strlcat ft_atoi ft_strdup ft_strndup \
	   ft_substr ft_strjoin ft_split ft_itoa ft_strmapi ft_strcmp \
	   ft_putchar_fd ft_putstr_fd ft_putendl_fd ft_isprint \
	   ft_putnbr_fd ft_strtrim ft_memcpy ft_memmove ft_memset \
	   ft_memcmp ft_memchr ft_bzero ft_calloc ft_striteri \
		ft_lstnew_bonus ft_lstadd_front_bonus \
	   ft_lstsize_bonus ft_lstlast_bonus \
	   ft_lstadd_back_bonus ft_lstdelone_bonus \
	   ft_lstclear_bonus ft_lstiter_bonus ft_lstmap_bonus
OBJ=$(FILES:=.o)

NAME=libft.a

.PHONY: all  clean fclean re

all: $(NAME)

$(NAME): $(OBJ)
	$(AR) $(NAME) $(OBJ)

%.o: %.c libft.h
	$(CC) $(FLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ) $(BOBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all
