#pragma once

#include "units_info_base.h"

namespace InSys {

struct unit_dev_base_interface {
  virtual std::size_t read() = 0;
  virtual std::size_t write() = 0;
  virtual ~unit_dev_base_interface() noexcept = default;
};

template <typename unit_dev_type>
using units_dev_list = std::vector<unit_dev_type>;

template <typename unit_info_type>
class unit_dev_base {
 public:
  using info_type = unit_info_type;
  unit_dev_base() = delete;
  unit_dev_base(const unit_dev_base &other) = default;
  unit_dev_base(unit_dev_base &&) noexcept = default;
  unit_dev_base &operator=(const unit_dev_base &) = default;
  unit_dev_base &operator=(unit_dev_base &&) noexcept = default;
  virtual ~unit_dev_base() noexcept = default;
  unit_dev_base(const info_type &info) : m_info{info} {}
  auto &get_info() const noexcept { return m_info; }

 private:
  info_type m_info{};
};

}  // namespace InSys
