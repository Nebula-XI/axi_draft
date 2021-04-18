#pragma once

#include "units_dev_base.h"
#include "units_info_sdram.h"

namespace InSys {

class unit_dev_axi_sdram final : public unit_dev_base<unit_info_axi_sdram> {
 public:
  unit_dev_axi_sdram() = default;
  unit_dev_axi_sdram(const info_type &info) : unit_dev_base{info} {
    // TODO: установить axi offset
  }
};

}  // namespace InSys
