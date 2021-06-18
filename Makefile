SRC_DIR := ./src
OBJ_DIR := ./obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
SFML := C:/SFML-2.5.1/include
SFML_LIB_PATH := C:/SFML-2.5.1/lib
SFML_LIBS := -lsfml-graphics -lsfml-window -lsfml-system 
CXXFLAGS := -Wall -std=c++11

gbc: $(OBJ_FILES)
	g++ -O2 -o $@ $^ -L$(SFML_LIB_PATH) -I$(SFML) $(SFML_LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	   g++ -O2 $(CXXFLAGS) -c $< -o $@ -L$(SFML_LIB_PATH) -I$(SFML) $(SFML_LIBS)

clean:
	rm -f obj/*.o
	rm -f gbc*