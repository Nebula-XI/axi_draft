#pragma once

#include "units_dev_base.h"

namespace InSys {

class unit_dev_axi_gpio final {
  uint64_t m_axi_offset{};

 public:
  unit_dev_axi_gpio() = default;
  unit_dev_axi_gpio(uint64_t axi_offset) : m_axi_offset{axi_offset} {
    // TODO:
  }
};

}  // namespace InSys
