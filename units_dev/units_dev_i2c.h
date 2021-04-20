#pragma once

#include "units_dev_base.h"

namespace InSys {

using i2c_address = uint16_t;
using i2c_data = std::vector<uint8_t>;

struct unit_dev_i2c_interface {
  virtual size_t write(i2c_address, const i2c_data&) = 0;
  virtual i2c_data read(i2c_address) = 0;
  virtual ~unit_dev_i2c_interface() noexcept = default;
};

struct unit_dev_axi_i2c_interface
    : unit_dev_i2c_interface,
      unit_dev_axi_interface<unit_dev_axi_i2c_interface> {
  virtual ~unit_dev_axi_i2c_interface() noexcept = default;
  virtual void configure() = 0;
};

template <typename unit_dev_type>
using unit_dev_i2c_base = unit_dev_base<unit_dev_axi_i2c_interface,
                                        unit_dev_i2c_interface, unit_dev_type>;

class unit_dev_axi_i2c final : public unit_dev_axi_i2c_interface {
  uint64_t m_axi_offset{};
  // TODO: добавить необходимую конфигурацию axi_i2c

 public:
  unit_dev_axi_i2c() = default;
  // TODO:
  void configure() final {
    // TODO: добавить конфигурирование \
    обновить только те параметры которые необходимо
    std::puts("configure axi_i2c");
  }
  size_t write(i2c_address address, const i2c_data& data) final {
    // TODO: добавить запись
    std::puts("write axi_i2c");
    return {};
  }
  i2c_data read(i2c_address address) final {
    // TODO: добавить чтение
    std::puts("read axi_i2c");
    return {};
  }
};

class unit_dev_i2c_mux
    : public unit_dev_base<unit_dev_axi_i2c_interface, unit_dev_i2c_interface,
                           unit_dev_i2c_mux> {
  uint32_t m_segment{};

 public:
  unit_dev_i2c_mux() = default;
  unit_dev_i2c_mux(uint32_t segment, axi_interface_type io)
      : unit_dev_base{io}, m_segment{segment} {}
  unit_dev_i2c_mux(uint32_t segment, axi_interface_type io,
                   parent_functor_type mux)
      : unit_dev_base{io, mux}, m_segment{segment} {}
  size_t write(i2c_address address, const i2c_data& data) final {
    parent();
    std::puts("write i2c_mux");
    m_io->configure();
    m_io->write(0, {});
    return {};
  }
  i2c_data read(i2c_address address) final {
    parent();
    std::puts("read i2c_mux");
    m_io->configure();
    m_io->read(0);
    return {};
  }
  void operator()() { segment(); }
  virtual void segment() {
    parent();
    std::puts("segment i2c_mux");
    read(0);
    write(0, {});
  }
};

class unit_dev_i2c
    : public unit_dev_base<unit_dev_axi_i2c_interface, unit_dev_i2c_interface,
                           unit_dev_i2c_mux> {
 public:
  unit_dev_i2c() = default;
  unit_dev_i2c(axi_interface_type io, parent_functor_type mux)
      : unit_dev_base{io, mux} {}
  size_t write(i2c_address address, const i2c_data& data) final {
    std::puts("write i2c_dev");
    parent();
    m_io->configure();
    m_io->write(0, {});
    return {};
  }
  i2c_data read(i2c_address address) final {
    std::puts("read i2c_dev");
    parent();
    m_io->configure();
    m_io->read(0);
    return {};
  }
};

}  // namespace InSys
