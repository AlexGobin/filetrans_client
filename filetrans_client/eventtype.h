#ifndef BRKS_COMMON_EVENT_TYPE_H_
#define BRKS_COMMON_EVENT_TYPE_H_

enum EErrorCode
{
      ERRC_SUCCESS              = 200,
	  ERRC_INVALID_MSG          = 400,
	  ERRC_INVALID_DATA         = 404,
	  ERRC_METHOD_NOT_ALLOWED   = 405,
	  ERRO_PROCCESS_FAILED      = 406,
	  ERRO_BIKE_IS_TOOK         = 407,
	  ERRO_BIKE_IS_RUNNING      = 408,
	  ERRO_BIKE_IS_DAMAGED      = 409
};

/* 事件ID */
enum EventID
{
    EEVENTID_COMMON_RSP           = 0x00,

    EEVENTID_START_SEND           = 0x01,
    EEVENTID_START_RECV           = 0x02,

    EEVENTID_DOWNLOAD_SEND        = 0x03,
    EEVENTID_DOWNLOAD_RECV        = 0x04,

    EEVENTID_FINISH_SEND          = 0x05,
    EEVENTID_FINISH_RECV          = 0x06,

	EEVENTID_UNKOWN               = 0xFFFF
};


#endif

