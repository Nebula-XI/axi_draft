#pragma once

#include "units_dev_base.h"
#include "units_info_i2c.h"

namespace InSys {

struct unit_dev_axi_i2c_interface {
  virtual std::size_t read() = 0;
  virtual std::size_t write() = 0;
  ~unit_dev_axi_i2c_interface() noexcept = default;
};
class unit_dev_axi_i2c final : public unit_dev_axi_i2c_interface,
                               public unit_dev_base<unit_info_axi_i2c> {
 public:
  using list_type = units_dev_list<unit_dev_axi_i2c>;
  unit_dev_axi_i2c(const info_type& _info) : unit_dev_base{_info} {}

 private:
  std::size_t read() final {
    std::puts("read");
    return {};
  }
  std::size_t write() final {
    std::puts("write");
    return {};
  }
};

class unit_dev_i2c_mux : public unit_dev_axi_i2c_interface,
                         public unit_dev_base<unit_info_i2c_mux> {
  uint32_t segment{};
  unit_dev_axi_i2c_interface& i2c;

 public:
  using list_type = units_dev_list<unit_dev_i2c_mux>;
  unit_dev_i2c_mux(const info_type& _info, uint32_t _segment,
                   unit_dev_axi_i2c_interface& _i2c)
      : unit_dev_base{_info}, segment{_segment}, i2c{_i2c} {}

 private:
  void select_segment() {
    std::puts("select_segment");
  }
  std::size_t read() final {
    select_segment();
    return i2c.read();
  }
  std::size_t write() final {
    select_segment();
    return i2c.write();
  }
};

class unit_dev_i2c : public unit_dev_axi_i2c_interface,
                     public unit_dev_base<unit_info_i2c_dev> {
  unit_dev_axi_i2c_interface& i2c;

 public:
  using list_type = units_dev_list<unit_dev_i2c>;
  unit_dev_i2c(const info_type& _info, unit_dev_axi_i2c_interface& _i2c)
      : unit_dev_base{_info}, i2c{_i2c} {}
  std::size_t read() final {
    // TODO: выполнить необходимые действия
    return i2c.read();
  }
  std::size_t write() final {
    // TODO: выполнить необходимые действия
    return i2c.write();
  }
};

}  // namespace InSys
