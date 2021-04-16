#pragma once

#include "units_info_base.h"

namespace InSys {

struct unit_info_axi_i2c : unit_info_axi_base {
  using list = units_info_list<unit_info_axi_i2c>;
  unit_info_axi_i2c(const std::string_view &_name,
                    const std::string_view &_label, unit_info_uid _axi_offset)
      : unit_info_axi_base{_name, _label, _axi_offset, "axi-i2c"} {}
};

struct unit_info_i2c_dev_base : unit_info_base {
  unit_info_i2c_dev_base(const std::string_view &_name,
                         const std::string_view &_label, uint32_t _address,
                         double _frequency, unit_info_uid _parent_uid,
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
                    double _frequency, unit_info_uid _parent_uid)
      : unit_info_i2c_dev_base{_name,      _label,      _address,
                               _frequency, _parent_uid, "i2c-dev"} {}
};

struct unit_info_i2c_mux : unit_info_i2c_dev_base {
  using list = units_info_list<unit_info_i2c_mux>;
  using segments_map = std::map<unit_info_uid, uint32_t, std::less<uint32_t>>;
  unit_info_i2c_mux(const std::string_view &_name,
                    const std::string_view &_label, uint32_t _address,
                    double _frequency, uint32_t ports,
                    unit_info_uid _parent_uid)
      : unit_info_i2c_dev_base{_name,      _label,      _address,
                               _frequency, _parent_uid, "i2c-mux"} {
    for (decltype(ports) port{}; port < ports; ++port) {
      segments.emplace(make_unit_info_uid{}(name, label, std::to_string(port)),
                       port);
    }
  }
  segments_map segments{};
};

class unit_info_axi_i2c_parser
    : public units_info_base_parser<units_info_list, unit_info_axi_i2c> {
  void parser(const std::string_view &config) override {
    // TODO: add configuration parser
    _info_list.emplace_back("I2C", "PORT0", 0x00001000);
    _info_list.emplace_back("I2C", "PORT1", 0x00002000);
    _info_list.emplace_back("I2C", "PORT2", 0x00003000);
  }

 public:
  unit_info_axi_i2c_parser() = default;
  unit_info_axi_i2c_parser(const std::string_view &config) { parser(config); }
};

class unit_info_i2c_mux_parser
    : public units_info_base_parser<units_info_list, unit_info_i2c_mux> {
  void parser(const std::string_view &config) override {
    // TODO: add configuration parser
    _info_list.emplace_back("LTC4356", "DD12", 0x44, 200_kHz, 8,
                            make_unit_info_uid{}("I2C", "PORT0"));
    _info_list.emplace_back("LTC4306", "DD45", 0x44, 200_kHz, 8,
                            make_unit_info_uid{}("LTC4356", "DD12"));
  }

 public:
  unit_info_i2c_mux_parser() = default;
  unit_info_i2c_mux_parser(const std::string_view &config) { parser(config); }
};

class unit_info_i2c_dev_parser
    : public units_info_base_parser<units_info_list, unit_info_i2c_dev> {
  void parser(const std::string_view &config) override {
    // TODO: add configuration parser
    _info_list.emplace_back("INA218", "DD1", 0x44, 200_kHz,
                            make_unit_info_uid{}("I2C", "PORT0"));
    _info_list.emplace_back("LTC2991", "DD2", 0x45, 200_kHz,
                            make_unit_info_uid{}("I2C", "PORT1"));
    _info_list.emplace_back("INA218", "DD3", 0x49, 200_kHz,
                            make_unit_info_uid{}("I2C", "PORT2"));
    _info_list.emplace_back(
        "INA218", "DD8", 0x49, 200_kHz,
        make_unit_info_uid{}("LTC4356", "DD12", std::to_string(3)));
  }

 public:
  unit_info_i2c_dev_parser() = default;
  unit_info_i2c_dev_parser(const std::string_view &config) { parser(config); }
};

class unit_info_i2c_parser : public unit_info_axi_i2c_parser,
                             public unit_info_i2c_dev_parser,
                             public unit_info_i2c_mux_parser {
 public:
  using axi_i2c_parser = unit_info_axi_i2c_parser;
  using i2c_dev_parser = unit_info_i2c_dev_parser;
  using i2c_mux_parser = unit_info_i2c_mux_parser;

  unit_info_i2c_parser(const std::string_view &config) { parser(config); }
  template <typename parser>
  typename parser::list_type get_info() const {
    return parser::get_info();
  }
  template <typename parser>
  typename parser::list_type find_by_name(const std::string_view &name) const {
    return parser::find_by_name(name);
  }
  template <typename parser>
  std::optional<typename parser::value_type> get_by_uid(
      unit_info_uid uid) const {
    return parser::get_by_uid(uid);
  }

 private:
  void parser(const std::string_view &config) override {
    // TODO: add configuration parser
    axi_i2c_parser::_info_list.emplace_back("I2C", "PORT0", 0x00001000);
    axi_i2c_parser::_info_list.emplace_back("I2C", "PORT1", 0x00002000);
    axi_i2c_parser::_info_list.emplace_back("I2C", "PORT2", 0x00003000);
    i2c_mux_parser::_info_list.emplace_back(
        "LTC4356", "DD12", 0x44, 200_kHz, 8,
        make_unit_info_uid{}("I2C", "PORT0"));
    i2c_mux_parser::_info_list.emplace_back(
        "LTC4306", "DD45", 0x44, 200_kHz, 8,
        make_unit_info_uid{}("LTC4356", "DD12"));
    i2c_dev_parser::_info_list.emplace_back(
        "INA218", "DD1", 0x44, 200_kHz, make_unit_info_uid{}("I2C", "PORT0"));
    i2c_dev_parser::_info_list.emplace_back(
        "LTC2991", "DD2", 0x45, 200_kHz, make_unit_info_uid{}("I2C", "PORT1"));
    i2c_dev_parser::_info_list.emplace_back(
        "INA218", "DD3", 0x49, 200_kHz, make_unit_info_uid{}("I2C", "PORT2"));
    i2c_dev_parser::_info_list.emplace_back(
        "INA218", "DD8", 0x49, 200_kHz,
        make_unit_info_uid{}("LTC4356", "DD12", std::to_string(3)));
  }
};

}  // namespace InSys
