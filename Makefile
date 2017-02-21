# ==============================================================================
# BEGIN template
# ==============================================================================

# parameters
EXE     = metallicar
LIBS    = -lSDL2 -lSDL2_ttf -lSDL2_net -lGL -lopenal
INC     = -I/usr/include/SDL2 -I/usr/include/AL

CXX     = g++ -g -std=c++0x
SRCS    = $(shell find src -name '*.cpp')
HEADERS = $(shell find src -name '*.hpp')
OBJS    = $(addprefix obj/,$(notdir $(SRCS:%.cpp=%.o)))

$(EXE): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LIBS)

obj/%.o: src/%.cpp $(HEADERS)
	mkdir -p obj
	$(CXX) -c $(INC) $< -o $@

.PHONY: clean

clean:
	rm -rf $(EXE) obj

# ==============================================================================
# END template
# ==============================================================================
