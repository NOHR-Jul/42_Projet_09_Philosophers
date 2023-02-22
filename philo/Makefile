# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: juchene <juchene@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/08 15:49:00 by juchene           #+#    #+#              #
#    Updated: 2023/02/15 22:15:51 by juchene          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

DIR_HEADER		=	inc/

OBJ_DIR			=	obj/

DEP_DIR			=	obj/

SRC_DIR			=	src/

SRC				=	philo.c ph_utils.c ph_init.c ph_free.c ph_meal.c \
					ph_routine.c ph_process.c 

OBJ				=	$(SRC:.c=.o)

OBJS			=	$(addprefix $(OBJ_DIR), $(OBJ))

DEP				=	$(SRC:.c=.d)

DEPS			=	$(addprefix $(DEP_DIR), $(DEP))

NAME			=	philo

NAME_SAN		=	philo_san

CC				=	gcc

RM				=	rm -f

RMDIR			=	rm -rf

MKDIR			=	mkdir -p

CFLAGS			=	-MMD -Wall -Werror -Wextra -g

CFLAGSAN		=	-MMD -Wall -Werror -Wextra -g3 -fsanitize=thread

$(NAME)			:	$(OBJS)
					@$(CC) $(CFLAGS) $(OBJS) -o $@ -I $(DIR_HEADER)

$(NAME_SAN)		:	$(OBJS)
					@$(CC) $(CFLAGSAN) $(OBJS) -o $@ -I $(DIR_HEADER)
					
all				:	$(NAME)

san				:	$(NAME_SAN)

$(OBJ_DIR)%.o	:	$(SRC_DIR)%.c
					@$(MKDIR) $(OBJ_DIR)
					$(CC) $(CFLAGS) -c $< -o $@ -I $(DIR_HEADER)

clean			:
					@$(RM) $(OBJS) $(DEPS)
					$(RMDIR) $(OBJ_DIR)

fclean			:	clean
					@$(RM) $(NAME)
					@$(RM) $(NAME_SAN)

re				:	fclean
					make all

-include $(DEPS)

.PHONY			:	all, san, clean, fclean, re