CXX=g++
CFLAGS=-std=c++11 -O2 -Wall -g
CXXFLAGS=-std=c++11 -O2 -Wall -g

TARGET:=myserver
SOURCE:=$(wildcard ../*.cpp)
OBJS=./buff/buffer.cpp ./http/HTTPrequest.cpp ./http/HTTPresponse.cpp ./http/HTTPconnection.cpp \
     ./timer/timer.cpp ./epoll/epoll.cpp ./webserver/webserver.cpp ./main.cpp

$(TARGET):$(OBJS)
	$(CXX) $(CXXFLAGS)  $(OBJS) -o ./bin/$(TARGET) -pthread

