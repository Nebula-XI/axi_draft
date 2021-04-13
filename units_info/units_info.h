#pragma once

#include <algorithm>
#include <any>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <utility>
#include <vector>

namespace InSys {

/// функция формирования уникального идентификатора узла
inline auto units_info_make_id(const std::string &name) noexcept {
  return std::hash<std::string>{}(name);
}

/// базовая информация присущая всем узлам
struct unit_info_base {
  unit_info_base() = delete;  //< контруктор по умолчанию
  unit_info_base(const std::string &_name, std::size_t _axi_offset)
      : id{units_info_make_id(_name)},
        name{_name},
        axi_offset{_axi_offset} {}  //< размещающий
                                    //конструктор
  virtual ~unit_info_base() noexcept = default;
  const std::size_t id{};  //< уникальный идентификатор узла
  const std::size_t unit_id{};  //< уникальный идентификатор узла
  const std::string name{};  //< имя узла
  const std::size_t axi_offset{};  //< смещение в адресном пространстве
};

/// параметры узла I2C
struct unit_info_i2c : unit_info_base {
  unit_info_i2c() = delete;  //< контруктор по умолчанию
  unit_info_i2c(const std::string &_name, std::size_t _axi_offset, int _address,
                double _frequency)
      : unit_info_base{_name, _axi_offset},
        address{_address},
        frequency{_frequency} {}  //< размещающий
                                  //конструктор
  const int address{};            //< I2C адрес
  const double frequency{};       //< I2C частота
  inline static const char unit[]{"i2c"};
};

/// параметры узла SPI
struct unit_info_spi : unit_info_base {
  unit_info_spi() = delete;  //< контруктор по умолчанию
  unit_info_spi(const std::string &_name, std::size_t _axi_offset,
                int _chip_select)
      : unit_info_base{_name, _axi_offset},
        chip_select{_chip_select} {}  //< размещающий
                                      //конструктор
  const int chip_select{};            //< выбор кристала
  inline static const char unit[]{"spi"};
};

/// параметры узла GPIO
struct unit_info_gpio : unit_info_base {
  unit_info_gpio() = delete;  //< контруктор по умолчанию
  unit_info_gpio(const std::string &_name, std::size_t _axi_offset)
      : unit_info_base{_name, _axi_offset} {}  //< размещающий
                                               //конструктор
  inline static const char unit[]{"gpio"};
};

/// список параметров узлов I2C
using units_info_i2c_list = std::vector<unit_info_i2c>;
/// список параметров узлов SPI
using units_info_spi_list = std::vector<unit_info_spi>;
/// список параметров узлов GPIO
using units_info_gpio_list = std::vector<unit_info_gpio>;
using unit_info_elem = std::any;
// список параметров любых узлов
using units_info_list = std::vector<unit_info_elem>;

/// класс формирователь информации об устройствах
class units_info final {
  units_info_i2c_list _i2c_info_list{};
  units_info_spi_list _spi_info_list{};
  units_info_gpio_list _gpio_info_list{};

 public:
  /// конструктор
  units_info(const std::string &config) {
    // TODO: add configuration parser
    _i2c_info_list.emplace_back("INA218", 0x00000200, 0x48,
                                200'000);  // FIXME: пример
    _i2c_info_list.emplace_back("LTC2991", 0x00000400, 0x53,
                                200'000);  // FIXME: пример
    _spi_info_list.emplace_back("LMX2594", 0x00000600,
                                1);  // FIXME: пример
    _spi_info_list.emplace_back("LTC6953", 0x00000800,
                                2);                     // FIXME: пример
    _gpio_info_list.emplace_back("GPIO0", 0x00000800);  // FIXME: пример
    _gpio_info_list.emplace_back("GPIO1", 0x00001000);  // FIXME: пример
  }
  units_info_i2c_list get_i2c_info() const { return _i2c_info_list; }
  units_info_spi_list get_spi_info() const { return _spi_info_list; }
  units_info_gpio_list get_gpio_info() const { return _gpio_info_list; }
  units_info_list get_info() const {
    units_info_list info_list{};
    for (const auto &i2c_info : _i2c_info_list) {
      info_list.emplace_back(i2c_info);
    }
    for (const auto &spi_info : _spi_info_list) {
      info_list.emplace_back(spi_info);
    }
    for (const auto &gpio_info : _gpio_info_list) {
      info_list.emplace_back(gpio_info);
    }
    return info_list;
  }
  units_info_i2c_list find_i2c_info(std::size_t id) const {
    units_info_i2c_list i2c_info_list{};
    for (const auto &i2c_info : _i2c_info_list) {
      if (i2c_info.id != id) {
        continue;
      }
      i2c_info_list.push_back(i2c_info);
    }
    return i2c_info_list;
  }
  units_info_spi_list find_spi_info(std::size_t id) const {
    units_info_spi_list spi_info_list{};
    for (const auto &spi_info : _spi_info_list) {
      if (spi_info.id != id) {
        continue;
      }
      spi_info_list.push_back(spi_info);
    }
    return spi_info_list;
  }
  units_info_gpio_list find_gpio_info(std::size_t id) const {
    units_info_gpio_list gpio_info_list{};
    for (const auto &gpio_info : _gpio_info_list) {
      if (gpio_info.id != id) {
        continue;
      }
      gpio_info_list.push_back(gpio_info);
    }
    return gpio_info_list;
  }
  units_info_list find_info(std::size_t id) const {
    units_info_list info_list{};
    auto i2c_info_list = find_i2c_info(id);
    for (const auto &i2c_info : i2c_info_list) {
      info_list.emplace_back(i2c_info);
    }
    auto spi_info_list = find_spi_info(id);
    for (const auto &spi_info : spi_info_list) {
      info_list.emplace_back(spi_info);
    }
    auto gpio_info_list = find_gpio_info(id);
    for (const auto &gpio_info : gpio_info_list) {
      info_list.emplace_back(gpio_info);
    }
    return info_list;
  }
};

template <typename ValueType>
inline ValueType unit_info_cast(const unit_info_elem &unit_info) {
  return std::any_cast<ValueType>(unit_info);
}

}  // namespace InSys
