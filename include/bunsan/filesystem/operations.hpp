#pragma once

#include <bunsan/config.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/system/system_error.hpp>

namespace bunsan {
namespace filesystem {

/*!
 * \brief recreate directory
 */
void reset_dir(const boost::filesystem::path &dir);

/*!
 * This function makes path relative to root,
 * but keeps it inside root.
 *
 * \code{.cpp}
 * assert(keep_in_root("../../etc/passwd", "/home/user") ==
 *"/home/user/etc/passwd");
 * assert(keep_in_root("../../etc/../passwd", "/home/user") ==
 *"/home/user/passwd");
 * \endcode
 */
boost::filesystem::path keep_in_root(const boost::filesystem::path &path,
                                     const boost::filesystem::path &root);

using boost::filesystem::copy_option;

void copy_tree(const boost::filesystem::path &from,
               const boost::filesystem::path &to);

void copy_tree(const boost::filesystem::path &from,
               const boost::filesystem::path &to,
               boost::system::error_code &ec);

void copy_tree(const boost::filesystem::path &from,
               const boost::filesystem::path &to,
               const BOOST_SCOPED_ENUM(copy_option) option);

void copy_tree(const boost::filesystem::path &from,
               const boost::filesystem::path &to,
               const BOOST_SCOPED_ENUM(copy_option) option,
               boost::system::error_code &ec);

}  // namespace filesystem
}  // namespace bunsan
