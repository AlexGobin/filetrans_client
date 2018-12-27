
#ifndef BRKS_COMMON_EVENTS_DEF_H_
#define BRKS_COMMON_EVENTS_DEF_H_

#include "event.h"
#include "glo_def.h"
#include "eventtype.h"

#include <string>
#include <vector>

class FileTransStartSendEv : public iEvent
{
public:
    FileTransStartSendEv(const std::string& filename,const int     &parallels) :
        iEvent(EEVENTID_START_SEND, generateSeqNo()), filename_(filename),parallels_(parallels)
    {};

    const std::string& get_filename(){return filename_;};
    const int& get_parallels(){return parallels_;};
    virtual std::ostream& dump(std::ostream& out) const;
private:
    std::string filename_;
    int parallels_;
};

class FileTransStartRecvEv : public iEvent
{
public:
    FileTransStartRecvEv(const std::string& filename) :
        iEvent(EEVENTID_START_RECV, generateSeqNo()), filename_(filename)
    {};

    const std::string& get_filename(){return filename_;};
    virtual std::ostream& dump(std::ostream& out) const;
private:
    std::string filename_;
};

/*class StartSendEv : public iEvent
{
public:
    StartSendEv(const std::string& filename, const std::string& parallels) :
        iEvent(EEVENTID_START_SEND, generateSeqNo()), filename_(filename), parallels_(parallels)
	{};

    const std::string& get_filename(){return filename_;};
    const std::string& get_parallels(){ return parallels_;};
    virtual std::ostream& dump(std::ostream& out) const;

private:
    std::string   filename_;  //文件名称
    std::string   parallels_;  //并行数
};*/

class CommonRspEv : public iEvent
{
public:
    CommonRspEv(i32 code, const std::string& msg, const std::string& data) :
        iEvent(EEVENTID_COMMON_RSP, generateSeqNo()), code_(code), msg_(msg), data_(data)
	{};

    i32 get_code(){return code_;};
    const std::string& get_msg(){return msg_;};
    const std::string& get_data(){ return data_;};
    virtual std::ostream& dump(std::ostream& out) const;

private:
    i32           code_;  //返回代码
    std::string   msg_;   //返回消息，对代码在具体业务场景下的解�?
    std::string   data_;  //辅助数据
};

#endif

