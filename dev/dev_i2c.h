#pragma once

#include "dev_base.h"
#include "dev_axi.h"
#include "utility.h"
#include "xilinx_iic.h"

namespace InSys {

using i2c_address = uint16_t;
using i2c_data = std::vector<uint8_t>;
using i2c_segment = uint32_t;

enum class i2c_addressing { _7bit, _10bit };

struct dev_i2c_interface : dev_interface<dev_i2c_interface> {
  virtual ~dev_i2c_interface() noexcept = default;
};

struct dev_axi_i2c_interface
    : dev_axi_interface<dev_axi_i2c_interface, dev_i2c_interface> {
  virtual void configure(i2c_address, double, i2c_addressing) noexcept = 0;
  virtual void set_address(i2c_address) noexcept = 0;
  virtual i2c_address get_address() const noexcept = 0;
  virtual void set_addressing(i2c_addressing) noexcept = 0;
  virtual i2c_addressing get_addressing() const noexcept = 0;
  virtual void set_frequency(double) noexcept = 0;
  virtual double get_frequency() const noexcept = 0;
  virtual ~dev_axi_i2c_interface() noexcept = default;
};

namespace detail {

template <typename dev_parent_type>
class dev_i2c_base : public dev_base<dev_axi_i2c_interface, dev_i2c_interface,
                                     dev_parent_type> {
 public:
  dev_i2c_base() = default;
  dev_i2c_base(typename dev_base<dev_axi_i2c_interface, dev_i2c_interface,
                                 dev_parent_type>::axi_interface io,
               typename dev_base<dev_axi_i2c_interface, dev_i2c_interface,
                                 dev_parent_type>::parent_functor fn,
               i2c_address address, double frequency = double(400_kHz),
               i2c_addressing addressing = i2c_addressing::_7bit)
      : dev_base<dev_axi_i2c_interface, dev_i2c_interface, dev_parent_type>{io,
                                                                            fn},
        m_address{address},
        m_frequency{frequency},
        m_addressing{addressing} {}
  dev_i2c_base(typename dev_base<dev_axi_i2c_interface, dev_i2c_interface,
                                 dev_parent_type>::axi_interface io,
               i2c_address address, double frequency = double(400_kHz),
               i2c_addressing addressing = i2c_addressing::_7bit)
      : dev_base<dev_axi_i2c_interface, dev_i2c_interface, dev_parent_type>{io},
        m_address{address},
        m_frequency{frequency},
        m_addressing{addressing} {}

 protected:
  i2c_address m_address{};
  double m_frequency{};
  i2c_addressing m_addressing{};
};

}  // namespace detail

class dev_axi_i2c final : public detail::dev_axi_base<dev_axi_i2c_interface>,
                          public detail::dev_base_creator<dev_axi_i2c> {
 public:
  // TODO: Добавить управление драйвером i2c. \
           Необходимо обеспечить чтобы на каждый оффсет был один экземпляр. \
           Конфигурацию обновлять если необходимо.
  dev_axi_i2c() = default;
  dev_axi_i2c(uint64_t axi_offset) : dev_axi_base{axi_offset} {}
  void configure(i2c_address address, double frequency,
                 i2c_addressing addressing) noexcept final {
    // TODO: проверить какой из параметром изменился и обновить
  }
  uint64_t get_axi_offset() const noexcept final { return m_axi_offset; }
  void set_address(i2c_address address) noexcept final {
    // TODO: добавить установку адреса
  }
  i2c_address get_address() const noexcept final {
    // TODO: добавить получение адреса
    return {};
  }
  void set_addressing(i2c_addressing addressing) noexcept final {
    // TODO: добавить устрановку режима адресации
  }
  i2c_addressing get_addressing() const noexcept final {
    // TODO: добавить запрос режима адресации
    return {};
  }
  void set_frequency(double frequency) noexcept final {
    // TODO: добавить установку частоты
    float axi_freq = 250000000.0f;
    float iic_freq = frequency;
    float scl_internal_delay = 0.0f;
    uint32_t val = (uint32_t)(((axi_freq/(2.0 * iic_freq)) - 7.0f - scl_internal_delay + 0.5f));
    m_hw->write_axi(m_axi_offset, xilinx_iic::THIGH, val & 0xffffffff);
    m_hw->write_axi(m_axi_offset, xilinx_iic::TLOW, val & 0xffffffff);
  }
  double get_frequency() const noexcept final {
    // TODO: добавить запрос частоты
    return {};
  }
  size_t write(const i2c_data& data) final {
    auto written = write(m_address, data);
    if(!written)
      return 0;
    return data.size();
  }
  size_t read(i2c_data& data, const size_t N) final {
    auto readed = read(m_address, data, N);
    if(!readed)
      return 0;
    return data.size();
  }
 private:

  devaxi_t  m_hw;
  i2c_address m_address{};
  double m_frequency{};
  i2c_addressing m_addressing{};
  uint32_t m_axi_offset;

  void enable() {
    m_hw->write_axi(m_axi_offset, xilinx_iic::RX_FIFO_PIRQ, 0xF);                                   //! Установим глубину RX_FIFO
    m_hw->write_axi(m_axi_offset, xilinx_iic::CR, xilinx_iic::CR_EN|xilinx_iic::CR_TX_FIFO_Reset);  //! СБросим TX_FIFO
    m_hw->write_axi(m_axi_offset, xilinx_iic::CR, xilinx_iic::CR_EN);                               //! Разрешим работу контроллера
  }

  void disable() { m_hw->write_axi(m_axi_offset, xilinx_iic::CR, 0); }

  bool check_rx_fifo(size_t timeout = 500) {
    //! Ожидаем, готовности RX_FIFO
    ipc_time_t start = ipc_get_time();
    uint32_t status = 0;
    if(!m_hw->read_axi(m_axi_offset, xilinx_iic::SR, status))
      return false;
    while(status & xilinx_iic::STAT_RX_FIFO_Empty) {
        if(ipc_get_difftime(start, ipc_get_time()) >= timeout) {
            fprintf(stderr, "RX_FIFO not filled! Timeout exit.\n");
            return false;
        }
        if(!m_hw->read_axi(m_axi_offset, xilinx_iic::SR, status))
          return false;
    }
    return true;
  }

  bool check_tx_fifo_full(size_t timeout = 500) {
    //! Ожидаем, готовности TX_FIFO
    ipc_time_t start = ipc_get_time();
    uint32_t status = 0;
    if(!m_hw->read_axi(m_axi_offset, xilinx_iic::SR, status))
      return false;
    while(status & xilinx_iic::STAT_TX_FIFO_Full) {
        if(ipc_get_difftime(start, ipc_get_time()) >= timeout) {
            fprintf(stderr, "TX_FIFO full! Timeout exit.\n");
            return false;
        }
        if(!m_hw->read_axi(m_axi_offset, xilinx_iic::SR, status))
          return false;
    }
    return true;
  }

  bool check_tx_fifo_empty(size_t timeout = 500) {
    //! Ожидаем, готовности TX_FIFO
    ipc_time_t start = ipc_get_time();
    uint32_t isr_status = 0;
    if(!m_hw->read_axi(m_axi_offset, xilinx_iic::ISR, isr_status))
      return false;
    while(!(isr_status & xilinx_iic::ISR_TX_FIFO_EMPTY)) {
      if(ipc_get_difftime(start, ipc_get_time()) >= timeout) {
        fprintf(stderr, "TX_FIFO full! Timeout exit.\n");
          return false;
      }
      if(!m_hw->read_axi(m_axi_offset, xilinx_iic::ISR, isr_status))
        return false;
    }
    m_hw->write_axi(m_axi_offset, xilinx_iic::ISR, (isr_status | xilinx_iic::ISR_TX_FIFO_EMPTY));
    return true;
  }

  bool wait_bus_busy(size_t timeout = 500) {
    //! Ожидаем, готовности RX_FIFO
    ipc_time_t start = ipc_get_time();
    uint32_t status = 0;
    if(!m_hw->read_axi(m_axi_offset, xilinx_iic::SR, status))
      return false;
    while(status & xilinx_iic::STAT_BB) {
        if(ipc_get_difftime(start, ipc_get_time()) >= timeout) {
            uint32_t isr_status = 0;
            m_hw->read_axi(m_axi_offset, xilinx_iic::SR, isr_status);
            fprintf(stderr, "IIC bus busy! Timeout exit. SR: 0x%x ISR: 0x%x\n", status, isr_status);
            return false;
        }
    if(!m_hw->read_axi(m_axi_offset, xilinx_iic::SR, status))
      return false;
    }
    return true;
  }

  bool read_fifo(uint32_t& value, size_t timeout = 500) {
    //! Ожидаем, готовности RX_FIFO
    if(!check_rx_fifo(timeout))
        return false;
    //! Читаем очередной байт из RX_FIFO
    return m_hw->read_axi(m_axi_offset, xilinx_iic::RX_FIFO, value);
  }

  bool write_fifo(uint32_t val, size_t timeout = 500) {
    //! Ожидаем, готовности TX_FIFO
    if(!check_tx_fifo_full(timeout))
        return false;
    //! Пишем очередной байт в RX_FIFO
    return m_hw->write_axi(m_axi_offset, xilinx_iic::TX_FIFO, val);
  }

  bool read(const i2c_address i2c_addr, std::vector<uint8_t>& data, size_t N) {

      enable();

      //! Проверим состояние шины и FIFO
      if(!wait_bus_busy()) {
          return false;
      }

      //! Старт, адрес, если data.empty() то чтение, иначе - запишем данные из data, а потом читаем
      uint32_t fifo_start = (xilinx_iic::IIC_START | (i2c_addr << 1) | (data.empty() ? xilinx_iic::IIC_READ : xilinx_iic::IIC_WRITE));
      if(!write_fifo(fifo_start))
          return false;


      //! Запишем все входные данные в TX_FIFO
      for(size_t ii = 0; ii < data.size(); ++ii) {
          uint32_t fifo_data = data.at(ii);
          if(!write_fifo(fifo_data))
              return false;
      }

      //! Команда на чтение данных
      if(!data.empty()) {
          uint32_t fifo_data = (xilinx_iic::IIC_START | (i2c_addr << 1) | xilinx_iic::IIC_READ);
          if(!write_fifo(fifo_data))
              return false;
      }

      //! Стоп и число байт, которые необходимо получить
      uint32_t fifo_stop = (xilinx_iic::IIC_STOP | N);
      if(!write_fifo(fifo_stop))
          return false;

      //! Считаем N-байт данных из RX_FIFO
      data.clear();
      for(size_t ii = 0; ii < N; ++ii) {

          //! Ожидаем, готовности RX_FIFO
          uint32_t fifo_data = 0;
          if(!read_fifo(fifo_data))
              return false;

          uint8_t byte = static_cast<uint8_t>(fifo_data);
          data.push_back(byte);
      }

      return true;
  }

  bool write(const uint8_t i2c_addr, const std::vector<uint8_t>& data) {

    if(data.empty())
        return false;

    enable();

    //! Проверим состояние шины и FIFO
    if(!wait_bus_busy()) {
        return false;
    }

    //! Старт, адрес, запись
    uint32_t fifo_start = (xilinx_iic::IIC_START | (i2c_addr << 1) | xilinx_iic::IIC_WRITE);
    if(!write_fifo(fifo_start))
        return false;


    //! Запишем все данные в TX_FIFO, кроме последнего байта
    for(size_t ii = 0; ii < data.size() - 1; ++ii) {
        uint32_t fifo_data = data.at(ii);
        if(!write_fifo(fifo_data))
            return false;
    }

    //! Cтоп, последний байт данных
    uint32_t fifo_stop = (xilinx_iic::IIC_STOP | data.at(data.size()-1));
    if(!write_fifo(fifo_stop))
        return false;

    if(!check_tx_fifo_empty(500))
        return false;

    return true;
  }
};

class dev_i2c_mux : public detail::dev_i2c_base<dev_i2c_mux>,
                    public detail::dev_base_creator<dev_i2c_mux> {
  i2c_segment m_segment{};

 public:
  dev_i2c_mux() = default;
  dev_i2c_mux(axi_interface io, i2c_segment segment, i2c_address address,
              double frequency = double(400_kHz),
              i2c_addressing addressing = i2c_addressing::_7bit)
      : dev_i2c_base{io, address, frequency, addressing}, m_segment{segment} {}
  dev_i2c_mux(axi_interface io, parent_functor fn, i2c_segment segment,
              i2c_address address, double frequency = double(400_kHz),
              i2c_addressing addressing = i2c_addressing::_7bit)
      : dev_i2c_base{io, fn, address, frequency, addressing},
        m_segment{segment} {}
  size_t write(const i2c_data& data) final {
    assert(m_io.use_count() != 0 && m_io.get() != nullptr);
    call_parent();
    m_io->configure(m_address, m_frequency, m_addressing);
    return m_io->write(data);
  }
  size_t read(i2c_data& data, const size_t N) final {
    assert(m_io.use_count() != 0 && m_io.get() != nullptr);
    call_parent();
    m_io->configure(m_address, m_frequency, m_addressing);
    return m_io->read(data, N);
  }
  void operator()() { segment(); }
  virtual void segment() {
    call_parent();
    // TODO: добавить управление выбором сегмента
    // read / write
  }
};

class dev_i2c : public detail::dev_i2c_base<dev_i2c_mux>,
                public detail::dev_base_creator<dev_i2c> {
 public:
  dev_i2c() = default;
  dev_i2c(axi_interface io, i2c_address address,
          double frequency = double(400_kHz),
          i2c_addressing addressing = i2c_addressing::_7bit)
      : dev_i2c_base{io, address, frequency, addressing} {}
  dev_i2c(axi_interface io, parent_functor fn, i2c_address address,
          double frequency = double(400_kHz),
          i2c_addressing addressing = i2c_addressing::_7bit)
      : dev_i2c_base{io, fn, address, frequency, addressing} {}
  size_t write(const i2c_data& data) final {
    assert(m_io.use_count() != 0 && m_io.get() != nullptr);
    call_parent();
    m_io->configure(m_address, m_frequency, m_addressing);
    return m_io->write(data);
  }
  size_t read(i2c_data& data, const size_t N) final {
    assert(m_io.use_count() != 0 && m_io.get() != nullptr);
    call_parent();
    m_io->configure(m_address, m_frequency, m_addressing);
    return m_io->read(data, N);
  }
};

}  // namespace InSys
