g++ Logger.cpp interface.cpp main.cpp BusProcessor.cpp  DispatchMsgService.cpp  event.cpp events_def.cpp json_protocol_codec.cpp protocol_head.cpp  MsgQueue.cpp user_event_handler.cpp -o main -I/home/qiulb/homework/filetrans_client/include  -L/home/qiulb/homework/filetrans_client/lib/log4cpp -llog4cpp -lpthread -rdynamic -Wall -g3 -m64 -pipe -std=c++0x -lrt -Wno-reorder -Wdeprecated-declarations -fpermissive -ljsoncpp
