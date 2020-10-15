#include "stdafx.h"

#include <iostream>
#include <regex>
/*

                 abc. Any character except newline
                 abc? Zero or 1 preceding character
                 abc* Zero or more preceding character
                 abc+ One or more preceding character
              ab[cd]* Any charcter inside the squre brackets
             ab[^cd]* Any character inside the square brackets
            ab[cd]{3} Only 3 preceding character
           ab[cd]{3,} 3 or more preceding character
          ab[cd]{3,5} 3 to 5 preceding character
           abc|de[fg] abc or def or deg
         abc|de[\[fg] abc or def, de[ or deg
           (abc)de+\1 \1 Fist Group again (NOTE: in char array, \ should be replaced to \\ to escape
       (ab)c(de+)\2\1 \2 Second Group and first Group again the number of repeated character by + should be matched while repeating

       Actual example:

       [[:w:]]+@[[:w:]]+\.com       [[:w:]] word character (digit, number or underscore)
                                    \.  = escaped .


       when searching,
                 ^abc abc should be placed at the beginning
                 abc$ abc should be placed at the end

*/
using namespace std;
void TestRegEx(int argc, char **argv)
{
    string regEx;
    string str = "regex";
    bool regExS = false;

    while (true)
    {
        if (str == "regex" || str == "regexs")
        {
            cout << "Type regex: ";
            cin >> regEx;

            if (str == "regexs")
            {
                regExS = true;
            }
        }

        cout << "Type data (NOTE: 'regex(s)' for switching): ";
        cin >> str;

        if (str != "regex" && str != "regexs")
        {
            regex e(regEx.c_str(), regex_constants::icase);
            smatch m;
            bool match = true;
            if (!regExS)
            {
                match = regex_match(str, e);
            }
            else
            {
                match = regex_search(str, m, e);
            }

            if (match)
            {
                HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hStdout, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            }
            else
            {
                HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_INTENSITY);
            }

            cout << (match ? "Matched" : "Not matched") << " to " << regEx << endl << endl;

            HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hStdout, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);

            if (match && regExS)
            {
                for (int n = 0; n < m.size(); n++)
                {
                    cout << "m[" << n << "]: str()=" << m[n].str() << endl;
                    // alternative for m[n].str()
                    //     m.str(n)
                    //     Or
                    //     *(m.begin()+n)
                }
                cout << "m.prefix().str(): " << m.prefix().str() << endl;
                cout << "m.suffix().str(): " << m.suffix().str() << endl;
            }
        }
    }
}