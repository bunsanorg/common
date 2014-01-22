#pragma once

#include <bunsan/error.hpp>

#include <boost/filesystem/path.hpp>

#include <string>

namespace bunsan
{
    struct tempfile_error: virtual error
    {
        typedef boost::error_info<struct tag_model, boost::filesystem::path> model;
    };
    struct tempfile_create_error: virtual tempfile_error
    {
        typedef boost::error_info<struct tag_tries, std::size_t> tries;
    };
    struct unable_to_create_unique_temp_directory: virtual tempfile_create_error {};
    struct unable_to_create_unique_temp_regular_file: virtual tempfile_create_error {};

    /*!
     * \note Model is pattern with '%' symbols
     * that are replaced by random symbols from [0-9a-f]
     */
    class tempfile
    {
    public:
        tempfile();

        /// Captures existing filesystem object
        explicit tempfile(const boost::filesystem::path &path_, bool do_auto_remove_=true);
        tempfile(const tempfile &tmp)=delete;
        tempfile(tempfile &&tmp) noexcept;

        tempfile &operator=(const tempfile &tmp)=delete;
        tempfile &operator=(tempfile &&tmp) noexcept;

        // path access
        const boost::filesystem::path &path() const;
        boost::filesystem::path::string_type native() const;
        std::string generic_string() const;
        std::string string() const;

        bool auto_remove() const;
        void auto_remove(bool do_auto_remove_);

        ~tempfile();

        // swap
        void swap(tempfile &tmp) noexcept;

    public:
        static tempfile regular_file_in_directory(const boost::filesystem::path &directory);
        static tempfile directory_in_directory(const boost::filesystem::path &directory);

        static tempfile regular_file_in_tempdir();
        static tempfile directory_in_tempdir();

        static tempfile regular_file_in_tempdir(const boost::filesystem::path &model);
        static tempfile directory_in_tempdir(const boost::filesystem::path &model);

        static tempfile regular_file_from_model(const boost::filesystem::path &model);
        static tempfile directory_from_model(const boost::filesystem::path &model);

    private:
        /// \return false if exists
        static bool create_regular_file(const boost::filesystem::path &path);

        /// \return false if exists
        static bool create_directory(const boost::filesystem::path &path);

    public:
        // DEPRECATED

        /// create tempfile in specified directory
        static tempfile in_dir(const boost::filesystem::path &dir);

        /// create tempfile in system temp dir
        static tempfile unique();

        /*!
         * \brief create tempfile in system temp dir with name specified by model
         *
         * \pre model.filename() == model
         */
        static tempfile unique(const boost::filesystem::path &model);

        /// create tempfile using model: % symbols will be replaced by random symbols [0-9a-f]
        static tempfile from_model(const boost::filesystem::path &model);

    private:
        static const boost::filesystem::path default_model;

    private:
        boost::filesystem::path m_path;
        bool do_auto_remove;
    };
    inline void swap(tempfile &a, tempfile &b) noexcept
    {
        a.swap(b);
    }
}
