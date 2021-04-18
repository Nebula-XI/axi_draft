#pragma once

#include "units_dev_base.h"
#include "units_info_spi.h"

namespace InSys {

class unit_dev_axi_spi final : public unit_dev_root_interface,
                               public unit_dev_base<unit_info_axi_spi> {
 public:
  unit_dev_axi_spi() = default;
  unit_dev_axi_spi(const info_type &info) : unit_dev_base{info} {
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

class unit_dev_spi final : public unit_dev_interface,
                           public unit_dev_base<unit_info_spi_dev> {
 public:
  unit_dev_spi() = default;
  unit_dev_spi(const info_type &info, interface_type io)
      : unit_dev_interface{io}, unit_dev_base{info} {}
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
