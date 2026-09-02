#ifndef Task_H
#define Task_H
#include "PLCRequest.h"
#include "PLCResponse.h"
#include "CloudDataPacket.h"

typedef enum{
    SELF_REPORT=1,
    CLOUD_REQUEST=2,
    UI_REQUEST=3
}TaskDirection;


class Task{

public:
    Task();
    Task(const Task &obj);
    ~Task();
    PLCRequest req;    //向PLC任务线程发送的请求（8个字节）
    PLCResponse res;   //PLC任务线程返回的响应（9个字节）

    TaskDirection taskSource=TaskDirection::UI_REQUEST;  //根据这个，判断发送哪个信号，触发响应的调用者槽函数
    //TaskDirection taskTo;  //根据这个，判断发送哪个信号，触发响应的调用者槽函数
    //bool (*callback)(Request req, Response res);
    //QObject caller;
    CloudDataPacket dataPacket;   //Socket与各个函数之间传递的数据包
};




/*
typedef  struct{
    Request req;
    Response res;
    TaskFrom taskFrom;
    //bool (MainWindow::*callback)(Request req, Response res);
    //QObject *caller;
}Task;
*/
#endif // Task_H
