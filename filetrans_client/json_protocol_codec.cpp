#include "json_protocol_codec.h"
#include "Logger.h"
#include <json/json.h>

bool json_protocol_codec_t::encode(iEvent* ev, u8* buffer, u32 size)
{
    bool flag = false;
    u32 sid = ev->get_eid();//获取事件类型
    switch (sid)
    {
        case EEVENTID_START_SEND:
            flag = encode_2_file_trans_start_send_ev(ev,buffer,size);
            break;  
        default:
            LOG_WARN("sid %d is invalid.", sid);
            break;        
    }

    return flag;
}

iEvent* json_protocol_codec_t::decode(u16 mid, u8* buffer, u32 size)
{
    Json::Reader reader;
    Json::Value attributes;
    if (!reader.parse((const char*)buffer, attributes))
        return NULL;

    iEvent* ev= NULL;

    switch (mid )
    {          
        case EEVENTID_START_RECV:
            ev = decode_2_file_trans_start_recv_ev(attributes);
            break;
        default:
            LOG_WARN("mid %d is invalid.", mid);
            break;
    }

    return ev;
}
bool json_protocol_codec_t::encode_2_file_trans_start_send_ev(FileTransStartSendEv* sendev, u8* buffer, u32 size)
{

/*    Json::Value attributes;
    attributes["filename"] = sendev->get_filename();
    attributes["parallels"]  = sendev->get_parallels();
    Json::FastWriter writer;
    std::string body = writer.write(attributes);
    size = body.size();
    memcpy(buffer, body.data(), body.size());*/

    return true;
}

FileTransStartRecvEv* json_protocol_codec_t::decode_2_file_trans_start_recv_ev(const Json::Value& attributes)
{
    if (attributes["filename"] == Json::nullValue)
    {
        return NULL;
    }

    std::string filename = attributes["filename"].asString();

    FileTransStartRecvEv* ev = new FileTransStartRecvEv(filename);

    return ev;
}

