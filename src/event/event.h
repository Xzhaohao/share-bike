#ifndef SHARE_BIKE_EVENT_H
#define SHARE_BIKE_EVENT_H

#include "../global_def.h"
#include "event_type.h"
#include <string>

class iEvent {
public:
    iEvent(u32 eid, u32 sn);

    virtual ~iEvent();

    virtual std::ostream &dump(std::ostream &out) const { return out; };

    u32 generate_seq_no();

    u32 get_eid() const { return eid_; };

    u32 get_sn() const { return sn_; };

    void set_eid(u32 eid) { eid_ = eid; }

private:
    u32 eid_;
    u32 sn_;
};

#endif
