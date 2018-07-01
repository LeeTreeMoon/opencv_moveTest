TARGET:=main
SOURCE:=main.cpp
INCLUDE=`pkg-config --cflags --libs opencv`

$(TARGET):$(SOURCE)
	g++ $(SOURCE) -o $@ $(INCLUDE)
clean:
	rm $(TARGET)
