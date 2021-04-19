#pragma once

#include "units_dev_base.h"

namespace InSys {

class unit_dev_axi_i2c final : public unit_dev_axi_interface {
  uint64_t m_axi_offset{};

 public:
  unit_dev_axi_i2c() = default;
  unit_dev_axi_i2c(uint64_t axi_offset) : m_axi_offset{axi_offset} {
    // TODO:
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

class unit_dev_i2c_mux : public unit_dev_interface {
  uint32_t m_address{};
  double m_frequency{};
  uint32_t m_segment{};

 public:
  unit_dev_i2c_mux() = default;
  unit_dev_i2c_mux(uint32_t address, double frequency, uint32_t segment,
                   interface_type io)
      : unit_dev_interface{io},
        m_address{address},
        m_frequency{frequency},
        m_segment{segment} {}
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
    // TODO: Сделать его виртуальным и в производном классе добавить управление
    // сегментом
    std::printf("select_segment[%d]\n", m_segment);
  }
};

class unit_dev_i2c : public unit_dev_interface {
  uint32_t m_address{};
  double m_frequency{};

 public:
  unit_dev_i2c() = default;
  unit_dev_i2c(uint32_t address, double frequency, interface_type io)
      : unit_dev_interface{io}, m_address{address}, m_frequency{frequency} {}
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
