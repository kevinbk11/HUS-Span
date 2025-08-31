CXX = g++
CXXFLAGS = -g -O0 -I./src -MMD -MP

# 自動抓 main.cpp + src 資料夾下所有 cpp
SRCS = main.cpp $(wildcard src/**/*.cpp)
OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)
TARGET = a.exe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 匯入依賴檔（.d 檔會包含 .h 的依賴關係）
-include $(DEPS)

clean:
	del /Q $(OBJS) $(DEPS) $(TARGET)   # Windows
	# rm -f $(OBJS) $(DEPS) $(TARGET)  # Linux/Mac
