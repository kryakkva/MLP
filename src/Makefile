NAME			= MLP
SRCS_D			= sources/
SRCS_H			= headers/


SRCS 			= $(addprefix $(SRCS_D), ../main.cpp Function.cpp Matrix.cpp Edge.cpp Layer.cpp Neuron.cpp NeuralNetwork.cpp)
SRC_TEST  		= tests/tests.cpp
SRCS_TEST		= $(addprefix $(SRCS_D), Function.cpp Matrix.cpp Edge.cpp Layer.cpp Neuron.cpp NeuralNetwork.cpp)
#SRCS			= $(addprefix $(SRCS_D),Matrix.cpp Function.cpp NeuralNetwork.cpp)
HEADER			= $(addprefix $(SRCS_H), Function.h Matrix.h Edge.h Layer.h Neuron.h NeuralNetwork.h)
#HEADER			= $(addprefix $(SRCS_D), Matrix.h Function.h NeuralNetwork.h)
OBJS_D			= objs/
OBJS_S			= objs/sources/
#OBJS_T			= objs/tests/

OBJS			= $(SRCS:.cpp=.o)
#OBJT			= $(SRC_TEST:.cpp=.o)
OBJ_S			= $(addprefix $(OBJS_D), $(OBJS))
#OBJ_T			= $(addprefix $(OBJS_D), $(OBJT))

CC				= c++
RM				= rm -rf
#FL				= -std=c++17
FL				= -Wshadow -Wall -Wextra -Werror -std=c++17
TEST_FLAG =-Wall -Werror -Wextra -std=c++17 -lgtest -lstdc++


all:			$(OBJS_S) $(NAME)

$(OBJS_S):
	mkdir -p $(OBJS_S) $(OBJS_T)

$(OBJS_D)%.o: %.cpp $(HEADER)
	$(CC) $(FL) -c -o $@ $< 

$(NAME):		$(OBJ_S)
				$(CC) $(FL) -o $(NAME) $(OBJ_S)

clean:
				$(RM) $(OBJ_D) $(OBJS_D)

fclean:			clean
				$(RM) $(NAME) g_tests

re:				fclean all

test:
	@$(CC) $(TEST_FLAG) $(SRC_TEST) $(SRCS_TEST) -o g_tests
	./g_tests


format:
				clang-format -i -style=google sources/*.cpp headers/*.h
				clang-format -n -style=google sources/*.cpp headers/*.h

.PHONY:			all clean fclean re