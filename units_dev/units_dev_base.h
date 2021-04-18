#pragma once

#include <memory>

namespace InSys {

struct unit_dev_root_interface {
  using interface_type = std::shared_ptr<unit_dev_root_interface>;
  virtual std::size_t read() = 0;
  virtual std::size_t write() = 0;
  virtual ~unit_dev_root_interface() noexcept = default;
};

class unit_dev_interface : public unit_dev_root_interface {
 public:
  unit_dev_interface() = default;
  unit_dev_interface(const interface_type &io) : m_io{io} {}

 protected:
  interface_type m_io{};
};

template <typename unit_dev_type, typename... Args>
auto make_unit_dev(Args... args) {
  return std::make_shared<unit_dev_type>(args...);
}

}  // namespace InSys
