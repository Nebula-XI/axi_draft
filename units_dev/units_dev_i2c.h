#pragma once

#include "units_dev_base.h"
#include "units_info_i2c.h"

namespace InSys {

struct unit_dev_axi_i2c_interface : unit_dev_base_interface {};

class unit_dev_axi_i2c final : public unit_dev_axi_i2c_interface,
                               public unit_dev_base<unit_info_axi_i2c> {
 public:
  using list_type = units_dev_list<unit_dev_axi_i2c>;
  unit_dev_axi_i2c(const info_type& info) : unit_dev_base{info} {
    // TODO: установить axi offset
  }

 private:
  std::size_t read() final {
    // TODO: добавить чтение
    std::puts("read");
    return {};
  }
  std::size_t write() final {
    // TODO: добавить запись
    std::puts("write");
    return {};
  }
};

class unit_dev_i2c_mux : public unit_dev_axi_i2c_interface,
                         public unit_dev_base<unit_info_i2c_mux> {
  uint32_t m_segment{};
  unit_dev_axi_i2c_interface& m_i2c;

 public:
  using list_type = units_dev_list<unit_dev_i2c_mux>;
  unit_dev_i2c_mux(const info_type& info, uint32_t segment,
                   unit_dev_axi_i2c_interface& i2c)
      : unit_dev_base{info}, m_segment{segment}, m_i2c{i2c} {}

 private:
  void select_segment() {
    // TODO: Добавить управление сегментом
    std::printf("select_segment[%d]\n", m_segment);
  }
  std::size_t read() final {
    select_segment();
    return m_i2c.read();
  }
  std::size_t write() final {
    select_segment();
    return m_i2c.write();
  }
};

class unit_dev_i2c : public unit_dev_axi_i2c_interface,
                     public unit_dev_base<unit_info_i2c_dev> {
  unit_dev_axi_i2c_interface& m_i2c;

 public:
  using list_type = units_dev_list<unit_dev_i2c>;
  unit_dev_i2c(const info_type& info, unit_dev_axi_i2c_interface& i2c)
      : unit_dev_base{info}, m_i2c{i2c} {}
  std::size_t read() final {
    // TODO: выполнить необходимые действия
    std::puts("pre-read");
    return m_i2c.read();
  }
  std::size_t write() final {
    // TODO: выполнить необходимые действия
    std::puts("pre-write");
    return m_i2c.write();
  }
};

}  // namespace InSys
