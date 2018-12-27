
#ifndef FT_BUS_MAIN_H_
#define FT_BUS_MAIN_H_

#include "user_event_handler.h"

class BusinessProcessor
{
public:
    BusinessProcessor(std::shared_ptr<DispatchMsgService> dms);
    bool init();
    virtual ~BusinessProcessor();

private:

    std::shared_ptr<DispatchMsgService> dms_;//账户接口分发系统
    std::shared_ptr<UserEventHandler> ueh_;  //账户管理系统
};

#endif