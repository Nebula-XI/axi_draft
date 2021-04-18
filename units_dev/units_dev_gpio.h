#pragma once

#include "units_dev_base.h"
#include "units_info_gpio.h"

namespace InSys {

class unit_dev_axi_gpio final : public unit_dev_base<unit_info_axi_gpio> {
 public:
  unit_dev_axi_gpio() = default;
  unit_dev_axi_gpio(const info_type &info) : unit_dev_base{info} {
    // TODO: установить axi offset
  }
};

}  // namespace InSys
