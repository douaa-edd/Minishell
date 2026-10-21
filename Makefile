NAME = minishell


LIBFT_DIR = Libft
LIBFT = $(LIBFT_DIR)/libft.a


CC = gcc
CFLAGS = -Wall -Wextra -Werror -I. -I$(LIBFT_DIR)  #-fsanitize=address -g3 -fsanitize=leak


SRCS = main.c Parsing/tokenisation.c Parsing/tokenisation_utils.c Parsing/expand.c Parsing/expand_utils_1.c  Parsing/expand_utils_2.c \
		Parsing/env_init.c Parsing/syntax_error.c Parsing/parsing.c Parsing/parsing_utils.c Parsing/free_1.c Parsing/free_2.c Parsing/split_tokens.c\
		Execution/heredoc.c Execution/heredoc_utils_1.c Execution/heredoc_utils_2.c Execution/builtins.c Execution/red.c \
		Execution/single_cmd.c Execution/single_cmd_utils.c Execution/cd.c Execution/cd_utils.c Execution/echo.c Execution/utils.c \
		Execution/env.c Execution/exit.c Execution/export.c Execution/export_utils.c Execution/pwd.c Execution/unset.c Execution/shlvl.c \
		Execution/signals.c Execution/pipe.c Execution/pipe_utils.c Execution/handle_paths.c Execution/handle_paths_utils.c


OBJS = $(SRCS:.c=.o)


all: $(LIBFT) $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -lncurses -o $(NAME)

clean:
	rm -f $(OBJS)
	make clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re