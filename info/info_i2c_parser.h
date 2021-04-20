#pragma once

#include "info_i2c.h"

namespace InSys {

class info_axi_i2c_parser : public info_base_parser<info_list, info_axi_i2c> {
  void parser(const std::string_view &config) override {
    // TODO: add configuration parser
    m_info_list.emplace_back("I2C", "PORT0", 0x00001000);
    m_info_list.emplace_back("I2C", "PORT1", 0x00002000);
    m_info_list.emplace_back("I2C", "PORT2", 0x00003000);
  }

 public:
  info_axi_i2c_parser() = default;
  info_axi_i2c_parser(const std::string_view &config) { parser(config); }
};

class info_i2c_mux_parser : public info_base_parser<info_list, info_i2c_mux> {
  void parser(const std::string_view &config) override {
    // TODO: add configuration parser
    m_info_list.emplace_back("LTC4356", "DD12", 0x44, 200_kHz, 8,
                             make_info_uid{}("I2C", "PORT0"));
    m_info_list.emplace_back("LTC4306", "DD45", 0x44, 200_kHz, 8,
                             make_info_uid{}("LTC4356", "DD12"));
  }

 public:
  info_i2c_mux_parser() = default;
  info_i2c_mux_parser(const std::string_view &config) { parser(config); }
};

class info_i2c_dev_parser : public info_base_parser<info_list, info_i2c_dev> {
  void parser(const std::string_view &config) override {
    // TODO: add configuration parser
    m_info_list.emplace_back("INA218", "DD1", 0x44, 200_kHz,
                             make_info_uid{}("I2C", "PORT0"));
    m_info_list.emplace_back("LTC2991", "DD2", 0x45, 200_kHz,
                             make_info_uid{}("I2C", "PORT1"));
    m_info_list.emplace_back("INA218", "DD3", 0x49, 200_kHz,
                             make_info_uid{}("I2C", "PORT2"));
    m_info_list.emplace_back(
        "INA218", "DD8", 0x49, 200_kHz,
        make_info_uid{}("LTC4356", "DD12", std::to_string(3)));
    m_info_list.emplace_back(
        "INA218", "DD79", 0x49, 200_kHz,
        make_info_uid{}("LTC4356", "DD12", std::to_string(1)));
  }

 public:
  info_i2c_dev_parser() = default;
  info_i2c_dev_parser(const std::string_view &config) { parser(config); }
};

class info_i2c_parser final : public info_axi_i2c_parser,
                              public info_i2c_dev_parser,
                              public info_i2c_mux_parser {
 public:
  using axi_parser = info_axi_i2c_parser;
  using dev_parser = info_i2c_dev_parser;
  using mux_parser = info_i2c_mux_parser;

  info_i2c_parser(const std::string_view &config) { parser(config); }
  template <typename parser>
  typename parser::list_type get_info() const {
    return parser::get_info();
  }
  template <typename parser>
  typename parser::list_type find_by_name(const std::string_view &name) const {
    return parser::find_by_name(name);
  }
  template <typename parser>
  std::optional<typename parser::value_type> get_by_label(
      const std::string_view &label) const {
    return parser::get_by_label(label);
  }
  template <typename parser>
  std::optional<typename parser::value_type> get_by_uid(info_uid uid) const {
    return parser::get_by_uid(uid);
  }

 private:
  void parser(const std::string_view &config) final {
    // TODO: add configuration parser
    axi_parser::m_info_list.emplace_back("I2C", "PORT0", 0x00001000);
    axi_parser::m_info_list.emplace_back("I2C", "PORT1", 0x00002000);
    axi_parser::m_info_list.emplace_back("I2C", "PORT2", 0x00003000);
    mux_parser::m_info_list.emplace_back("LTC4356", "DD12", 0x44, 200_kHz, 8,
                                         make_info_uid{}("I2C", "PORT0"));
    mux_parser::m_info_list.emplace_back("LTC4306", "DD45", 0x44, 200_kHz, 8,
                                         make_info_uid{}("LTC4356", "DD12"));
    dev_parser::m_info_list.emplace_back("INA218", "DD1", 0x44, 200_kHz,
                                         make_info_uid{}("I2C", "PORT0"));
    dev_parser::m_info_list.emplace_back("LTC2991", "DD2", 0x45, 200_kHz,
                                         make_info_uid{}("I2C", "PORT1"));
    dev_parser::m_info_list.emplace_back("INA218", "DD3", 0x49, 200_kHz,
                                         make_info_uid{}("I2C", "PORT2"));
    dev_parser::m_info_list.emplace_back(
        "INA218", "DD8", 0x49, 200_kHz,
        make_info_uid{}("LTC4356", "DD12", std::to_string(3)));
    dev_parser::m_info_list.emplace_back(
        "INA218", "DD79", 0x49, 200_kHz,
        make_info_uid{}("LTC4356", "DD12", std::to_string(1)));
  }
};

}  // namespace InSys
