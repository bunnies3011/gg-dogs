# ==== paths & tools ====
SDL3_ROOT       := D:/SDL3-3.2.24/x86_64-w64-mingw32
SDL3_IMAGE_ROOT := C:/msys64/ucrt64

CXX       := C:/msys64/ucrt64/bin/g++.exe
CXXFLAGS  := -std=c++17 -I"$(SDL3_ROOT)/include" -I"$(SDL3_IMAGE_ROOT)/include"
LDFLAGS   := -L"$(SDL3_ROOT)/lib" -L"$(SDL3_IMAGE_ROOT)/lib"
LIBS      := -lSDL3_image -lSDL3

# Nếu code của bạn là test.c -> vẫn dùng g++ để link C libs
SRC       := 12_color_modulation.cpp   # đổi thành file của bạn: test.c / main.cpp ...
TARGET    := 12_color_modulation.exe

# ==== rules ====
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS) $(LIBS)

run: $(TARGET)
	@copy /Y "$(SDL3_ROOT)\bin\SDL3.dll" . > NUL
	@copy /Y "$(SDL3_IMAGE_ROOT)\bin\SDL3_image.dll" . > NUL
	@./$(TARGET)

clean:
	-del /Q $(TARGET) 2> NUL
.PHONY: all run clean
