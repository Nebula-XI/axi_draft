#pragma once

#include "units_dev_base.h"

namespace InSys {

class unit_dev_axi_spi final : public unit_dev_root_interface {
  uint64_t m_axi_offset{};
  uint32_t m_chip_select{};
  double m_frequency{};

 public:
  unit_dev_axi_spi() = default;
  unit_dev_axi_spi(uint64_t axi_offset, uint32_t chip_select, double frequency)
      : m_axi_offset{axi_offset},
        m_chip_select{chip_select},
        m_frequency{frequency} {}
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

class unit_dev_spi : public unit_dev_interface {
 public:
  unit_dev_spi() = default;
  unit_dev_spi(interface_type io) : unit_dev_interface{io} {}
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
