#pragma once

#include "dev_base.h"

namespace InSys {

using i2c_address = uint16_t;
using i2c_data = std::vector<uint8_t>;
using i2c_segment = uint32_t;

struct dev_i2c_interface : dev_interface<dev_i2c_interface> {
  virtual size_t write(i2c_address, const i2c_data&) = 0;
  virtual i2c_data read(i2c_address) = 0;
  virtual ~dev_i2c_interface() noexcept = default;
};

struct dev_axi_i2c_interface
    : dev_axi_interface<dev_axi_i2c_interface, dev_i2c_interface> {
  virtual ~dev_axi_i2c_interface() noexcept = default;
  virtual void configure() = 0;
};

template <typename dev_parent_type>
using dev_i2c_base =
    dev_base<dev_axi_i2c_interface, dev_i2c_interface, dev_parent_type>;

class dev_axi_i2c final : public dev_axi_i2c_interface,
                          public dev_base_creator<dev_axi_i2c> {
  uint64_t m_axi_offset{};

 public:
  // TODO: Добавить управление драйвером i2c. \
           Необходимо обеспечить чтобы на каждый оффсет был один экземпляр. \
           Конфигурацию обновлять если необходимо. 
  dev_axi_i2c() = default;
  dev_axi_i2c(uint64_t axi_offset) : m_axi_offset{axi_offset} {}
  void configure() final {}
  size_t write(i2c_address address, const i2c_data& data) final { return {}; }
  i2c_data read(i2c_address address) final { return {}; }
};

class dev_i2c_mux : public dev_i2c_base<dev_i2c_mux>,
                    public dev_base_creator<dev_i2c_mux> {
  i2c_segment m_segment{};

 public:
  dev_i2c_mux() = default;
  dev_i2c_mux(i2c_segment segment, axi_interface io)
      : dev_base{io}, m_segment{segment} {}
  dev_i2c_mux(i2c_segment segment, axi_interface io, parent_functor fn)
      : dev_base{io, fn}, m_segment{segment} {}
  size_t write(i2c_address address, const i2c_data& data) final {
    assert(m_io.use_count() != 0 && m_io.get() != nullptr);
    parent();
    m_io->configure();
    return m_io->write(address, data);
  }
  i2c_data read(i2c_address address) final {
    assert(m_io.use_count() != 0 && m_io.get() != nullptr);
    parent();
    m_io->configure();
    return m_io->read(address);
  }
  void operator()() { segment(); }
  virtual void segment() { parent(); }
};

class dev_i2c : public dev_i2c_base<dev_i2c_mux>,
                public dev_base_creator<dev_i2c> {
 public:
  dev_i2c() = default;
  dev_i2c(axi_interface io) : dev_base{io} {}
  dev_i2c(axi_interface io, parent_functor fn) : dev_base{io, fn} {}
  size_t write(i2c_address address, const i2c_data& data) final {
    assert(m_io.use_count() != 0 && m_io.get() != nullptr);
    parent();
    m_io->configure();
    return m_io->write(address, data);
  }
  i2c_data read(i2c_address address) final {
    assert(m_io.use_count() != 0 && m_io.get() != nullptr);
    parent();
    m_io->configure();
    return m_io->read(address);
  }
};

}  // namespace InSys
