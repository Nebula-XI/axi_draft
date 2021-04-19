#pragma once

#include <memory>

namespace InSys {

template <typename unit_dev_axi_interface_type>
struct unit_dev_axi_interface {
  using shared = typename std::shared_ptr<unit_dev_axi_interface_type>;
  virtual ~unit_dev_axi_interface() noexcept = default;
};

template <typename unit_dev_axi_interface_type,
          typename unit_dev_interface_type>
class unit_dev_base : unit_dev_interface_type {
 public:
  using axi_interface_type = typename unit_dev_axi_interface_type::shared;
  unit_dev_base() = default;
  template <typename unit_dev_type = axi_interface_type>
  unit_dev_base(unit_dev_type &&io) : m_io{std::forward<unit_dev_type>(io)} {}

 protected:
  axi_interface_type m_io{};
};

template <typename unit_dev_type, typename... Args>
auto make_unit_dev(Args &&...args) {
  return std::make_shared<unit_dev_type>(std::forward<Args>(args)...);
}

}  // namespace InSys
