CXX       = g++
CFLAGS    = -I/usr/include/botan-1.10
LIBS      = -lbotan-1.10

all: 
	$(CXX) USER99D59.cc $(CFLAGS) $(LIBS) -o USER99D59.exe
	$(CXX) adduser.cc $(CFLAGS) $(LIBS) -o adduser.exe
