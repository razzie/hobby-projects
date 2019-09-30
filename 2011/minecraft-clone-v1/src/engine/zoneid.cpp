#include "engine\zone.h"

using namespace irr;
using namespace Engine;

ZoneID::ZoneID() : undefined(true)
{
}

ZoneID::ZoneID(s32 x, s32 z) : undefined(false), x(x), z(z)
{
}

ZoneID::ZoneID(const ZoneID &zone) : undefined(zone.undefined), x(zone.x), z(zone.z)
{
}

void ZoneID::set(s32 x, s32 z)
{
    this->x = x;
    this->z = z;
    undefined = false;
}

bool ZoneID::operator< (const ZoneID &zone) const
{
    if (x < zone.x)
        return true;
    else if(x == zone.x)
    {
        if (z < zone.z)
            return true;
        else
            return false;
    }
    else
        return false;
}

bool ZoneID::operator> (const ZoneID &zone) const
{
    if (x > zone.x)
        return true;
    else if(x == zone.x)
    {
        if (z > zone.z)
            return true;
        else
            return false;
    }
    else
        return false;
}

bool ZoneID::operator== (const ZoneID &zone) const
{
    if (x == zone.x && z == zone.z)
        return true;
    else
        return false;
}

bool ZoneID::operator!= (const ZoneID &zone) const
{
    return !(*this == zone);
}

bool ZoneID::operator<= (const ZoneID &zone) const
{
    return (*this)<zone || (*this)==zone;
}

bool ZoneID::operator>= (const ZoneID &zone) const
{
    return (*this)>zone || (*this)==zone;
}

s32 ZoneID::X()
{
    return x;
}

s32 ZoneID::Z()
{
    return z;
}

bool ZoneID::invalid()
{
    return undefined;
}
