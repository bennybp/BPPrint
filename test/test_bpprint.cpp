#include <bpprint/Format.hpp>
#include <iostream>

#if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wformat-security"
#endif


template<typename... Targs>
void test_string(const std::string & fmt, Targs... args)
{
    char refstr[1024];
    snprintf(refstr, 1024, fmt.c_str(), args...);

    std::string bpstr = bpprint::format_string(fmt, args...);

    std::cout << "Format string: " << fmt << "\n";
    std::cout << "    Reference output: " << refstr << "\n";
    std::cout << "      BPPrint output: " << bpstr << "\n";

    if(std::string(refstr) != bpstr)
        throw std::runtime_error("!!!!! MISMATCHED OUTPUT !!!!!\n");
}

template<typename... Targs>
void test_format(const std::string & fmt, Targs &&... args)
{
    test_string(fmt, args...);
    test_string(fmt + " ", args...);
    test_string(std::string(" ") + fmt, args...);
    test_string(fmt + "\n", args...);
    test_string(std::string("\n") + fmt, args...);
    test_string(fmt + "\t", args...);
    test_string(std::string("\t") + fmt, args...);

    // tests the border between stack and heap allocation
    for(unsigned int i = 250; i < 260; i++)
        test_string(std::string(i, '@') + fmt, args...);
}


int main(void)
{
    try {
        test_format("%s", "Hello");

        test_format("%0s", "Hello");
        test_format("%-0s", "Hello");
        test_format("%1s", "Hello");
        test_format("%-1s", "Hello");
        test_format("%2s", "Hello");
        test_format("%-2s", "Hello");

        // escapes
        test_format("%%");
        test_format("%%?");
        test_format("%%%d", 5);
        //test_format("%%%?", 5);

        

    }
    catch(std::exception & ex)
    {
        std::cout << "Test failed: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
