# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/19 11:56:22 by tuthayak          #+#    #+#              #
#    Updated: 2025/03/19 11:56:22 by tuthayak         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# === Compiler and Flags ===
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -pthread

# === Project Name ===
NAME		= philo

# === Directories ===
OBJDIR		= obj

# === Source Files ===
SRCS		=	main.c \
				init.c \
				philosopher.c \
				simulation.c \
				parsing.c \
				utils.c \
				cleanup.c

# === Object Files ===
OBJS		= $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))

# === Includes ===
INCDIR		= .
INCLUDES	= -I$(INCDIR)

# === Makefile Rules ===

# Default rule: Build executable
all: $(NAME)

# Compile the executable
$(NAME): $(OBJDIR) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "\033[32m[✔] Compilation finished!\033[0m"

# Compile object files
$(OBJDIR)/%.o: %.c philo.h
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@echo "\033[34m[✔] Compiling:\033[0m $<"

# Create object directory
$(OBJDIR):
	@mkdir -p $(OBJDIR)

# Clean object files
clean:
	@rm -rf $(OBJDIR)
	@echo "\033[33m[✔] Object files removed.\033[0m"

# Full clean (objects + binary)
fclean: clean
	@rm -f $(NAME)
	@echo "\033[31m[✔] Executable removed.\033[0m"

# Recompile everything
re: fclean all

# Phony targets
.PHONY: all clean fclean re
