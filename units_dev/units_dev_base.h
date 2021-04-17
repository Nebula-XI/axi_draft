#pragma once

#include <memory>

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
class unit_dev_base : public unit_dev_base_interface {
 public:
  using info_type = unit_info_type;
  unit_dev_base() = default;
  virtual ~unit_dev_base() noexcept = default;
  template <typename Type = info_type>
  unit_dev_base(Type &&info, unit_dev_base_interface *io)
      : m_info{std::forward<Type>(info)}, m_io{io} {}
  auto &get_info() const noexcept { return m_info; }

 protected:
  unit_dev_base_interface *m_io{};

 private:
  info_type m_info{};
};

}  // namespace InSys
