# set the C compiler, for C++ use g++

# use this setting on most sane machines
CC = gcc

# use this setting on the engin elf machines
#CC = gcc296

# set the path to the include directory
INCDIR =../include

# set the flags for the C and C++ compiler to give lots of warnings
CFLAGS = -I$(INCDIR) -I/opt/local/include -O2 -Wall -Wstrict-prototypes -Wnested-externs -Wmissing-prototypes -Wmissing-declarations
CPPFLAGS = $(CFLAGS)

# library tool defs
AR = ar
ARFLAGS = rv
RANLIB = ranlib

# path to the object file directory
ODIR = obj

# path to the lib directory
LIBDIR =.

# path to the bin directory
BINDIR =../bin

# put all of the relevant include files here
_DEPS = ppmIO.h alphaMask.h Bezier.h Color.h Image.h FPixel.h Fractals.h Noise.h Point.h Lighting.h Scanline.h Line.h Circle.h Ellipse.h Polyline.h Polygon.h Graphics.h list.h Vector.h Matrix.h View2D.h View3D.h DrawState.h Module.h plyRead.h RayTracer.h

# convert them to point to the right place
DEPS = $(patsubst %,$(INCDIR)/%,$(_DEPS))

# put a list of all the object files (with .o endings)
_COMMON = ppmIO.o alphaMask.o Bezier.o Color.o Image.o Fractals.o Noise.o Point.o Line.o Lighting.o Circle.o Ellipse.o Polyline.o Polygon.o list.o Scanline.o Vector.o Matrix.o View2D.o View3D.o DrawState.o Module.o plyRead.o RayTracer.o

# convert them to point to the right place
COMMON = $(patsubst %,$(ODIR)/%,$(_COMMON))


# patterns for compiling source code
$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.C $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# output library
OUTPUT_LIB = libimageIO.a

# make rules
all: $(OUTPUT_LIB)

$(OUTPUT_LIB) : $(COMMON) $(DEPS)
	rm -f $@
	$(AR) $(ARFLAGS) $@ $(COMMON)
	$(RANLIB) $@

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

