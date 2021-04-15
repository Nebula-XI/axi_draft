#pragma once

#include "units_info_base.h"

namespace InSys {

struct unit_info_axi_i2c : unit_info_axi_base {
  using list = units_info_list<unit_info_axi_i2c>;
  unit_info_axi_i2c(const std::string_view &_name, std::size_t _axi_offset)
      : unit_info_axi_base{_name, {}, _axi_offset, "axi-i2c"} {}
};

struct unit_info_i2c_dev_base : unit_info_base {
  unit_info_i2c_dev_base(const std::string_view &_name,
                         const std::string_view &_label, uint32_t _address,
                         double _frequency, std::size_t _parent_uid,
                         const std::string_view &_unit)
      : unit_info_base{_name, _label, _parent_uid, _unit},
        address{_address},
        frequency{_frequency} {}
  const uint32_t address{};
  const double frequency{};
};

struct unit_info_i2c_dev : unit_info_i2c_dev_base {
  using list = units_info_list<unit_info_i2c_dev>;
  unit_info_i2c_dev(const std::string_view &_name,
                    const std::string_view &_label, uint32_t _address,
                    double _frequency, std::size_t _parent_uid)
      : unit_info_i2c_dev_base{_name,      _label,      _address,
                               _frequency, _parent_uid, "i2c-dev"} {}
};

struct unit_info_i2c_mux : unit_info_i2c_dev_base {
  using list = units_info_list<unit_info_i2c_mux>;
  using segments_map = std::map<std::size_t, uint32_t>;
  unit_info_i2c_mux(const std::string_view &_name,
                    const std::string_view &_label, uint32_t _address,
                    double _frequency, uint32_t ports, std::size_t _parent_uid)
      : unit_info_i2c_dev_base{_name,      _label,      _address,
                               _frequency, _parent_uid, "i2c-mux"} {
    for (decltype(ports) port{}; port < ports; ++port) {
      segments.emplace(make_units_info_uid{}(name, label, port), port);
    }
  }
  segments_map segments{};
};

class unit_info_axi_i2c_parser
    : public units_info_base<units_info_list, unit_info_axi_i2c> {
  void parser(const std::string_view &config) override {
    // TODO: add configuration parser
    _info_list.emplace_back("I2C0", 0x00001000);
    _info_list.emplace_back("I2C1", 0x00002000);
    _info_list.emplace_back("I2C2", 0x00003000);
  }

 public:
  unit_info_axi_i2c_parser(const std::string_view &config)
      : units_info_base{config} {
    parser(config);
  }
};

class unit_info_i2c_mux_parser
    : public units_info_base<units_info_list, unit_info_i2c_mux> {
  void parser(const std::string_view &config) override {
    // TODO: add configuration parser
    _info_list.emplace_back("LTC4306", "DD12", 0x44, 200_kHz, 8,
                            make_units_info_uid{}("I2C0"));
    _info_list.emplace_back("LTC4306", "DD45", 0x44, 200_kHz, 8,
                            make_units_info_uid{}("LTC4306", "DD12"));
  }

 public:
  unit_info_i2c_mux_parser(const std::string_view &config)
      : units_info_base{config} {
    parser(config);
  }
};

class unit_info_i2c_dev_parser
    : public units_info_base<units_info_list, unit_info_i2c_dev> {
  void parser(const std::string_view &config) override {
    // TODO: add configuration parser
    _info_list.emplace_back("INA218", "DD1", 0x44, 200_kHz,
                            make_units_info_uid{}("I2C0"));
    _info_list.emplace_back("LTC2991", "DD2", 0x45, 200_kHz,
                            make_units_info_uid{}("I2C0"));
    _info_list.emplace_back("INA218", "DD3", 0x49, 200_kHz,
                            make_units_info_uid{}("I2C1"));
    _info_list.emplace_back("INA218", "DD8", 0x49, 200_kHz,
                            make_units_info_uid{}("LTC4306", "DD12"));
  }

 public:
  unit_info_i2c_dev_parser(const std::string_view &config)
      : units_info_base{config} {
    parser(config);
  }
};

}  // namespace InSys
