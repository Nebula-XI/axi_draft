#pragma once

#include "units_info_base.h"

namespace InSys {

template <typename unit_dev_type>
using units_dev_list = std::vector<unit_dev_type>;

template <typename unit_info_type>
class unit_dev_base {
 public:
  using info_type = unit_info_type;
  unit_dev_base() = delete;
  ~unit_dev_base() noexcept = default;
  unit_dev_base(const info_type &_info) : info{_info} {}

  info_type get_info() const { return info; }

 private:
  info_type info{};
};

}  // namespace InSys
