#pragma once

#include "info_base.h"

namespace InSys {

class info_axi_gpio final : public info_axi_base {
public:
    using list_type = info_list<info_axi_gpio>;
    info_axi_gpio()
        : info_axi_base { "axi-gpio" }
    {
    }
    info_axi_gpio(const std::string_view& name, const std::string_view& label,
        uint64_t axi_offset)
        : info_axi_base { name, label, axi_offset, "axi-gpio" }
    {
    }
};

} // namespace InSys
