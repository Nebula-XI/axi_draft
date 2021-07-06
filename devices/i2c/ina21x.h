#pragma once

#include "dev_i2c.h"

namespace InSys {

namespace chips {

    class ina21x : public dev_i2c {
    public:
        using dev_i2c::dev_i2c;
    };

} // namespace chips

} // namespace InSys
