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

#include <cstring>

#include "make_string.h"

void inplace_replace_all(string& s, const string &find, const string &repl)
{
    string::size_type start = 0;
    string::size_type found;

    while ((found = s.find(find, start)) != string::npos)
    {
        s.replace(found, find.length(), repl);
        start = found + repl.length();
    }
}

// get position and length of first printf-style format specifier in string
// if none found, pos = string::npos and len = 0
void find_first_format_spec(const string& s, size_t& pos, size_t& len)
{
    const string length_specs = "hljztL";

    size_t i = 0;
    pos = string::npos;
    len = 0;

    while (len == 0 && i < s.length())
    {
        char ch = s[i];

        if (pos != string::npos)
        {
            if (isalpha(ch) && length_specs.find(ch) == string::npos)
                len = i - pos + 1;
            else if (ch == '%')
            {
                if (pos == i - 1)
                    // escaped % sign
                    pos = string::npos;
                else
                    pos = i;
            }
        }
        else if (ch == '%')
        {
            pos = i;
        }

        i++;
    }

    if (len == 0)
    {
        // not found
        pos = string::npos;
    }
}

string make_string(const string& format, const char* arg)
{
    const size_t BUFSIZE = 4096;
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
        else if (format[pos + len - 1] != 's')
        {
            // format specifier is not string type
            // (but should be safe to let pointer be treated as int or whatever)
            snprintf(buff, BUFSIZE, result.substr(pos, len).c_str(), arg);
            result.replace(pos, len, buff);
        }
        else
        {
            // guard against null pointer
            const char* safe_arg = (arg ? arg : "");

            if (len == 2)
            {
                // Format spec is just %s. For efficiency, just do a straight string replacement.
                // (about 15% faster with g++ without -O flag, about 25% faster with -O2)
                result.replace(pos, len, safe_arg);      
            }
            else
            {
                snprintf(buff, BUFSIZE, result.substr(pos, len).c_str(), safe_arg);
                result.replace(pos, len, buff);
            }
        }

        find_first_format_spec(result, pos, len);
        first = false;
    }

    inplace_replace_all(result, "%%", "%");
    return result;
}

string make_string(const string& format, const string& arg)
{
    return make_string(format, arg.c_str());
}

