#ifndef _ZONEID_H_
#define _ZONEID_H_

#include "irrlicht.h"

namespace Engine
{

class ZoneID
{
public:
    ZoneID();
    ZoneID(irr::s32 x, irr::s32 z);
    ZoneID(const ZoneID &zone);
    void set(irr::s32 x, irr::s32 z);
    bool operator< (const ZoneID &zone) const;
    bool operator> (const ZoneID &zone) const;
    bool operator== (const ZoneID &zone) const;
    bool operator!= (const ZoneID &zone) const;
    bool operator>= (const ZoneID &zone) const;
    bool operator<= (const ZoneID &zone) const;
    irr::s32 X();
    irr::s32 Z();
    bool invalid();
protected:
    bool undefined;
    irr::s32 x, z;
};

};

#endif // _ZONEID_H_
