#pragma once

#include "units_dev_base.h"
#include "units_info_i2c.h"

namespace InSys {

struct unit_dev_axi_i2c_interface : unit_dev_base_interface {};

struct unit_dev_i2c_mux_interface : unit_dev_base_interface {
  virtual void select(uint32_t segment) = 0;
};

class unit_dev_axi_i2c : unit_dev_axi_i2c_interface,
                         public unit_dev_base<unit_info_axi_i2c> {
 public:
  using list_type = units_dev_list<unit_dev_axi_i2c>;
  unit_dev_axi_i2c(const info_type& _info) : unit_dev_base{_info} {}
  std::size_t read() override { return {}; }
  std::size_t write() override { return {}; }
};

}  // namespace InSys
