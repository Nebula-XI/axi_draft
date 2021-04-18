#pragma once

#include "units_dev_base.h"
#include "units_info_i2c.h"

namespace InSys {

class unit_dev_axi_i2c final : public unit_dev_root_interface,
                               public unit_dev_base<unit_info_axi_i2c> {
 public:
  unit_dev_axi_i2c() = default;
  unit_dev_axi_i2c(const info_type &info) : unit_dev_base{info} {
    // TODO: установить axi offset
  }
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

class unit_dev_i2c_mux : public unit_dev_interface,
                         public unit_dev_base<unit_info_i2c_mux> {
  uint32_t m_segment{};

 public:
  unit_dev_i2c_mux() = default;
  unit_dev_i2c_mux(const info_type &info, uint32_t segment, interface_type io)
      : unit_dev_interface{io}, unit_dev_base{info}, m_segment{segment} {}
  std::size_t read() override {
    select_segment();
    return m_io->read();
  }
  std::size_t write() override {
    select_segment();
    return m_io->write();
  }

 protected:
  virtual void select_segment() {
    // TODO: Добавить управление сегментом
    std::printf("select_segment[%d]\n", m_segment);
  }
};

class unit_dev_i2c : public unit_dev_interface,
                     public unit_dev_base<unit_info_i2c_dev> {
 public:
  unit_dev_i2c() = default;
  unit_dev_i2c(const info_type &info, interface_type io)
      : unit_dev_interface{io}, unit_dev_base{info} {}
  std::size_t read() override {
    // TODO: выполнить необходимые действия
    std::puts("pre-read");
    return m_io->read();
  }
  std::size_t write() override {
    // TODO: выполнить необходимые действия
    std::puts("pre-write");
    return m_io->write();
  }
};

}  // namespace InSys
