# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/01/09 12:41:58 by cchameyr          #+#    #+#              #
#    Updated: 2018/04/06 13:34:48 by cchameyr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =			ft_nm

FILES =			main.c \
				check_security.c \
				nlist64_list.c \
				free_list.c \
				handle_magic64.c \
				print_magic64.c

SRCS =			$(addprefix srcs/, $(FILES))

OBJS =			$(addprefix objs/, $(FILES:.c=.o))

DEBUG = 		-g -fsanitize=address

FLAGS =			$(DEBUG)#-Wall -Werror -Wextra $(DEBUG) TODO UNCOMMENT HERE

RM =			rm -rf

CC =			gcc

LIBFT =			libft/libft.a

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(FLAGS) $(OBJS) $(LIBFT) -o $(NAME)

$(OBJS):
	$(CC) $(FLAGS) -c $(SRCS)
	@make objs_mv

objs_mv:
	@mkdir objs
	@mv $(FILES:.c=.o) ./objs

objs_rm:
	@$(RM) objs
	@$(RM) $(FILES:.c=.o)

$(LIBFT):
	make -C ./libft

clean: objs_rm
	make clean -C ./libft

fclean: clean
	$(RM) $(LIBFT) $(NAME)

re: fclean all

r: objs_rm
	$(RM) $(NAME)
	@make
