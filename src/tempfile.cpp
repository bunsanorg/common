#include <bunsan/tempfile.hpp>

#include <bunsan/filesystem/error.hpp>
#include <bunsan/logging/trivial.hpp>

#include <boost/filesystem/operations.hpp>
#include <boost/scope_exit.hpp>
#include <boost/utility/swap.hpp>

#include <cstdio>

const boost::filesystem::path bunsan::tempfile::default_model =
    "%%%%-%%%%-%%%%-%%%%";

// static creators

bunsan::tempfile bunsan::tempfile::in_dir(
    const boost::filesystem::path &dir)
{
    return from_model(dir / default_model);
}

bunsan::tempfile bunsan::tempfile::from_model(
    const boost::filesystem::path &model)
{
    return tempfile(boost::filesystem::unique_path(model));
}

bunsan::tempfile bunsan::tempfile::unique()
{
    return unique(default_model);
}

bunsan::tempfile bunsan::tempfile::unique(
    const boost::filesystem::path &model)
{
    BOOST_ASSERT(model == model.filename());
    return from_model(boost::filesystem::temp_directory_path() / model);
}

// constructors

bunsan::tempfile::tempfile(): m_do_auto_remove(false) {}

bunsan::tempfile::tempfile(const boost::filesystem::path &path,
                           bool do_auto_remove):
    m_path(path), m_do_auto_remove(do_auto_remove) {}

bunsan::tempfile::tempfile(tempfile &&tmp) noexcept
{
    swap(tmp);
}

bunsan::tempfile &bunsan::tempfile::operator=(tempfile &&tmp) noexcept
{
    swap(tmp);
    return *this;
}

// swap

void bunsan::tempfile::swap(tempfile &tmp) noexcept
{
    using boost::swap;
    swap(m_path, tmp.m_path);
    swap(m_do_auto_remove, tmp.m_do_auto_remove);
}

// path accessors

const boost::filesystem::path &bunsan::tempfile::path() const
{
    return m_path;
}

boost::filesystem::path::string_type bunsan::tempfile::native() const
{
    return m_path.native();
}

std::string bunsan::tempfile::generic_string() const
{
    return m_path.generic_string();
}

std::string bunsan::tempfile::string() const
{
    return m_path.string();
}

bool bunsan::tempfile::auto_remove() const
{
    return m_do_auto_remove;
}

void bunsan::tempfile::auto_remove(bool do_auto_remove)
{
    m_do_auto_remove = do_auto_remove;
}

bunsan::tempfile::~tempfile()
{
    if (m_do_auto_remove && !m_path.empty())
    {
        try
        {
            BUNSAN_LOG_TRACE << "Removing tempfile " << m_path;
            if (boost::filesystem::exists(m_path))
                boost::filesystem::remove_all(m_path);
        }
        catch(std::exception &e)
        {
            BUNSAN_LOG_ERROR << "Unable to remove tempfile " <<
                               m_path << ": " << e.what();
        }
        catch(...)
        {
            BUNSAN_LOG_ERROR << "Unable to remove tempfile " <<
                               m_path << ": unknown";
        }
    }
}

#define BUNSAN_TEMPFILE_OBJECT(TYPE) \
    bunsan::tempfile bunsan::tempfile::TYPE##_in_directory( \
        const boost::filesystem::path &directory) \
    { \
        return TYPE##_from_model(directory / default_model); \
    } \
    bunsan::tempfile bunsan::tempfile::TYPE##_in_tempdir() \
    { \
        return TYPE##_in_directory( \
            boost::filesystem::temp_directory_path()); \
    } \
    bunsan::tempfile bunsan::tempfile::TYPE##_in_tempdir( \
        const boost::filesystem::path &model) \
    { \
        return TYPE##_from_model( \
            boost::filesystem::temp_directory_path() / model); \
    } \
    bunsan::tempfile bunsan::tempfile::TYPE##_from_model( \
        const boost::filesystem::path &model) \
    { \
        constexpr std::size_t TRIES = 32; \
        for (std::size_t i = 0; i < TRIES; ++i) \
        { \
            tempfile tmp(boost::filesystem::unique_path(model)); \
            if (create_##TYPE(tmp.path())) \
            { \
                return tmp; \
            } \
            else \
            { \
                tmp.auto_remove(false); \
            } \
        } \
        BOOST_THROW_EXCEPTION( \
            unable_to_create_unique_temp_##TYPE() << \
            unable_to_create_unique_temp_##TYPE::tries(TRIES) << \
            unable_to_create_unique_temp_##TYPE::model(model)); \
    }

BUNSAN_TEMPFILE_OBJECT(regular_file)
BUNSAN_TEMPFILE_OBJECT(directory)

bool bunsan::tempfile::create_regular_file(
    const boost::filesystem::path &path)
{
    FILE *file = nullptr;
    BOOST_SCOPE_EXIT_ALL(&file)
    {
        if (file)
            fclose(file);
    };
    file = fopen(path.string().c_str(), "wx");
    if (file)
    {
        return true;
    }
    else
    {
        if (errno == EEXIST)
            return false;
        else
            BOOST_THROW_EXCEPTION(filesystem::system_error("fopen"));
    }
}

bool bunsan::tempfile::create_directory(
    const boost::filesystem::path &path)
{
    return boost::filesystem::create_directory(path);
}
