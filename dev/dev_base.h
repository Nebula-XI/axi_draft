#pragma once

#include <cassert>
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

namespace InSys {

template <typename dev_interface_type>
struct dev_interface {
  using dev_shared = typename std::shared_ptr<dev_interface_type>;
  virtual ~dev_interface() noexcept = default;
};

template <typename dev_axi_interface_type, typename dev_interface_type>
struct dev_axi_interface : dev_interface_type {
  using axi_shared = typename std::shared_ptr<dev_axi_interface_type>;
  virtual ~dev_axi_interface() noexcept = default;
};

template <typename dev_type, typename... args_type>
auto make_dev(args_type &&...args) {
  return std::make_shared<dev_type>(std::forward<args_type>(args)...);
}

template <typename dev_axi_interface_type, typename dev_interface_type,
          typename dev_parent_type>
class dev_base : public dev_interface_type {
 public:
  using dev_interface = typename dev_axi_interface_type::dev_shared;
  using axi_interface = typename dev_axi_interface_type::axi_shared;
  using parent_functor = std::shared_ptr<dev_parent_type>;
  dev_base() = default;
  template <typename axi_interface_type = axi_interface,
            typename parent_functor_type = parent_functor>
  dev_base(axi_interface_type &&io, parent_functor_type &&parent_functor)
      : m_io{std::forward<axi_interface_type>(io)},
        m_parent_functor{std::forward<parent_functor_type>(parent_functor)} {}
  template <typename axi_interface_type = axi_interface>
  dev_base(axi_interface_type &&io)
      : m_io{std::forward<axi_interface_type>(io)} {}

 protected:
  axi_interface m_io{};
  void parent() {
    if (m_parent_functor) {
      auto functor = *m_parent_functor;
      functor();
    }
  }

 private:
  parent_functor m_parent_functor{};
};

}  // namespace InSys
