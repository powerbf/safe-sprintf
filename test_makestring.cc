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

#include <iostream>
#include <sstream>
#include <chrono>

#include "make_string.h"



int main()
{
    ios::sync_with_stdio(true);

    string item = "arrows";

    cout << "TEST 1:" << endl;
    cout << make_string("%d%%\n", 1);

    cout << "TEST 2:" << endl;
    cout << make_string("%s\n", item);

    cout << "TEST 3:" << endl;
    cout << make_string("%d %s\n", 2, item);

    cout << "TEST 4:" << endl;
    cout << make_string("%d %s", 10, item) << endl;

    ostringstream os;
    for (int i = 0; i < 200; i++)
        os << "A";

    cout << "TEST 5:" << endl;
    cout << make_string("%s\n", os.str());
    cout << "TEST 6:" << endl;
    cout << make_string("%s\n", os.str().c_str());

    cout << "TEST 7:" << endl;
    cout << make_string("%s picks up %d %s.", "The orc", 27, "arrows");
    cout << endl;

    cout << "TEST 8: BAD FORMAT SPEC" << endl;
    cout << make_string("My number is %f and my string is %s", string("The orc"));
    cout << endl;

    cout << "TEST 9: BAD FORMAT SPEC" << endl;
    cout << make_string("My string is %s", 27);
    cout << endl;

    cout << "PERFORMANCE:" << endl;

    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i =0; i < 10000000; i++)
        make_string("%s bends down and picks up %d %s.", "The orc", 27, "arrows");
    auto t2 = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();

    std::cout << duration << " milliseconds" << endl;

    cout << "done." << endl;
    return 0;
}
