#pragma once

#include "units_info_base.h"

namespace InSys {

struct i2c_parent_switch {
  i2c_parent_switch(std::size_t _uid = 0, int _port = -1)
      : uid{_uid}, port{_port} {}
  const std::size_t uid{};
  const int port{-1};
  const bool is_present{bool(uid)};
};

/// параметры узла I2C
struct unit_info_i2c : unit_info_base {
  unit_info_i2c() = delete;  //< контруктор по умолчанию
  unit_info_i2c(const std::string &_name, const std::string &_label,
                std::size_t _axi_offset, int _address, double _frequency,
                const i2c_parent_switch &_parent_switch = i2c_parent_switch{})
      : unit_info_base{_name, _label, _axi_offset},
        address{_address},
        frequency{_frequency},
        parent_switch{_parent_switch} {}  //< размещающий конструктор
  const int address{};                    //< I2C адрес
  const double frequency{};               //< I2C частота
  const i2c_parent_switch parent_switch{};  //< I2C switch
  inline static const std::string unit{"i2c"};
};

/// список параметров узлов I2C
using units_info_i2c_list = units_info_list<unit_info_i2c>;

/// класс формирователь информации об узлах I2C
class units_info_i2c_parser
    : public units_info_base<units_info_list, unit_info_i2c> {
  void parser(const std::string config) override {
    // TODO: add configuration parser
    _info_list.emplace_back("TCA9548A", "DD12", 0x00000020, 0x15,
                            200_kHz);  // FIXME: пример
    auto tca9548a_id = get_info_by_offset(0x00000020).value().uid;
    _info_list.emplace_back(
        "TCA9548A", "DD43", 0x00000040, 0x47, 200_kHz,
        i2c_parent_switch{tca9548a_id, 2});  // FIXME: пример
    tca9548a_id = get_info_by_offset(0x00000040).value().uid;
    _info_list.emplace_back("INA218", "DD5", 0x00000100, 0x32,
                            200_kHz);  // FIXME: пример
    _info_list.emplace_back(
        "INA218", "DD6", 0x00000200, 0x48, 200_kHz,
        i2c_parent_switch{tca9548a_id, 0});  // FIXME: пример
    _info_list.emplace_back(
        "LTC2991", "DD7", 0x00000400, 0x53, 200_kHz,
        i2c_parent_switch{tca9548a_id, 7});  // FIXME: пример
  }

 public:
  /// конструктор
  units_info_i2c_parser(const std::string &config) : units_info_base{config} {
    parser(config);
  }
};

}  // namespace InSys
