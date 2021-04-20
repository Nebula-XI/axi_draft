#pragma once

#include <memory>

namespace InSys {

template <typename unit_dev_axi_interface_type>
struct unit_dev_axi_interface {
  using shared = typename std::shared_ptr<unit_dev_axi_interface_type>;
  virtual ~unit_dev_axi_interface() noexcept = default;
};

template <typename unit_dev_axi_interface_type,
          typename unit_dev_interface_type, typename unit_dev_type>
class unit_dev_base : public unit_dev_interface_type {
 public:
  using axi_interface_type = typename unit_dev_axi_interface_type::shared;
  using parent_functor_type = std::shared_ptr<unit_dev_type>;
  unit_dev_base() = default;
  template <typename type_io = axi_interface_type,
            typename type_fn = parent_functor_type>
  unit_dev_base(type_io &&io, type_fn &&parent)
      : m_io{std::forward<type_io>(io)},
        m_parent{std::forward<type_fn>(parent)} {}
  template <typename type_io = axi_interface_type>
  unit_dev_base(type_io &&io) : m_io{std::forward<type_io>(io)} {}

 protected:
  axi_interface_type m_io{};
  void parent() {
    if (m_parent) {
      auto fn = *m_parent;
      fn();
    }
  }

 private:
  parent_functor_type m_parent{};
};

template <typename unit_dev_type, typename... Args>
auto make_unit_dev(Args &&...args) {
  return std::make_shared<unit_dev_type>(std::forward<Args>(args)...);
}

}  // namespace InSys
