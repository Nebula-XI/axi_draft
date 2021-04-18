#pragma once

#include <memory>

namespace InSys {

struct unit_dev_axi_interface {
  using interface_type = std::shared_ptr<unit_dev_axi_interface>;
  virtual std::size_t read() = 0;
  virtual std::size_t write() = 0;
  virtual ~unit_dev_axi_interface() noexcept = default;
};

class unit_dev_interface : public unit_dev_axi_interface {
 public:
  unit_dev_interface() = default;
  template <typename unit_dev_type = interface_type>
  unit_dev_interface(unit_dev_type &&io)
      : m_io{std::forward<unit_dev_type>(io)} {}

 protected:
  interface_type m_io{};
};

template <typename unit_dev_type, typename... Args>
auto make_unit_dev(Args &&...args) {
  return std::make_shared<unit_dev_type>(std::forward<Args>(args)...);
}

}  // namespace InSys
