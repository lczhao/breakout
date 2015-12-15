# super simple makefile
# call it using 'make NAME=name_of_code_file_without_extension'
# (assumes a .cpp extension)
EXEC = breakout
OBJECTS = Brick.o Padel.o Ball.o breakout.o game.o
SOURCE = Brick.cc Padel.cc Ball.cc breakout.cc game.cc
CXX = g++
CXXFLAGS = -L/usr/X11R6/lib -lX11 -lstdc++
#
# Add $(MAC_OPT) to the compile line for Mac OSX.
MAC_OPT = "-I/opt/X11/include"

all:
	@echo "Compiling..."
	$(CXX) -c $(CXXFLAGS) $(SOURCE); $(CXX) $(OBJECTS) -o $(EXEC) $(CXXFLAGS)
	   
run: all
	@echo "Running..."
	./$(EXEC) 

clean:
	-rm a1
