#include "Task.h"
Task::Task(){

}
Task::~Task(){

}
Task::Task(const Task &obj)
{
    req = obj.req;
    res = obj.res;
    taskSource = obj.taskSource;
    dataPacket = obj.dataPacket;
}
