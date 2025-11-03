# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/27 15:10:54 by mring             #+#    #+#              #
#    Updated: 2025/11/03 14:59:26 by jpflegha         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = miniRT
CC = cc
CFLAGS = -Wall -Werror -Wextra -lm
DEBUG_FLAGS = -fsanitize=address -g3
INCLUDES = -Iinclude -I$(LIBFT_DIR)

# Colors for output
RED			= \033[0;31m
GREEN		= \033[0;32m
YELLOW		= \033[0;33m
BLUE		= \033[0;34m
MAGENTA		= \033[0;35m
CYAN		= \033[0;36m
WHITE		= \033[0;37m
RESET		= \033[0m
BOLD		= \033[1m

SRC_DIR = src
OBJ_DIR = obj
LIBFT_DIR = libft

TARGET = $(NAME)

SRC_FILES = main.c 

SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))

OBJ = $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SRC:.c=.o))

HEADERS = include/miniRT.h

LIBFT = $(LIBFT_DIR)/libft.a
LIBS = -lmath

# ─────────────────────────────────────────────── #
#                 PURPLE GRADIENT BANNER           #
# ─────────────────────────────────────────────── #

define BANNER
\033[38;2;170;0;255m ██████   ██████ █████ ██████   █████ █████    ███████████   ███████████\033[0m
\033[38;2;155;0;255m▒▒██████ ██████ ▒▒███ ▒▒██████ ▒▒███ ▒▒███    ▒▒███▒▒▒▒▒███ ▒█▒▒▒███▒▒▒█\033[0m
\033[38;2;140;0;255m ▒███▒█████▒███  ▒███  ▒███▒███ ▒███  ▒███     ▒███    ▒███ ▒   ▒███  ▒ \033[0m
\033[38;2;125;0;255m ▒███▒▒███ ▒███  ▒███  ▒███▒▒███▒███  ▒███     ▒██████████      ▒███   \033[0m
\033[38;2;110;0;255m ▒███ ▒▒▒  ▒███  ▒███  ▒███ ▒▒██████  ▒███     ▒███▒▒▒▒▒███     ▒███   \033[0m
\033[38;2;95;0;255m ▒███      ▒███  ▒███  ▒███  ▒▒█████  ▒███     ▒███    ▒███     ▒███   \033[0m
\033[38;2;80;0;255m █████     █████ █████ █████ ▒▒█████  █████    █████   █████    █████  \033[0m
\033[38;2;65;0;255m▒▒▒▒▒     ▒▒▒▒▒ ▒▒▒▒▒ ▒▒▒▒▒    ▒▒▒▒▒ ▒▒▒▒▒    ▒▒▒▒▒   ▒▒▒▒▒    ▒▒▒▒▒   \033[0m
endef
export BANNER

# ─────────────────────────────────────────────── #
#                     TARGETS                     #
# ─────────────────────────────────────────────── #

all: banner $(LIBFT) $(TARGET)

banner:
	@printf "\n$$BANNER\n\n"

debug: CFLAGS += $(DEBUG_FLAGS)
debug: fclean all

noflags: CFLAGSL =
noflags: fclean all

$(LIBFT):
	@rm -rf libft/.git
	@printf "$(BLUE)Compiling $(YELLOW)$<$(RESET)... "
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@printf "$(GREEN)✓$(RESET)\n"

$(TARGET): $(OBJ) $(LIBFT)
	@printf "$(MAGENTA)Linking $(NAME)...$(RESET)\n"
	@$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIBFT) $(LIBS)
	@printf "$(GREEN)$(BOLD)✓ $(NAME) built successfully!$(RESET)\n"

valgrind: $(TARGET)
	@printf "$(CYAN)Running $(NAME) with valgrind...$(RESET)\n"
	@valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --suppressions=sub.sub ./$(TARGET)

clean:
	@printf "$(YELLOW)Cleaning object files...$(RESET)\n"
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean --no-print-directory
	@printf "$(GREEN)Object files cleaned!$(RESET)\n"

fclean:
	@printf "$(YELLOW)Cleaning everything...$(RESET)\n"
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) fclean --no-print-directory
	@printf "$(GREEN)Everything cleaned!$(RESET)\n"

re: fclean all

.PHONY: all debug clean fclean re valgrind noflags banner
