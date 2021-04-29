#pragma once

#include "info_i2c.h"

namespace InSys {

// FIXME: черновой вариант, нужен рефакторинг

class i2c_units_tree : public units_tree {
  static constexpr auto k_i2c{"i2c"};
  static constexpr auto k_segments_num{"segs"};
  static constexpr auto k_segment{"seg"};
  static constexpr auto k_address{"addr"};
  static constexpr auto k_frequency{"freq"};

 public:
  using units_tree::units_tree;
  constexpr auto unit_name() const { return k_i2c; }
  auto has_segments() const {
    return m_units_tree.get_optional<uint32_t>(k_segments_num).has_value();
  }
  auto get_segments_num() const {
    return m_units_tree.get<uint32_t>(k_segments_num);
  }
  auto get_segment() const { return m_units_tree.get<uint32_t>(k_segment); }
  auto get_address() const {
    return std::strtol(m_units_tree.get<std::string>(k_address).c_str(),
                       nullptr, 16);
  }
  auto get_frequency() const { return m_units_tree.get<double>(k_frequency); }
};

class info_axi_i2c_parser : public info_base_parser<info_list, info_axi_i2c> {
 protected:
  void parser(const units_tree &units_tree) override {
    for (auto &units : units_tree) {
      auto units_tree = i2c_units_tree{std::move(units.second)};
      auto name = units_tree.get_name();
      auto label = units_tree.get_label();
      auto offset = units_tree.get_offset();
      if (name == units_tree.unit_name()) {
        m_info_list.emplace_back(name, label, offset);
      }
    }
  }
  void parser(const std::string_view &config) override {
    parser(i2c_units_tree{config}.get_units());
  }

 public:
  info_axi_i2c_parser() = default;
  info_axi_i2c_parser(const std::string_view &config) { parser(config); }
};

class info_i2c_mux_parser : public info_base_parser<info_list, info_i2c_mux> {
 protected:
  void parser(const units_tree &units_tree) override {
    for (auto &units : units_tree) {
      auto units_tree = i2c_units_tree{std::move(units.second)};
      auto parent_name = units_tree.get_name();
      auto parent_label = units_tree.get_label();
      auto paren_offset = units_tree.get_offset();
      if (parent_name == units_tree.unit_name()) {
        auto i2c_tree = units_tree.get_units();
        for (auto &i2c : i2c_tree) {
          auto i2c_tree = i2c_units_tree{std::move(i2c.second)};
          if (!i2c_tree.has_segments()) {
            continue;
          }
          auto segments = i2c_tree.get_segments_num();
          auto name = i2c_tree.get_name();
          auto label = i2c_tree.get_label();
          auto addr = i2c_tree.get_address();
          auto freq = i2c_tree.get_frequency();
          m_info_list.emplace_back(name, label, addr, freq, segments,
                                   make_info_uid{}(parent_name, parent_label));
        }
      }
    }
  }
  void parser(const std::string_view &config) override {
    parser(i2c_units_tree{config}.get_units());
  }

 public:
  info_i2c_mux_parser() = default;
  info_i2c_mux_parser(const std::string_view &config) { parser(config); }
};

class info_i2c_dev_parser : public info_base_parser<info_list, info_i2c_dev> {
 protected:
  void parser(const units_tree &units_tree) override {
    for (auto &units : units_tree) {
      auto units_tree = i2c_units_tree{std::move(units.second)};
      auto parent_name = units_tree.get_name();
      auto parent_label = units_tree.get_label();
      auto paren_offset = units_tree.get_offset();
      if (parent_name == units_tree.unit_name()) {
        auto i2c_tree = units_tree.get_units();
        for (auto &i2c : i2c_tree) {
          auto i2c_tree = i2c_units_tree{std::move(i2c.second)};
          auto name = i2c_tree.get_name();
          auto label = i2c_tree.get_label();
          auto addr = i2c_tree.get_address();
          auto freq = i2c_tree.get_frequency();
          if (i2c_tree.has_segments()) {
            auto segments = i2c_tree.get_segments_num();
            auto i2c_subtree = i2c_tree.get_units();
            for (auto &i2c_sub : i2c_subtree) {
              auto i2c_sub_tree = i2c_units_tree{std::move(i2c_sub.second)};
              // FIXME: нужна рекурсивность
              auto child_name = i2c_sub_tree.get_name();
              auto child_label = i2c_sub_tree.get_label();
              auto child_addr = i2c_sub_tree.get_address();
              auto child_freq = i2c_sub_tree.get_frequency();
              auto segment = i2c_sub_tree.get_segment();
              m_info_list.emplace_back(
                  child_name, child_label, child_addr, child_freq,
                  make_info_uid{}(name, label, std::to_string(segment)));
            }
          } else {
            m_info_list.emplace_back(
                name, label, addr, freq,
                make_info_uid{}(parent_name, parent_label));
          }
        }
      }
    }
  }
  void parser(const std::string_view &config) override {
    parser(i2c_units_tree{config}.get_units());
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
    auto units_tree = i2c_units_tree{config}.get_units();
    axi_parser::parser(units_tree);
    mux_parser::parser(units_tree);
    dev_parser::parser(units_tree);
  }
};

}  // namespace InSys
