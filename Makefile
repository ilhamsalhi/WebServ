#================================= Files to compile ===================================
SRC_FILES	= main Client Server ConfigParser ServerParser LocationParser Multiplexer  Exception RequestParser \
 			  Response ResponseHandlers ResponseGenerators postResponse postResponseUtils chunkedRequest
LIB_FILES 	= signal split trim  addr_resolver ft_stoll ft_stoi socket_addr_ntop epoll_add \
			  getFileSize	is_directory log
INC_FILES	= Client define Multiplexer Server ConfigParser \
			  Exception Response webserv
# #======================================================================================

# #====================================== Colors ========================================
NO_COLOR	= \033[0m
GRAY		= \033[0;1;3;90m
RED			= \033[0;1;3;91m
GREEN		= \033[0;1;3;92m
GREEN_L		= \033[0;1;3;36m
YELLOW		= \033[0;1;3;33m
BLUE		= \033[0;1;3;34m
# #======================================================================================

# #=================================files directories ===================================
SRC_DIR		= src/
LIB_DIR		= lib/
OBJ_DIR		= objs/
INC_DIR		= include/
# #======================================================================================


# #===================================== Standard =======================================
NAME		= webserv

AUTHOR		= BELKARTO && ISALHI

CC			= c++

OBJF		= .cache_exists

FLAGS		=  -Wall -Wextra -Werror -std=c++98 -Iinclude -g -fsanitize=address

# #======================================================================================


# # ===Better not to touch ===#
SRC 		= 	$(addprefix $(SRC_DIR), $(addsuffix .cpp, $(SRC_FILES)))
S_LIB 		= 	$(addprefix $(LIB_DIR), $(addsuffix .cpp, $(LIB_FILES)))
OBJ 		= 	$(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_FILES)))
OBJ_LIB 	= 	$(addprefix $(OBJ_DIR), $(addsuffix .o, $(LIB_FILES)))
INCLUDES	=	$(addprefix $(INC_DIR), $(addsuffix .hpp, $(INC_FILES)))
# #===========================#

all : header $(NAME)

# # == Rule that compile source files into object files ==
$(OBJ_DIR)%.o	: $(SRC_DIR)%.cpp $(INCLUDES) | $(OBJF)
	@$(CC) $(FLAGS) -c $< -o $@
	@printf "$(GRAY)\r- Compiling src...⌛$(NO_COLOR)"
	@sleep 0.01
	@printf "$(GRAY)\r- Compiling src...⏳$(NO_COLOR)"
	@sleep 0.01
$(OBJ_DIR)%.o	: $(LIB_DIR)%.cpp $(INCLUDES) | $(OBJF)
	@$(CC) $(FLAGS) -c $< -o $@
	@printf "$(GRAY)\r- Compiling lib...⌛$(NO_COLOR)"
	@sleep 0.01
	@printf "$(GRAY)\r- Compiling lib...⏳$(NO_COLOR)"
	@sleep 0.01
# #=======================================================

# #=== rule that compile the final program ===
$(NAME) : $(OBJ) $(OBJ_LIB)
	@$(CC) $(FLAGS) $(OBJ) $(OBJ_LIB) -o $(NAME)
	@printf "$(GREEN)\n- Finished ✅🥳\n$(NO_COLOR)"
# #===========================================

# #================================= keepalive rule ===================================
keepalive: FLAGS += -DKEEPALIVE
keepalive: clean all
# #======================================================================================

# #=============================================================================#

# #== rule that called if object folder doesn't exist ==
$(OBJF):
	@mkdir -p $(OBJ_DIR)
# #=====================================================

## # == rule deleting compiled files : the cache folder ==
clean : header
	@rm -rf $(OBJ_DIR)
	@printf "$(BLUE)clean\t:\t\t\t$(GREEN)[✓]$(NO_COLOR)\n"
# # =====================================================

# # == Rule calling clean and deleting the final file ==
fclean	: header clean
	@rm -f $(NAME)
	@printf "$(BLUE)fclean\t:\t\t\t$(GREEN)[✓]$(NO_COLOR)\n\n"
# # ====================================================

# # == HEADER DISPLAYS AUTHOR'S NAME AND COMILER AND FLAGS BEEN USED AND ===
header:
	@echo
	@printf  "$(GREEN_L)Author\t: $(BLUE)$(AUTHOR)\n"
	@printf  "$(GREEN_L)CC    \t: $(YELLOW)$(CC)\n\033[m"
	@printf  "$(GREEN_L)Flags \t: $(YELLOW)$(FLAGS)\n\033[m"
	@echo
# # ========================================================================
re		:	fclean all
