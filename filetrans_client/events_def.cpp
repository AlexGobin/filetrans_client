
#include "events_def.h"
#include <iostream>
#include <sstream>

std::ostream& FileTransStartSendEv::dump(std::ostream& out) const
{
	out << "FileTransStartSendEv sn =" << get_sn() << ",";
    out << "filename=" << filename_ << std::endl;
    out << "parallels=" << parallels_ << std::endl;

	return out;
}

std::ostream& FileTransStartRecvEv::dump(std::ostream& out) const
{
	out << "FileTransStartRecvEv sn =" << get_sn() << ",";
    out << "filename=" << filename_ << std::endl;

	return out;
}


std::ostream& CommonRspEv::dump(std::ostream& out) const
{
	out << "CommonRspEv sn =" << get_sn() << ",";
    out << "code=" << code_ << ",";
    out << "msg=" << msg_ << ",";
    out << "data=" << data_ << std::endl;

	return out;
}

