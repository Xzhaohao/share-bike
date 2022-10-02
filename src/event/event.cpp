#include "event.h"

iEvent::iEvent(u32 eid, u32 sn) : eid_(eid), sn_(sn) {

}

iEvent::~iEvent() = default;

u32 iEvent::generate_seq_no() {
    static u32 sn = 0;
    return sn++;
}
