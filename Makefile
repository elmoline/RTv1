# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wael-mos <wael-mos@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/07 16:47:44 by evogel            #+#    #+#              #
#    Updated: 2019/11/05 15:05:53 by evogel           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = RTv1

#########
# FILES #
#########

SRCS = main.c \
	   parsing1.c \
	   parsing2.c \
	   parsing3.c \
	   render.c \
	   cast_ray.c \
	   intersect1.c \
	   intersect2.c \
	   tools1.c \
	   tools2.c \
	   tools3.c \
	   tools4.c \
	   tools_mlx.c

OBJS = $(SRCS:.c=.o)

###############
# DIRECTORIES #
###############

INC = incs

P_SRCS = srcs

P_OBJS = objs

D_OBJS = $(addprefix $(P_OBJS)/, $(OBJS))

#############
# LIBRARIES #
#############

LIBS = libft.a \
	   libmlx.a

D_LIBS = $(foreach LIB, $(LIBS), $(basename $(LIB))/$(LIB))

############
# COMPILER #
############

CC = gcc

CFLAGS = -g -Wall -Wextra -Werror

MLXFLAGS = -framework OpenGL -framework AppKit

I_FLAGS = $(foreach LIB, $(LIBS),-I$(basename $(LIB))) -I$(INC)

L_FLAGS = $(foreach LIB, $(LIBS),-L$(basename $(LIB)) -l$(LIB:lib%.a=%)) -lm

################
# COMPILE TEST #
################

COM_COLOR   = \033[0;34m
OBJ_COLOR   = \033[0;36m
OK_COLOR    = \033[0;32m
ERROR_COLOR = \033[0;31m
WARN_COLOR  = \033[0;33m
NO_COLOR    = \033[m

define run_and_test
printf "%b" "$(COM_COLOR)$(COM_STRING) $(OBJ_COLOR)$(@F)$(NO_COLOR)\r"; \
	$(1) 2> $@.log; \
	RESULT=$$?; \
	if [ $$RESULT -ne 0 ]; then \
		printf "%-60b%b" "$(COM_COLOR)$(COM_STRING)$(OBJ_COLOR) $@" "$(ERROR_COLOR)$(ERROR_STRING)$(NO_COLOR)\n"   ; \
	elif [ -s $@.log ]; then \
		printf "%-60b%b" "$(COM_COLOR)$(COM_STRING)$(OBJ_COLOR) $@" "$(WARN_COLOR)$(WARN_STRING)$(NO_COLOR)\n"   ; \
	else  \
		printf "%-60b%b" "$(COM_COLOR)$(COM_STRING)$(OBJ_COLOR) $(@F)" "$(OK_COLOR)$(OK_STRING)$(NO_COLOR)\n"   ; \
	fi; \
	cat $@.log; \
	rm -f $@.log; \
	exit $$RESULT
endef

OK_STRING    = "[OK]"
ERROR_STRING = "[ERROR]"
WARN_STRING  = "[WARNING]"
COM_STRING   = "Compiling"

#########
# RULES #
#########

all: $(NAME)

$(NAME): $(D_LIBS) $(P_OBJS) $(D_OBJS)
	@$(call run_and_test, $(CC) $(CFLAGS) $(MLXFLAGS) -o $(NAME) $(D_OBJS) $(L_FLAGS))

$(P_OBJS)/%.o: $(P_SRCS)/%.c $(INC)
	@$(call run_and_test, $(CC) $(CFLAGS) $(I_FLAGS) -o $@ -c $<)

$(P_OBJS):
	@$(call run_and_test, mkdir -p $(P_OBJS))

$(D_LIBS):
	@$(call run_and_test, make -C $(dir $@))

clean:
	@$(call run_and_test, rm -rf $(P_OBJS) $(foreach LIB, $(LIBS),&& make -C $(basename $(LIB)) clean))

fclean: clean
	@$(call run_and_test, rm -f $(NAME) $(foreach LIB, $(LIBS),&& make -C $(basename $(LIB)) fclean))

re: fclean all
