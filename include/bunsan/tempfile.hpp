#ifndef BUNSAN_TEMPFILE_HPP
#define BUNSAN_TEMPFILE_HPP

#include <string>

#include <boost/filesystem/path.hpp>

namespace bunsan
{
    class tempfile
    {
    public:
        // constructors
        tempfile();
        explicit tempfile(const boost::filesystem::path &path_, bool do_auto_remove_=true);
        tempfile(const tempfile &tmp)=delete;
        tempfile(tempfile &&tmp) noexcept;
        // operator=
        tempfile &operator=(const tempfile &tmp)=delete;
        tempfile &operator=(tempfile &&tmp) noexcept;
        // path access
        const boost::filesystem::path &path() const;
        boost::filesystem::path::string_type native() const;
        std::string generic_string() const;
        std::string string() const;
        // auto_remove
        bool auto_remove() const;
        void auto_remove(bool do_auto_remove_);
        // destruction
        ~tempfile();
        // swap
        void swap(tempfile &tmp) noexcept;

        // static creators
        /// create tempfile in specified directory
        static tempfile in_dir(const boost::filesystem::path &dir);
        /// create tempfile in system temp dir
        static tempfile unique();
        /*!
         * \brief create tempfile in system temp dir with name specified by model
         *
         * \pre model.filename()==model
         */
        static tempfile unique(const boost::filesystem::path &model);
        /// create tempfile using model: % symbols will be replaced by random symbols [0-9a-f]
        static tempfile from_model(const boost::filesystem::path &model);
    private:
        static const boost::filesystem::path default_model;
        boost::filesystem::path m_path;
        bool do_auto_remove;
    };
    inline void swap(tempfile &a, tempfile &b) noexcept
    {
        a.swap(b);
    }
}

#endif //BUNSAN_TEMPFILE_HPP

