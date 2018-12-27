
#include "user_event_handler.h"
#include "Logger.h"

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>
#include <unistd.h>
#include <json/json.h>


//#include "user_service.h"


#define SA struct sockaddr
#define MAXLINE 4096
#define MAXSUB  2000
#define MAXPARAM 2048
#define LISTENQ 1024

extern int h_errno;

UserEventHandler::UserEventHandler(std::shared_ptr<DispatchMsgService> dms)
    : dms_(dms), basefd_(0)
{
    dms_->subscribe(EEVENTID_START_SEND, this);
    dms_->subscribe(EEVENTID_START_RECV, this);
    
    dms_->subscribe(EEVENTID_DOWNLOAD_SEND, this);
    dms_->subscribe(EEVENTID_DOWNLOAD_RECV, this);
    
    dms_->subscribe(EEVENTID_COMMON_RSP, this);
    
    dms_->subscribe(EEVENTID_FINISH_RECV, this);
    dms_->subscribe(EEVENTID_FINISH_SEND, this);
}

UserEventHandler::~UserEventHandler()
{
    dms_->unsubscribe(EEVENTID_START_SEND, this);
    dms_->unsubscribe(EEVENTID_START_RECV, this);
    
    dms_->unsubscribe(EEVENTID_DOWNLOAD_SEND, this);
    dms_->unsubscribe(EEVENTID_DOWNLOAD_RECV, this);
    
    dms_->unsubscribe(EEVENTID_COMMON_RSP, this);
    
    dms_->unsubscribe(EEVENTID_FINISH_RECV, this);
    dms_->unsubscribe(EEVENTID_FINISH_SEND, this);

}

iEvent* UserEventHandler::handle(const iEvent* ev)
{
    if (ev == NULL)
    {
        LOG_ERROR("input ev is NULL");
    }

    u32 eid = ev->get_eid();

    if (eid == EEVENTID_START_SEND)
    {
        return handle_file_trans_start_send();
    }
    else if (eid == EEVENTID_START_RECV)
    {
        return handle_file_trans_start_recv((FileTransStartRecvEv*) ev);
    }

    return NULL;
}
CommonRspEv* UserEventHandler::handle_file_trans_start_send()
{
    LOG_DEBUG("send sms success");
    return new CommonRspEv(200, "success", "");
}

CommonRspEv* UserEventHandler::handle_file_trans_start_recv(FileTransStartRecvEv* ev)
{
    LOG_DEBUG("send sms success");
    return new CommonRspEv(200, "success", "");
}

size_t UserEventHandler::write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    std::string data((const char*) ptr, (size_t) size * nmemb);
    *((std::stringstream*) stream) << data << endl;

    return size * nmemb;
}

