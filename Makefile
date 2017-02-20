# ==============================================================================
# BEGIN template
# ==============================================================================

# parameters
EXE     = metallicar
LIBS    = 
INC     = -I/usr/include/SDL2

CXX     = g++ -g -std=c++0x
SRCS    = $(shell find src -name '*.cpp')
HEADERS = $(shell find src -name '*.hpp')
OBJS    = $(addprefix obj/,$(notdir $(SRCS:%.cpp=%.o)))

$(EXE): $(OBJS)
	$(CXX) $(LIBS) $(OBJS) -o $@

obj/%.o: src/%.cpp $(HEADERS)
	mkdir -p obj
	$(CXX) -c $(INC) $< -o $@

.PHONY: clean

clean:
	rm -rf $(EXE) obj

# ==============================================================================
# END template
# ==============================================================================
