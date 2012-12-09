#pragma once

#include "bunsan/config/error.hpp"
#include "bunsan/config/traits.hpp"

#include <memory>
#include <vector>
#include <string>
#include <chrono>
#include <unordered_map>
#include <unordered_set>

#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/variant.hpp>
#include <boost/variant/static_visitor.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/nvp.hpp>

namespace bunsan{namespace config
{
    template <typename Ptree>
    class output_archive
    {
    public:
        typedef std::integral_constant<bool, false> is_loading;
        typedef std::integral_constant<bool, true> is_saving;

        inline unsigned int get_library_version() { return 0; }

    public:
        explicit output_archive(Ptree &ptree): ptree_(&ptree) {}

        template <typename T>
        output_archive &operator&(const T &obj)
        {
            return *this << obj;
        }

        template <typename T>
        output_archive &operator<<(const T &obj)
        {
            save(obj);
            return *this;
        }

        template <typename T>
        output_archive &operator<<(const boost::serialization::nvp<T> &nvp)
        {
            Ptree ptree;
            save_to_ptree(nvp.value(), ptree);
            ptree_->put_child(nvp.name(), ptree);
            return *this;
        }

        template <typename T>
        output_archive &operator<<(const boost::serialization::nvp<boost::optional<T>> &nvp)
        {
            if (nvp.value())
                *this << boost::serialization::make_nvp(nvp.name(), nvp.value().get());
            return *this;
        }

        template <typename T>
        static void save_to_ptree(const T &obj, Ptree &ptree)
        {
            output_archive<Ptree> oa(ptree);
            oa << obj;
        }

    private:
        /// For complex types.
        template <typename T>
        typename std::enable_if<!traits::direct_assign<T>::value, void>::type
        save(const T &obj)
        {
            const_cast<T &>(obj).serialize(*this, boost::serialization::version<T>::value);
            //FIXME boost::serialization::serialize_adl(*this, obj, boost::serialization::version<T>::value);
        }

        /// For primitive types.
        template <typename T>
        typename std::enable_if<traits::direct_assign<T>::value, void>::type
        save(const T &obj)
        {
            ptree_->put_value(obj);
        }

        /// For boost::filesystem::path.
        void save(const boost::filesystem::path &obj)
        {
            save(obj.string());
        }

        /// For std::chrono::duration.
        template <typename Rep, typename Period>
        void save(const std::chrono::duration<Rep, Period> &obj)
        {
            save(obj.count());
        }

        /// For general object.
        template <typename T>
        static typename Ptree::value_type to_value_type(const T &obj)
        {
            typename Ptree::value_type value;
            save_to_ptree(obj, value.second);
            return value;
        }

        /// For key-value pair.
        template <typename K, typename V>
        static typename Ptree::value_type to_value_type(const std::pair<K, V> &obj)
        {
            typename Ptree::value_type value = {
                boost::lexical_cast<std::string>(obj.first),
                Ptree()
            };
            save_to_ptree(obj.second, value.second);
            return value;
        }

        /// For iterable.
        template <typename T>
        void save_sequence(const T &seq)
        {
            ptree_->clear();
            for (const auto &value: seq)
            {
                ptree_->push_back(to_value_type(value));
            }
        }

        /// For std::unordered_map.
        template <typename Key, typename Tp, typename Hash, typename Pred, typename Alloc>
        void save(const std::unordered_map<Key, Tp, Hash, Pred, Alloc> &obj)
        {
            save_sequence(obj);
        }

        /// For std::unordered_set.
        template <typename Value, typename Hash, typename Pred, typename Alloc>
        void save(const std::unordered_set<Value, Hash, Pred, Alloc> &obj)
        {
            save_sequence(obj);
        }

        /// For std::vector.
        template <typename Tp, typename Alloc>
        void save(const std::vector<Tp, Alloc> &obj)
        {
            save_sequence(obj);
        }

        /// For boost::variant.
        template <BOOST_VARIANT_ENUM_PARAMS(typename T)>
        void save(const boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)> &obj);

    private:
        Ptree *const ptree_;
    };

    namespace output_archive_detail
    {
        template <typename Archive, typename Variant>
        class save_visitor: public boost::static_visitor<void>
        {
        public:
            explicit save_visitor(Archive &archive): archive_(&archive) {}
            save_visitor(const save_visitor<Archive, Variant> &)=default;
            save_visitor &operator=(const save_visitor<Archive, Variant> &)=default;

            template <typename T>
            void operator()(const T &obj) const
            {
                static_assert(traits::type_key<Variant, T>::call(), "Undefined type key.");
                *archive_ & boost::serialization::make_nvp(traits::type_key<Variant, T>::call(), obj);
            }

        private:
            Archive *archive_;
        };
    }

    template <typename Ptree>
    template <BOOST_VARIANT_ENUM_PARAMS(typename T)>
    void output_archive<Ptree>::save(const boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)> &obj)
    {
        typedef boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)> visitor_type;
        typedef output_archive_detail::save_visitor<output_archive<Ptree>, visitor_type> save_visitor;
        save_visitor visitor(*this);
        boost::apply_visitor(visitor, obj);
    }
}}
