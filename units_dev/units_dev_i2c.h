#pragma once

#include "units_dev_base.h"
#include "units_info_i2c.h"

namespace InSys {

class unit_dev_axi_i2c final : public unit_dev_base<unit_info_axi_i2c> {
 public:
  using list_type = units_dev_list<unit_dev_axi_i2c>;
  unit_dev_axi_i2c() = default;
  unit_dev_axi_i2c(const info_type &info) : unit_dev_base{info, this} {
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

class unit_dev_i2c_mux : public unit_dev_base<unit_info_i2c_mux> {
  uint32_t m_segment{};

 public:
  using list_type = units_dev_list<unit_dev_i2c_mux>;
  unit_dev_i2c_mux() = default;
  unit_dev_i2c_mux(const info_type &info, uint32_t segment,
                   unit_dev_base_interface *io)
      : unit_dev_base{info, io}, m_segment{segment} {}

 private:
  void select_segment() {
    // TODO: Добавить управление сегментом
    std::printf("select_segment[%d]\n", m_segment);
  }
  std::size_t read() final {
    select_segment();
    return m_io->read();
  }
  std::size_t write() final {
    select_segment();
    return m_io->write();
  }
};

class unit_dev_i2c : public unit_dev_base<unit_info_i2c_dev> {
 public:
  using list_type = units_dev_list<unit_dev_i2c>;
  unit_dev_i2c() = default;
  unit_dev_i2c(const info_type &info, unit_dev_base_interface *io)
      : unit_dev_base{info, io} {}
  std::size_t read() final {
    // TODO: выполнить необходимые действия
    std::puts("pre-read");
    return m_io->read();
  }
  std::size_t write() final {
    // TODO: выполнить необходимые действия
    std::puts("pre-write");
    return m_io->write();
  }
};

}  // namespace InSys
