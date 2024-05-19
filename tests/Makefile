# Assumes you're in the test subfolder

CC = gcc   # This variable is which compiler to use, we will use the variable later by $(CC)
# set the path to the include directory
INCDIR =../include

CFLAGS = -Wall  # this variable is command line arguments

# path to the object file directory
ODIR = ../src/obj

# path to the lib directory
LIBDIR =../lib

# path to the bin directory
BINDIR =../bin

LIBS = -lm -limageIO
LFLAGS = -L$(LIBDIR) -L/opt/local/lib

# put all of the relevant include files here
_DEPS = ppmIO.h alphaMask.h Color.h FPixel.h Image.h

# convert them to point to the right place
DEPS = $(patsubst %,$(INCDIR)/%,$(_DEPS))

# put a list of the executables here
EXECUTABLES = ppmtest lab1 createMask composite imagetest

# put a list of all the object files here for all executables (with .o endings)
_OBJ = tiffmain.o ppmmain.o createMask.o compositeImage.o imagetest.o

# convert them to point to the right place
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

# patterns for compiling source code
# $< is the file that caused the action to occur
$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.C $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all:
	for i in $(EXECUTABLES); do (make $$i) ; done

TEST_FILE = tests.c
IMAGE_FILE = imagetest.c
OUTPUT = tests
 
# Detect the OS
ifeq ($(OS),Windows_NT)
	EXT = .exe
else
	EXT = .out
endif
#while linux/mac doesn't care about the extension, windows does, so we 
#want to check the OS and set the extension accordingly. Technically, we 
#could just set the extension to .exe and it would work on all OS's, but
#.out for (in progress) executables is more common on linux/mac, so we will use that.
#often a final release uses no extension (ls, cd, etc are all programs in linux/mac)


# gcc -Wall -O3 -o tests.out tests.c Image.c 

# put the specific executable rules here
imagetest: $(ODIR)/imagetest.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)


all: $(TEST_FILE)
	$(CC) $(CFLAGS) -O3 -o tests$(EXT) $(LIB_FILES) $^

 # this is the default target, it will run if you just type "make" in the terminal

image-test: $(IMAGE_FILE)
	$(CC) $(CFLAGS) -O3 -o imagetest$(EXT) $(LIB_FILES) $^


clean: #this is a clean target, it removes all the .exe/.out files, called via "make clean"
	rm  *$(EXT)