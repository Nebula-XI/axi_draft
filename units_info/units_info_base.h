#pragma once

#include <algorithm>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace InSys {

using unit_info_uid = std::size_t;
class make_unit_info_uid final {
  std::string m_str{};
  unit_info_uid operator()() { return std::hash<std::string>{}(m_str); }

 public:
  template <typename Arg = std::string, typename... Args>
  unit_info_uid operator()(Arg &&str, Args... other_str) {
    m_str += std::forward<Arg>(str);
    return operator()(other_str...);
  }
};

class unit_info_base {
  unit_info_uid m_uid{};
  std::string m_name{};
  std::string m_label{};
  unit_info_uid m_parent_uid{};
  std::string m_unit{};

 public:
  unit_info_base() = delete;
  unit_info_base(const std::string_view &unit)
      : m_uid{}, m_name{}, m_label{}, m_parent_uid{}, m_unit{unit} {}
  unit_info_base(const std::string_view &name, const std::string_view &label,
                 unit_info_uid parent_uid, const std::string_view &unit)
      : m_uid{make_unit_info_uid{}(name, label)},
        m_name{name},
        m_label{label},
        m_parent_uid{parent_uid},
        m_unit{unit} {}
  unit_info_base(const unit_info_base &other) = default;
  unit_info_base(unit_info_base &&) noexcept = default;
  unit_info_base &operator=(const unit_info_base &) = default;
  unit_info_base &operator=(unit_info_base &&) noexcept = default;
  virtual ~unit_info_base() noexcept = default;
  auto uid() const noexcept { return m_uid; }
  auto &name() const noexcept { return m_name; }
  auto &label() const noexcept { return m_label; }
  auto parent_uid() const noexcept { return m_parent_uid; }
  auto &unit() const noexcept { return m_unit; }
};

class unit_info_axi_base : public unit_info_base {
  unit_info_uid m_axi_offset{};

 public:
  unit_info_axi_base(const std::string_view &unit)
      : unit_info_base{unit}, m_axi_offset{} {}
  unit_info_axi_base(const std::string_view &name,
                     const std::string_view &label, unit_info_uid axi_offset,
                     const std::string_view &unit)
      : unit_info_base{name, label, {}, unit}, m_axi_offset{axi_offset} {}
  auto axi_offset() const noexcept { return m_axi_offset; }
};

template <typename unit_info_type>
using units_info_list = std::vector<unit_info_type>;

template <template <typename> typename units_info_list, typename unit_info_type>
class units_info_base_parser {
 public:
  using list_type = units_info_list<unit_info_type>;
  using value_type = unit_info_type;
  units_info_base_parser() = default;
  virtual ~units_info_base_parser() noexcept = default;
  list_type get_info() const { return m_info_list; }
  list_type find_by_name(const std::string_view &name) const {
    list_type info_list{};
    for (const auto &info : m_info_list) {
      if (info.name() == name) {
        info_list.push_back(info);
      }
    }
    return info_list;
  }
  std::optional<value_type> get_by_label(const std::string_view &label) const {
    std::optional<value_type> result{};
    for (const auto &info : m_info_list) {
      if (info.label() == label) {
        result.emplace(info);
      }
    }
    return result;
  }
  std::optional<value_type> get_by_uid(unit_info_uid uid) const {
    std::optional<value_type> result{};
    for (const auto &info : m_info_list) {
      if (info.uid() == uid) {
        result.emplace(info);
      }
    }
    return result;
  }

 protected:
  list_type m_info_list{};
  virtual void parser(const std::string_view &config) = 0;
};

constexpr long double operator"" _kHz(long double value) {
  return value * 1000;
}
constexpr long double operator"" _MHz(long double value) {
  return value * 1000000;
}
constexpr long double operator"" _GHz(long double value) {
  return value * 1000000000;
}

constexpr unsigned long long operator"" _kHz(unsigned long long value) {
  return value * 1000;
}
constexpr unsigned long long operator"" _MHz(unsigned long long value) {
  return value * 1000000;
}
constexpr unsigned long long operator"" _GHz(unsigned long long value) {
  return value * 1000000000;
}

}  // namespace InSys
