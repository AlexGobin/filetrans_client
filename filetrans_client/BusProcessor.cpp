
#include "BusProcessor.h"

BusinessProcessor::BusinessProcessor(std::shared_ptr<DispatchMsgService> dms)
    : dms_(dms),ueh_(new UserEventHandler(dms))
{

}

bool BusinessProcessor::init()
{
    return true;
}

BusinessProcessor::~BusinessProcessor()
{
    ueh_.reset();
}

