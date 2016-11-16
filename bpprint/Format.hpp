#pragma once

#include <sstream>
#include <stdexcept>

#include "bpprint/Printf_wrap.hpp"

namespace bpprint {
namespace detail {


/*! \brief Information about a single format specification
 *
 * This stores a piece of a format string (such as "%d" or "%12.8e"),
 * as well as what came before it and what comes after it in the
 * format string
 */
struct FormatInfo
{
    //! The string before the format specification
    std::string prefix;

    //! The string after the format specification
    std::string suffix;

    //! The format specification itself, except for
    //    the length and type specifier characters
    std::string format;

    //! The length specifier
    char length[3];

    //! The type specifier character
    char spec;
};


/*! \brief Get the next format specification
 *
 * If the function returns true, the members of \p fi are filled in.
 *
 * If the function returns false, the prefix member contains the entire
 * string but with some processing (such as replacing %% with %).
 *
 * \throw std::runtime_error if the format string is badly formatted
 *
 * \param [out] fi Information about the specification
 * \param [in] str The string to search
 * \return True if a format specification was found, otherwise false
 */
bool get_next_format_(FormatInfo & fi, const std::string & str);


/*! \brief Format a string into an ostream
 *
 * Used to terminate the variadic template
 *
 * \throw std::runtime_error if the string contains a format
 *        specification (meaning it is expecting an argument)
 *
 * \param [in] os The ostream used to build the string
 * \param [in] fi Format info to use as a workspace
 * \param [in] str String (possibly with format string specification)
 */
void format_(std::ostream & os, FormatInfo & fi, const std::string & str);


/*! \brief Format a string into an ostream
 *
 * This will only format the first specification found in
 * \p str, using \p arg as the substitution
 *
 * \throw std::runtime_error if the correct number of arguments is not given or
 *        if the format string is badly formed
 *
 * \param [in] os The ostream to output to
 * \param [in] fi The format information struct to use
 * \param [in] str String (possibly with format string specification)
 * \param [in] arg Substitution for the first format specification found
 * \param [in] args Additional arguments for later format specifications
 */
template<typename T, typename... Targs>
void format_(std::ostream & os, FormatInfo & fi,
             const std::string & str, T arg, Targs... args)
{
    // just in case
    typedef typename std::remove_cv<T>::type nocv_T;
    typedef typename std::decay<nocv_T>::type decay_T;
    typedef typename std::remove_reference<decay_T>::type actual_T;

    static_assert(ValidPrintfArg<actual_T>::value == true,
                  "Invalid argument type passed to Format");

    // Obtain the next format in the string.
    // If there aren't any, that is a problem - we were passed
    // more arguments!

    if(get_next_format_(fi, str))
    {
        // after this, fi.format has been overwritten
        handle_fmt_(fi.format, fi.length, fi.spec, arg);

        os << fi.prefix << fi.format;
        format_(os, fi, fi.suffix, args...);
    }
    else
        throw std::runtime_error("Too many arguments to format string");
}

} // close namespace detail



/* \brief Apply formatting to a string, outputting it to an ostream
 *
 * \throw std::runtime_error if the correct number of arguments is not given or
 *        if the format string is badly formed
 *
 * \param [in] os The ostream to output to
 * \param [in] fmt The format string
 * \param [in] args Arguments to the format string
 */
template<typename... Targs>
void format_stream(std::ostream & os, const std::string & fmt, Targs... args)
{
    detail::FormatInfo fi;

    // Reserve space in the strings
    // These are just guesses, and should hold most substitutions
    fi.prefix.reserve(64);
    fi.suffix.reserve(64);
    fi.format.reserve(16);

    detail::format_(os, fi, fmt, args...);
}



/* \brief Apply formatting to a string
 *
 * \throw std::runtime_error if the correct number of arguments is not given or
 *        if the format string is badly formed
 */
template<typename... Targs>
std::string format_string(const std::string & str, Targs... args)
{
    std::stringstream ss;
    format_stream(ss, str, args...);
    return ss.str();
}



} // close namespace bpprint

