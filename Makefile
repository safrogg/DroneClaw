OS := $(shell uname)
ARCH := $(shell uname -m)

ifeq ($(OS), Linux)
  ifeq ($(ARCH), x86_64)
    LEAP_LIBRARY := ../lib/x64/libLeap.so -Wl,-rpath,../lib/x64
  else
    LEAP_LIBRARY := ../lib/x86/libLeap.so -Wl,-rpath,../lib/x86
  endif
else
  # OS X
  LEAP_LIBRARY := ./libs/LeapLibs/libLeap.dylib
endif

DroneClaw : ./src/LeapController/DroneClaw.cpp
	$(CXX) -Wall -g -I ./libs/LeapLibs/includes ./src/LeapController/DroneClaw.cpp -o ./out/DroneClaw $(LEAP_LIBRARY)
ifeq ($(OS), Darwin)
	install_name_tool -change @loader_path/libLeap.dylib ./libs/LeapLibs/libLeap.dylib ./out/DroneClaw
endif

clean:
	rm -rf ./out/*

