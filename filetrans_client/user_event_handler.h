
#ifndef FT_BUS_USERM_HANDLER_H_
#define FT_BUS_USERM_HANDLER_H_

#include "glo_def.h"
#include "iEventHandler.h"
#include "events_def.h"
#include "DispatchMsgService.h"

#include <string>
#include <map>
#include <memory>

class UserEventHandler : public iEventHandler
{
public:
    UserEventHandler(std::shared_ptr<DispatchMsgService> dms);
    virtual ~UserEventHandler();
    virtual iEvent* handle(const iEvent* ev);

    static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);
private:
     CommonRspEv* handle_file_trans_start_send();   
     CommonRspEv* handle_file_trans_start_recv(FileTransStartRecvEv* ev);
private:
    std::shared_ptr<DispatchMsgService> dms_;
    std::string filename_;
    int basefd_;
};

#endif
