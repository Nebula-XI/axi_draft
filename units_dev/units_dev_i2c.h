#pragma once

#include "units_dev_base.h"
#include "units_info_i2c.h"

namespace InSys {

struct unit_dev_axi_i2c_interface : unit_dev_base_interface {};

struct unit_dev_i2c_mux_interface : unit_dev_base_interface {
  virtual std::size_t read() = 0;
  virtual std::size_t write() = 0;
};

class unit_dev_axi_i2c : public unit_dev_base<unit_info_axi_i2c> {
 public:
  using list_type = units_dev_list<unit_dev_axi_i2c>;
  unit_dev_axi_i2c(const unit_info_axi_i2c& _info) : unit_dev_base{_info} {}
};

}  // namespace InSys
