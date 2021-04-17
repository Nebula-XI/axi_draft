#pragma once

#include "units_info_base.h"

namespace InSys {

class unit_info_axi_i2c final : public unit_info_axi_base {
  inline static constexpr auto k_unit{"axi-i2c"};

 public:
  using list_type = units_info_list<unit_info_axi_i2c>;
  unit_info_axi_i2c() : unit_info_axi_base{k_unit} {}
  unit_info_axi_i2c(const std::string_view &name, const std::string_view &label,
                    unit_info_uid axi_offset)
      : unit_info_axi_base{name, label, axi_offset, k_unit} {}
};

class unit_info_i2c_dev_base : public unit_info_base {
  uint32_t m_address{};
  double m_frequency{};

 public:
  unit_info_i2c_dev_base(const std::string_view &unit)
      : unit_info_base{unit}, m_address{}, m_frequency{} {}
  unit_info_i2c_dev_base(const std::string_view &name,
                         const std::string_view &label, uint32_t address,
                         double frequency, unit_info_uid parent_uid,
                         const std::string_view &unit)
      : unit_info_base{name, label, parent_uid, unit},
        m_address{address},
        m_frequency{frequency} {}
  auto address() const noexcept { return m_address; }
  auto frequency() const noexcept { return m_frequency; }
};

class unit_info_i2c_dev final : public unit_info_i2c_dev_base {
  inline static constexpr auto k_unit{"i2c-dev"};

 public:
  using list_type = units_info_list<unit_info_i2c_dev>;
  unit_info_i2c_dev() : unit_info_i2c_dev_base{k_unit} {}
  unit_info_i2c_dev(const std::string_view &name, const std::string_view &label,
                    uint32_t address, double frequency,
                    unit_info_uid parent_uid)
      : unit_info_i2c_dev_base{name,      label,      address,
                               frequency, parent_uid, k_unit} {}
};

class unit_info_i2c_mux final : public unit_info_i2c_dev_base {
  inline static constexpr auto k_unit{"i2c-mux"};

 public:
  using list_type = units_info_list<unit_info_i2c_mux>;
  using segments_map = std::map<unit_info_uid, uint32_t, std::less<uint32_t>>;
  unit_info_i2c_mux() : unit_info_i2c_dev_base{k_unit}, m_segments{} {}
  unit_info_i2c_mux(const std::string_view &name, const std::string_view &label,
                    uint32_t address, double frequency, uint32_t ports,
                    unit_info_uid parent_uid)
      : unit_info_i2c_dev_base{name,      label,      address,
                               frequency, parent_uid, k_unit} {
    for (decltype(ports) port{}; port < ports; ++port) {
      m_segments.emplace(
          make_unit_info_uid{}(name, label, std::to_string(port)), port);
    }
  }
  auto &segments() const noexcept { return m_segments; }

 private:
  segments_map m_segments{};
};

class unit_info_axi_i2c_parser
    : public units_info_base_parser<units_info_list, unit_info_axi_i2c> {
  void parser(const std::string_view &config) override {
    // TODO: add configuration parser
    m_info_list.emplace_back("I2C", "PORT0", 0x00001000);
    m_info_list.emplace_back("I2C", "PORT1", 0x00002000);
    m_info_list.emplace_back("I2C", "PORT2", 0x00003000);
  }

 public:
  unit_info_axi_i2c_parser() = default;
  unit_info_axi_i2c_parser(const std::string_view &config) { parser(config); }
};

class unit_info_i2c_mux_parser
    : public units_info_base_parser<units_info_list, unit_info_i2c_mux> {
  void parser(const std::string_view &config) override {
    // TODO: add configuration parser
    m_info_list.emplace_back("LTC4356", "DD12", 0x44, 200_kHz, 8,
                             make_unit_info_uid{}("I2C", "PORT0"));
    m_info_list.emplace_back("LTC4306", "DD45", 0x44, 200_kHz, 8,
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
    m_info_list.emplace_back("INA218", "DD1", 0x44, 200_kHz,
                             make_unit_info_uid{}("I2C", "PORT0"));
    m_info_list.emplace_back("LTC2991", "DD2", 0x45, 200_kHz,
                             make_unit_info_uid{}("I2C", "PORT1"));
    m_info_list.emplace_back("INA218", "DD3", 0x49, 200_kHz,
                             make_unit_info_uid{}("I2C", "PORT2"));
    m_info_list.emplace_back(
        "INA218", "DD8", 0x49, 200_kHz,
        make_unit_info_uid{}("LTC4356", "DD12", std::to_string(3)));
    m_info_list.emplace_back(
        "INA218", "DD79", 0x49, 200_kHz,
        make_unit_info_uid{}("LTC4356", "DD12", std::to_string(1)));
  }

 public:
  unit_info_i2c_dev_parser() = default;
  unit_info_i2c_dev_parser(const std::string_view &config) { parser(config); }
};

class unit_info_i2c_parser final : public unit_info_axi_i2c_parser,
                                   public unit_info_i2c_dev_parser,
                                   public unit_info_i2c_mux_parser {
 public:
  using axi_parser = unit_info_axi_i2c_parser;
  using dev_parser = unit_info_i2c_dev_parser;
  using mux_parser = unit_info_i2c_mux_parser;

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
  std::optional<typename parser::value_type> get_by_label(
      const std::string_view &label) const {
    return parser::get_by_label(label);
  }
  template <typename parser>
  std::optional<typename parser::value_type> get_by_uid(
      unit_info_uid uid) const {
    return parser::get_by_uid(uid);
  }

 private:
  void parser(const std::string_view &config) final {
    // TODO: add configuration parser
    axi_parser::m_info_list.emplace_back("I2C", "PORT0", 0x00001000);
    axi_parser::m_info_list.emplace_back("I2C", "PORT1", 0x00002000);
    axi_parser::m_info_list.emplace_back("I2C", "PORT2", 0x00003000);
    mux_parser::m_info_list.emplace_back("LTC4356", "DD12", 0x44, 200_kHz, 8,
                                         make_unit_info_uid{}("I2C", "PORT0"));
    mux_parser::m_info_list.emplace_back(
        "LTC4306", "DD45", 0x44, 200_kHz, 8,
        make_unit_info_uid{}("LTC4356", "DD12"));
    dev_parser::m_info_list.emplace_back("INA218", "DD1", 0x44, 200_kHz,
                                         make_unit_info_uid{}("I2C", "PORT0"));
    dev_parser::m_info_list.emplace_back("LTC2991", "DD2", 0x45, 200_kHz,
                                         make_unit_info_uid{}("I2C", "PORT1"));
    dev_parser::m_info_list.emplace_back("INA218", "DD3", 0x49, 200_kHz,
                                         make_unit_info_uid{}("I2C", "PORT2"));
    dev_parser::m_info_list.emplace_back(
        "INA218", "DD8", 0x49, 200_kHz,
        make_unit_info_uid{}("LTC4356", "DD12", std::to_string(3)));
    dev_parser::m_info_list.emplace_back(
        "INA218", "DD79", 0x49, 200_kHz,
        make_unit_info_uid{}("LTC4356", "DD12", std::to_string(1)));
  }
};

}  // namespace InSys
