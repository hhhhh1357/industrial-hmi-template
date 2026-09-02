#include "CloudTaskHandler.h"


CloudTaskHandler::~CloudTaskHandler()
{
    m_run = false;
    m_tcp->deleteLater();
}

void CloudTaskHandler::run()
{


}

