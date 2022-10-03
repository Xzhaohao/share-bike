#include "events_def.h"

std::ostream &MobileCodeReqEv::dump(std::ostream &out) const {
    // return iEvent::dump(out);
    out << "MobileCodeReq sn = " << get_sn() << ", ";
    out << "mobile = " << msg_.mobile() << std::endl;
    return out;
}

std::ostream &MobileCodeRspEv::dump(std::ostream &out) const {
    out << "MobileCodeRspEv sn" << get_sn() << ", ";
    out << "code = " << msg_.code() << std::endl;
    out << "icode = " << msg_.icode() << std::endl;
    out << "describel = " << msg_.data() << std::endl;
    return out;
}
