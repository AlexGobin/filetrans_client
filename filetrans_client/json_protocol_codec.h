
#ifndef BRKS_INTF_JSON_PROTOCOL_CODEC_H_
#define BRKS_INTF_JSON_PROTOCOL_CODEC_H_

#include "glo_def.h"
#include "protocol_head.h"
#include "protocol_codec.h"

#include <json/json.h>


class json_protocol_codec_t : public protocol_codec_t
{
public:
	virtual bool encode(iEvent* ev, u8* buffer, u32 size);
	virtual iEvent* decode(u16 mid, u8* buffer, u32 size);

private:
    bool encode_2_file_trans_start_send_ev(FileTransStartSendEv* sendev, u8* buffer, u32 size);
    FileTransStartRecvEv* decode_2_file_trans_start_recv_ev(const Json::Value& attributes);   
};

#endif

