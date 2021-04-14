#pragma once

#include "units_info_base.h"

namespace InSys {

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
  inline static const std::string unit{"i2c"};
  std::string to_config() const {
    std::stringstream config{};
    config << unit_info_base::to_config();
    config << "unit = " << unit << '\n';
    config << "addr = 0x" << std::hex << address << '\n';
    config << "freq = " << std::dec << frequency << '\n';
    return config.str();
  }
};

/// список параметров узлов I2C
using units_info_i2c_list = units_info_list<unit_info_i2c>;

/// класс формирователь информации об узлах I2C
class units_info_i2c : public units_info_base<units_info_list, unit_info_i2c> {
  void parser(const std::string config) override {
    // TODO: add configuration parser
    _info_list.emplace_back("INA218", 0x00000100, 0x32,
                            200_kHz);  // FIXME: пример
    _info_list.emplace_back("INA218", 0x00000200, 0x48,
                            200_kHz);  // FIXME: пример
    _info_list.emplace_back("LTC2991", 0x00000400, 0x53,
                            200_kHz);  // FIXME: пример
  }

 public:
  /// конструктор
  units_info_i2c(const std::string &config) : units_info_base{config} {
    parser(config);
  }
};

}  // namespace InSys
