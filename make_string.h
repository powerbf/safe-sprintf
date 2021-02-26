/*
   An alternative to sprintf that:
   a) won't core dump when given the wrong number or type of args
   b) treats a null char* as empty string
   c) allows std::string as an arg
*/

/* Copyright 2021 by Brian Power

   Permission is hereby granted, free of charge, to any person
   obtaining a copy of this software and associated documentation files
   (the "Software"), to deal in the Software without restriction,
   including without limitation the rights to use, copy, modify, merge,
   publish, distribute, sublicense, and/or sell copies of the Software,
   and to permit persons to whom the Software is furnished to do so,
   subject to the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <string>

using namespace std;

void inplace_replace_all(string& s, const string &find, const string &repl);
void find_first_format_spec(const string& s, size_t& pos, size_t& len);

// trivial case
static inline string make_string(const string& s)
{
    return s;
}

// format + single string arg
string make_string(const string& format, const char* arg);
string make_string(const string& format, const string& arg);

// format + single non-string arg
template<typename T>
static string make_string(const string& format, const T& arg)
{
    const size_t BUFSIZE = 128;
    static char buff[BUFSIZE];

    string result = format;

    bool first = true;
    size_t pos, len;
    find_first_format_spec(result, pos, len);

    while (pos != string::npos)
    {
        if (!first)
        {
            // not enough args
            result.replace(pos, len, "ERR");
        }
        else if (format[pos + len - 1] == 's')
        {
            // format specifier is string type, but arg is not
            // sprintf would probably core dump on this
            result.replace(pos, len, "ERR");
        }
        else
        {
            snprintf(buff, BUFSIZE, format.substr(pos, len).c_str(), arg);
            result.replace(pos, len, buff);
        }

        find_first_format_spec(result, pos, len);
        first = false;
    }

    inplace_replace_all(result, "%%", "%");
    return result;
}

// format + multiple args
template<typename T, typename... Ts>
static string make_string(const string& format, const T& arg1, const Ts&... rest)
{
    size_t pos, len;
    find_first_format_spec(format, pos, len);

    // split after first format specifier
    string fmt1, fmt2;
    if (len > 0)
    {
        size_t split = pos + len;
        fmt1 = format.substr(0, split);
        fmt2 = format.substr(split);       
    }
    else
    {
        fmt1 = format;
    }

    return make_string(fmt1, arg1) + make_string(fmt2, rest...);
}
