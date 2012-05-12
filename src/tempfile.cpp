#include "bunsan/tempfile.hpp"
#include "bunsan/util.hpp"

#include <boost/filesystem/operations.hpp>

const boost::filesystem::path bunsan::tempfile::default_model = "%%%%-%%%%-%%%%-%%%%";

// static creators

bunsan::tempfile bunsan::tempfile::in_dir(const boost::filesystem::path &dir)
{
    return from_model(dir/default_model);
}

bunsan::tempfile bunsan::tempfile::from_model(const boost::filesystem::path &model)
{
    return tempfile(boost::filesystem::unique_path(model));
}

bunsan::tempfile bunsan::tempfile::unique()
{
    return unique(default_model);
}

bunsan::tempfile bunsan::tempfile::unique(const boost::filesystem::path &model)
{
    BOOST_ASSERT(model==model.filename());
    return from_model(boost::filesystem::temp_directory_path()/model);
}

// constructors

bunsan::tempfile::tempfile(): do_auto_remove(false) {}

bunsan::tempfile::tempfile(const boost::filesystem::path &path_, bool do_auto_remove_):
    m_path(path_), do_auto_remove(do_auto_remove_) {}

bunsan::tempfile::tempfile(tempfile &&tmp) throw()
{
    swap(tmp);
}

bunsan::tempfile &bunsan::tempfile::operator=(tempfile &&tmp) throw()
{
    swap(tmp);
    return *this;
}

// swap

void bunsan::tempfile::swap(tempfile &tmp) throw()
{
    using boost::swap;
    swap(m_path, tmp.m_path);
    swap(do_auto_remove, tmp.do_auto_remove);
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
    return do_auto_remove;
}

void bunsan::tempfile::auto_remove(bool do_auto_remove_)
{
    do_auto_remove = do_auto_remove_;
}

bunsan::tempfile::~tempfile()
{
    if (do_auto_remove)
    {
        try
        {
            SLOG("trying to remove tempfile "<<m_path);
            if (boost::filesystem::exists(m_path))
                boost::filesystem::remove_all(m_path);
        }
        catch(std::exception &e)
        {
            DLOG(error);
            SLOG(e.what());
            SLOG("unable to remove tempfile "<<m_path);
        }
        catch(...)
        {
            DLOG(unknown error);
            SLOG("unable to remove tempfile "<<m_path);
        }
    }
    else
    {
        SLOG("skipping tempfile remove "<<m_path);
    }
}

