# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/09/05 16:30:43 by cchameyr          #+#    #+#              #
#*   Updated: 2016/09/19 17:50:06 by                  ###   ########.fr       *#
#                                                                              #
# **************************************************************************** #

FILES_NM =				main.c \
						check_security.c \
						nlist64_list.c \
						nlist32_list.c \
						free_list.c \
						handle_magic64.c \
						print_magic64.c \
						handle_magic32.c \
						print_magic32.c \
						handle_fat32.c \
						handle_fat64.c \
						handle_ar.c \
						tools.c

FILES_OTOOL =			main.c

SRCS_NM =				$(addprefix srcs_nm/, $(FILES_NM))

SRCS_OTOOL =			$(addprefix srcs_otool/, $(FILES_OTOOL))

OBJS_NM =				$(addprefix obj_nm/, $(FILES_NM:.c=.o))

OBJS_OTOOL =			$(addprefix obj_otool/, $(FILES_OTOOL:.c=.o))

CC =					gcc

NAME_NM =				ft_nm

NAME_OTOOL =			ft_otool

LIBFT =					libft/libft.a

DEBUGSEGFAULT =			-fsanitize=address

FLAGS =					$(DEBUGSEGFAULT) #-Wall -Wextra -Werror #$(DEBUGSEGFAULT)

RM =					rm -rf

all: $(NAME_OTOOL) $(NAME_NM)

$(NAME_OTOOL): $(LIBFT) $(OBJS_OTOOL)
	$(CC) $(FLAGS) $(OBJS_OTOOL) $(LIBFT) -o $(NAME_OTOOL)

$(OBJS_OTOOL):
	$(CC) $(FLAGS) -c $(SRCS_OTOOL)
	@make obj_otool_mv

obj_otool_mv:
	@mkdir obj_otool
	@mv $(FILES_OTOOL:.c=.o) ./obj_otool/

obj_otool_rm:
	@$(RM) ./obj_otool
	@$(RM) $(FILES_OTOOL:.c=.o)


$(NAME_NM): $(LIBFT) $(OBJS_NM)
	$(CC) $(FLAGS) $(OBJS_NM) $(LIBFT) -o $(NAME_NM)

$(OBJS_NM):
	$(CC) $(FLAGS) -c $(SRCS_NM)
	@make obj_nm_mv

obj_nm_mv:
	@mkdir obj_nm
	@mv $(FILES_NM:.c=.o) ./obj_nm/

obj_nm_rm:
	@$(RM) ./obj_nm
	@$(RM) $(FILES_NM:.c=.o)


$(LIBFT):
	make -C ./libft/

clean: obj_nm_rm obj_otool_rm
	make clean -C ./libft/

fclean: clean
	$(RM) $(LIBFT) $(NAME_NM) $(NAME_OTOOL)

re: fclean all

r: obj_nm_rm obj_otool_rm
	$(RM) $(NAME_NM) $(NAME_OTOOL)
	@make
