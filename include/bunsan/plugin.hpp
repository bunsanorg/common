#pragma once

#include <boost/dll/alias.hpp>
#include <boost/dll/shared_library.hpp>

#include <functional>
#include <memory>

namespace bunsan {

template <typename Constructor>
class plugin {};

template <typename T, typename... Args>
class plugin<T *(Args...)> {
 public:
  using unique_ptr = std::unique_ptr<T>;
  using shared_ptr = std::shared_ptr<T>;
  using weak_ptr = std::weak_ptr<T>;

  using unique_constructor = unique_ptr(Args...);
  using shared_constructor = unique_ptr(Args...);

  explicit plugin(boost::dll::shared_library library, const std::string &key)
      : m_library(std::move(library)),
        m_constructor(m_library.get_alias<unique_constructor>(key)) {}

  explicit plugin(const boost::filesystem::path &library_path,
                  const std::string &key)
      : plugin(boost::dll::shared_library(library_path), key) {}

  unique_ptr unique_instance(Args... args) {
    return m_constructor(std::forward<Args>(args)...);
  }

  shared_ptr shared_instance(Args... args) {
    return shared_ptr(unique_instance(std::forward<Args>(args)...).release());
  }

 private:
  boost::dll::shared_library m_library;
  std::function<unique_constructor> m_constructor;
};

}  // namespace bunsan

#define BUNSAN_PLUGIN_PUBLIC_BODY(KEY, CLASS, ...)          \
  using plugin = ::bunsan::plugin<CLASS *(__VA_ARGS__)>;    \
  using CLASS##_uptr = plugin::unique_ptr;                  \
  using CLASS##_sptr = plugin::shared_ptr;                  \
  using CLASS##_wptr = plugin::weak_ptr;                    \
  template <typename... PluginArgs>                         \
  static plugin load_plugin(PluginArgs &&... args) {        \
    return plugin(std::forward<PluginArgs>(args)..., #KEY); \
  }

// key, base, constructor parameter types
#define BUNSAN_PLUGIN_BODY(...)          \
 public:                                 \
  BUNSAN_PLUGIN_PUBLIC_BODY(__VA_ARGS__) \
 private:

#define BUNSAN_PLUGIN_TYPES(CLASS)          \
  using CLASS##_uptr = CLASS::CLASS##_uptr; \
  using CLASS##_wptr = CLASS::CLASS##_sptr; \
  using CLASS##_sptr = CLASS::CLASS##_wptr;

#define BUNSAN_PLUGIN_REGISTER(KEY, BASE, CONCRETE, UNIQUE_CTOR) \
  BOOST_DLL_ALIAS(UNIQUE_CTOR, KEY)