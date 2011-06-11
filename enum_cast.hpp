#ifndef ENUM_CAST_HPP
#define ENUM_CAST_HPP

namespace bunsan
{
	/*!
	 * \brief cast value to T enum type, if it is impossible error argument will be returned
	 */
	template <typename T, typename I>
	T enum_cast(I value, T error)
	{
		if (static_cast<I>(static_cast<T>(value))==value)
			return static_cast<T>(value);
		else
			return error;
	}
}

#endif //ENUM_CAST_HPP

