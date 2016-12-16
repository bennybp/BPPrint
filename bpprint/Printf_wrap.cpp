#include <string>
#include <cstring>
#include <stdexcept>
#include <typeinfo>

#include "bpprint/Printf_wrap.hpp"


namespace bpprint {
namespace detail {

// Mapping of basic types to their printf specifiers
template<typename T> struct PFTypeMap { };

#define DECLARE_PFTYPE(t, cast, length, pft) template<> struct PFTypeMap<t> { \
         static constexpr const char * pflength = length; \
         static constexpr const char * pftype = pft; \
         typedef cast cast_type; \
       };


DECLARE_PFTYPE(bool,               int,                 "",    "d")
DECLARE_PFTYPE(char,               char,                "",    "c")

DECLARE_PFTYPE(signed char,        signed char,         "hh",  "d")
DECLARE_PFTYPE(signed short,       signed short,        "h",   "d")
DECLARE_PFTYPE(signed int,         signed int,          "",    "d")
DECLARE_PFTYPE(signed long,        signed long,         "l",   "d")
DECLARE_PFTYPE(signed long long,   signed long long,    "ll",  "d")
DECLARE_PFTYPE(unsigned char,      unsigned char,       "hh",  "uoxX")
DECLARE_PFTYPE(unsigned short,     unsigned short,      "h",   "uoxX")
DECLARE_PFTYPE(unsigned int,       unsigned int,        "",    "uoxX")
DECLARE_PFTYPE(unsigned long,      unsigned long,       "l",   "uoxX")
DECLARE_PFTYPE(unsigned long long, unsigned long long,  "ll",  "uoxX")

DECLARE_PFTYPE(float,            double, "",  "fFeEaAgG")
DECLARE_PFTYPE(double,           double, "",  "fFeEaAgG")
DECLARE_PFTYPE(long double, long double, "L", "fFeEaAgG")

DECLARE_PFTYPE(const char *, const char *, "", "s")
DECLARE_PFTYPE(char *,       char *,       "", "s")
DECLARE_PFTYPE(std::string,  std::string,  "", "s")

DECLARE_PFTYPE(const void *, const void *, "", "p")
DECLARE_PFTYPE(void *,       void *,       "", "p")




/*! \brief Handles substitution of a single specifier
 *
 * This takes a string containing a single format specifier
 * and substitutes in the value. By this point, the type
 * should already have been checked against the type specifier
 * of the format.
 *
 * \throw std::runtime_error If there is a problem with the substitution
 *
 * \tparam T The type of data to substitute with
 *
 * \param [inout] fmt String with a single format specifier. Will
 *                    be replaced with the formatted string
 * \param [in] subst What to put in place of the specifier
 */
template<typename T>
void handle_fmt_single_(std::string & fmt, T subst)
{
    static const int bufsize = 256;

    // should be fine for most substitutions
    char buf[bufsize];

    // Try to write to the buffer
    const int n = snprintf(buf, bufsize, fmt.c_str(), subst);

    // If the return value is >= bufsize, then the buffer wasn't
    // big enough. Then we use heap allocation
    if(n >= bufsize)
    {
        const int neededsize = n+1; // includes null termination

        char * hbuf = new char[static_cast<size_t>(neededsize)];

        const int n2 = snprintf(hbuf, static_cast<size_t>(neededsize),
                                fmt.c_str(), subst);

        // these two conditions signal success
        if(n2 >= 0 && n2 < neededsize)
            fmt = std::string(hbuf); // copies to the output

        delete [] hbuf;

        if(n2 < 0 || n2 >= neededsize)
            throw std::runtime_error(std::string("Error here: ") +
                                     std::to_string(n2));
    }
    else
        fmt = std::string(buf);
}



template<typename T>
void handle_fmt_(std::string & fmt,
                 const char * length,
                 char spec, T subst)
{
    typedef typename std::remove_reference<T>::type noref_T; 
    typedef typename std::remove_cv<noref_T>::type nocv_T; 
    typedef typename std::decay<nocv_T>::type actual_T;

    typedef typename PFTypeMap<actual_T>::cast_type cast_type;
    const char * pftype  = PFTypeMap<actual_T>::pftype;
    const char * pflength = PFTypeMap<actual_T>::pflength;
    const char * mangled_type = typeid(actual_T).name();


    fmt.reserve(fmt.size() + 5);

    if(strlen(length) == 0 && spec == '?') // auto deduction
    {
        fmt += pflength;
        fmt += pftype[0];  // first type = default
    }
    else
    {
        // given a length and type spec, is it valid?
        if(strcmp(length, pflength) != 0)
        {
            std::string errstr = "Bad length specifier ";
            errstr += length;
            errstr += " for type ";
            errstr += mangled_type;
            throw std::runtime_error(errstr);
        }

        // see if spec occurs in pftype
        if(strchr(pftype, spec) == nullptr)
        {
            std::string errstr = "Bad type specifier ";
            errstr += spec;
            errstr += " for type ";
            errstr += mangled_type;
            throw std::runtime_error(errstr);
        }

        fmt += length;
        fmt += spec;
    }

    handle_fmt_single_(fmt, static_cast<cast_type>(subst));
}


// const char * , since we don't always want it to be %s
// (ie, we might want it passed to %p)
void handle_fmt_(std::string & fmt, const char * length,
                 char spec, const char * subst)
{
    if(spec == 's' || spec == '?')
        handle_fmt_<const char *>(fmt, length, spec, subst);
    else
        handle_fmt_<void const *>(fmt, length, spec, subst);
}


// char * , since we don't always want it to be %s
// (ie, we might want it passed to %p)
void handle_fmt_(std::string & fmt, const char * length,
                 char spec, char * subst)
{
    handle_fmt_(fmt, length, spec, static_cast<const char *>(subst));
}


// std::string - for convenience
void handle_fmt_(std::string & fmt, const char * length,
                 char spec, const std::string & subst)
{
    handle_fmt_(fmt, length, spec, subst.c_str());
}



/////////////////////////////////////////
// Explicitly instantiate the templates
// of handle_fmt_
/////////////////////////////////////////
#define DECLARE_TEMPLATE_FORMAT(type) \
       template void handle_fmt_<type>(std::string &, const char *, char, type);

DECLARE_TEMPLATE_FORMAT(bool)
DECLARE_TEMPLATE_FORMAT(char)
DECLARE_TEMPLATE_FORMAT(unsigned char)
DECLARE_TEMPLATE_FORMAT(signed char)
DECLARE_TEMPLATE_FORMAT(unsigned short)
DECLARE_TEMPLATE_FORMAT(signed short)
DECLARE_TEMPLATE_FORMAT(unsigned int)
DECLARE_TEMPLATE_FORMAT(signed int)
DECLARE_TEMPLATE_FORMAT(unsigned long)
DECLARE_TEMPLATE_FORMAT(signed long)
DECLARE_TEMPLATE_FORMAT(unsigned long long)
DECLARE_TEMPLATE_FORMAT(signed long long)

DECLARE_TEMPLATE_FORMAT(float)
DECLARE_TEMPLATE_FORMAT(double)
DECLARE_TEMPLATE_FORMAT(long double)

DECLARE_TEMPLATE_FORMAT(void const *)

#undef DECLARE_TEMPLATE_FORMAT

} // close namespace detail
} // close namespace bpprint

