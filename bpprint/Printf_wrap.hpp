#pragma once

#include <string>


namespace bpprint {
namespace detail {


/*! \brief Prepare and check a decomposed format
 *
 * This checks the type against the type specifier in the
 * format string (that has been decomposed into its pieces).
 *
 * \throw std::runtime_error If there is a problem with the substitution,
 *        such as if the type specification or length specification
 *        does not match the data type passed in.
 *
 * \tparam T The type of data to substitute with
 *
 * \param [in] fmt The format specifier without the length or type specifiers
 * \param [in] length The length specifier
 * \param [in] spec The type specifier
 * \param [in] subst What to put in place of the specifier
 */
template<typename T>
void handle_fmt_(std::string & fmt, const char * length,
                 char spec, T subst);


/*! \brief Prepare and check a decomposed format
 *
 * Overload for pointers, which are always passwd as `void *`
 */
template<typename T>
void handle_fmt_(std::string & fmt, const char * length,
                 char spec, T * subst)
{
    return handle_fmt_<void const *>(fmt, length, spec, subst);
}


/*! \brief Prepare and check a decomposed format
 *
 * Overload for `char *`, since we may not always want it
 * to be used as a string (ie, %p)
 */
void handle_fmt_(std::string & fmt, const char * length,
                 char spec, const char * subst);


/*! \brief Prepare and check a decomposed format
 *
 * Overload for `char *`, since we may not always want it
 * to be used as a string (ie, %p)
 */
void handle_fmt_(std::string & fmt, const char * length,
                 char spec, char * subst);


/*! \brief Prepare and check a decomposed format
 *
 * Overload for `std::string`, so we can pass it to %s
 */
void handle_fmt_(std::string & fmt, const char * length,
                 char spec, const std::string & subst);



/////////////////////////////////////////////////////////
// Explicitly instantiate handle_fmt_
// template function for all valid types. Also, mark them
// as valid by deriving their ValidPrintfArg structure
// from std:true_type
/////////////////////////////////////////////////////////

template<typename T> struct ValidPrintfArg : public std::false_type { };

#define DECLARE_VALID_FORMAT(type) \
    extern template void handle_fmt_<type>(std::string &, const char *, char, type); \
    template<> struct ValidPrintfArg<type> : public std::true_type { };

DECLARE_VALID_FORMAT(bool)
DECLARE_VALID_FORMAT(char)
DECLARE_VALID_FORMAT(unsigned char)
DECLARE_VALID_FORMAT(signed char)
DECLARE_VALID_FORMAT(unsigned short)
DECLARE_VALID_FORMAT(signed short)
DECLARE_VALID_FORMAT(unsigned int)
DECLARE_VALID_FORMAT(signed int)
DECLARE_VALID_FORMAT(unsigned long)
DECLARE_VALID_FORMAT(signed long)
DECLARE_VALID_FORMAT(unsigned long long)
DECLARE_VALID_FORMAT(signed long long)

DECLARE_VALID_FORMAT(float)
DECLARE_VALID_FORMAT(double)
DECLARE_VALID_FORMAT(long double)

DECLARE_VALID_FORMAT(void const *)

#undef DECLARE_VALID_FORMAT


// Mark the other special types as valid as well
template<> struct ValidPrintfArg<std::string> : public std::true_type { };
template<> struct ValidPrintfArg<char *> : public std::true_type { };
template<> struct ValidPrintfArg<const char *> : public std::true_type { };
template<typename T> struct ValidPrintfArg<T *> : public std::true_type { };


} // close namespace detail
} // close namespace bpprint

