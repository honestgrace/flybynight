#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <bitset>
#include <complex>
#include <iomanip>
#include <algorithm>

using namespace std;

void General()
{
    { 
        // close of handle automatically RAII technique (Resource Acquisition is initialization)
        ofstream of("C:\\temp\\test.txt");
        of << "test.txt" << endl;
        of << bitset<8>(14) << endl;  //00001110
        of << complex<int>(2, 3) << endl; // (2,3)
    }

    {
        // ofstream of("C:\\temp\\test.txt");  // this will clear content
        ofstream of("C:\\temp\\test.txt", ofstream::app); // move the output pointer to the end of the file
        of << "Appended" << endl;
    }

    {
        ofstream of("C:\\temp\\test.txt", ofstream::in | ofstream::out);
        of.seekp(10, ios::beg); // Move the output pointer 10 chars after begin
        of << "Inserted" << endl; // Overwrite 
        of.seekp(-5, ios::end);
        of << "Inserted2" << endl;
        of.seekp(-5, ios::cur);
        of << "Inserted3" << endl;
    }

    {
        ifstream inf("C:\\temp\\test.txt");
        int i;
        inf >> i; // read one word
        // Error status: goodbit, badbit, failbit, eofbit

        inf.good();  // everything is ok (goodbit == 1)
        inf.bad();   // non-recoverable error (badbit == 1)
        inf.fail();  // failed stream operation but recoverable (failbit == 1 and badbit == 1)
        inf.eof();   // end of file (eofbit == 1)

        inf.clear(); // clear all the error status  =>  clear(ios::goodbit)
        inf.clear(ios::badbit); // sets a new value to the error flag
        inf.rdstate();  // read the current status flag
        inf.clear(inf.rdstate() & ~ios::failbit); // clear only the failbit

        if (inf)  // equivalent to: if (!inf.fail())
            cout << "Read scuccessfully" << endl;
        else
            cout << "Fail to read" << endl;


        if (inf >> i)    // inf >> i will return the original ostream object
            cout << "Read scuccessfully" << endl;

        // Handle the errors with exceptions
        inf.exceptions(ios::badbit | ios::failbit); // Setting the exception mask
        // When badbit or failbit set to 1, exception of ios::failure will be thrown
        // When eofbit set to 1, no exception

        inf.exceptions(ios::goodbit); // No exception

        // Formatting
        cout << 34 << endl;
        cout.setf(ios::oct, ios::basefield);
        cout << 34; // 42
        cout.setf(ios::showbase);
        cout << 34; // 042
        cout.setf(ios::hex, ios::basefield);
        cout << 34; // 0x22
        cout.unsetf(ios::showbase);
        cout << 34; // 22

        cout.setf(ios::dec, ios::basefield);

        cout.width(10);
        cout << 26;
        cout.setf(ios::left, ios::adjustfield);

        // Floating point value
        cout.setf(ios::scientific, ios::floatfield);
        cout << 340.1 << endl;  // 3.401000e+002
        cout.setf(ios::fixed, ios::floatfield);
        cout << 340.1 << endl;  // 340.100000
        cout.precision(3);
        cout << 340.1 << endl;  // 340.100

        cin.setf(ios::hex, ios::basefield);
        cin >> i;   //Enter 12 
        // i == 18

        ios::fmtflags f = cout.flags();
        cout.flags(ios::oct | ios::showbase);

        // Member functions for unformatted IO
        // input
        {
            ifstream inf("MyLog.txt");
            char buf[80];
            inf.get(buf, 80); // read up to 80 chars and save into buf
            inf.getline(buf, 80); // read up to 80 chars or until \n and save into buf
            inf.read(buf, 20); // read 20 chars
            inf.ignore(3);
            inf.peek(); // search a character on top of the stream
            inf.unget(); // return a char back to the stream
                         // cf.) inf.putback('z')
            inf.get();   // get a char
            inf.gcount(); // return the number of chars being read by last unformatted read

            // output
            ofstream of("MyLog.txt");
            of.put('c');
            of.write(buf, 6); // write first 6 char of buf
            of.flush(); //it will flush the output
        }
    }
}

/*
ostream& endl(ostream& sm) 
{
    sm.put('\n');
    sm.flush();
    return sm;
}

ostream& ostream::operator<<(ostream& (*func)(ostream&(( 
{
    return (*func)(*this);
}
*/

void TestManipuator()
{
    cout << "hello" << endl;  // \n
    cout << ends;  // \0
    cout << flush;
    cin >> ws;  // read and discard white spaces
    cout << setw(8) << left << setfill('_') << 99 << endl;
    //99______

    cout << hex << showbase << 14; // 0xe
}

void StreamBuffer()
{
    // IO operation
    // formatting data -- stream
    // communicating data to external devices -- stream buffer

    cout << 34;
    streambuf* pbuf = cout.rdbuf();

    ostream myCout(pbuf);
    myCout << 34; // 34 to stdout
    myCout.setf(ios::showpos);
    myCout.width(20);
    myCout << 12 << endl; //              +12
    cout << 12 << endl;   //12
    
    ofstream of("MyLog.txt");
    streambuf* origBuf = cout.rdbuf();  // save before redirecting
    cout.rdbuf(of.rdbuf());
    cout << "Hello" << endl; // MyLog.txt has "Hello"
    //Redirecting

    cout.rdbuf(origBuf);  // rollback

    cout << "Goodbye" << endl;  // stdout: Goodbye

    // Stream buffer iterator
    istreambuf_iterator<char> i(cin);
    ostreambuf_iterator<char> o(cout);

    while (*i != 'x')
    {
        *o = *i;
        ++o;
        ++i;
    }

    // other example using copy
    copy(istreambuf_iterator<char>(cin), istreambuf_iterator<char>(), ostreambuf_iterator<char>(cout));
}

void StringStream()
{
    // String stream
    stringstream ss; // Stream without IO operation
    // read/write of string

    ss << 89 << "  Hex: " << hex << 89 << " Oct: " << oct << 89;
    cout << ss.str() << endl; // 89 Hex: 59 Oct: 131
    int a, b, c;
    string s1;

    ss >> hex >> a;  // Formatted input works token by token. spaces, tabs, newlines
        // a <== 137 (=> hex(89))
    ss >> s1; // s1: "Hex:"
    ss >> dec >> b; // b <== 59
    ss.ignore(6); 
    ss >> oct >> c;  // c <== 89 

    // ostringstream -- formatted output
    // istringstream -- formatted input
}

struct Dog
{
    int age_;
    string name_;
};

ostream& operator << (ostream& sm, const Dog& d)
{
    sm << "My name is " << d.name_ << " and my age is " << d.age_ << endl;
    return sm;
}

istream& operator >> (istream& sm, Dog& d)
{
    sm >> d.age_;
    sm >> d.name_;
    return sm;
}

void MyOwnStreamClass()
{
    Dog d{ 2, "bog" };
    Dog d2;
    cout << d;
    cin >> d2;
    cout << d2;
}

void TestString()
{
    // consturctor
    string s1("hello");
    string s2("hello", 3); // s2: hel
    string s3(s1, 2); // s3: llo
    string s4(s1, 2, 2); // s4: ll
    string s5(5, 'a'); // s5: "aaaaa"
    string s6({ 'a', 'b', 'c' }); // s6: abc

    //size
    s1 = "goodbye";
    s1.size(); s1.length();  //Synonymous, both returns 7

    s1.capacity(); // size of storage space currently allocated to s1
    s1.reserve(100); // s1.capacity() => 100
    s1.reserve(5);   // s1.capacity => 7  content won't change
    s1.shrink_to_fit();  // no change

    s1.resize(9); // s1: goodbye\0\0
    s1.resize(12, 'x'); // s1: goodbye\0\0\xxx
    s1.resize(3); // s1: goo

    s1 = "goodbye";
    s1[2]; // o
    s1[2] = 'x';  // goxdbye
    s1.at(2) = 'y';  // goydbye
    try
    {
        s1.at(20);  // throw exception
    }
    catch(std::out_of_range e)
    {
        cout << "Out of range error handled here";
    }

    /*
    try
    {
        s1[20];  //undefined behavior and impossible to catch
    }
    catch (...)
    {
        cout << "This won't work";
    }
    */

    s1.front();  // g
    s1.back();   // e
    s1.push_back('z');
    s1.pop_back();
    s1.begin(); // return iterator pointing start
    s1.end();   // return iterator pointing end

    //Like vector, string does not have push_front() and pop_front()
    s1 = "0123456789";
    string s7(s1.begin(), s1.begin() + 3);  // 012
    string s8 = s1.substr(3, s1.length() - 3);  // 3456789
    string s9(s8, 3);    // 6789

    // Ranged Access
    // assign, append, insert, replace
    string s10 = "Dragon Land";
    s1.assign(s10);  // s1 = s10
    s1.assign(s10, 2, 4); // s1 = agon
    s1.assign("Wisdom"); // s1: Wisdom
    s1.assign("Wisdom", 3); // s1: Wis
    // s1.assign(s10, 3); // error
    s1.assign(3, 'x'); // s1: xxx
    s1.assign({ 'a', 'b', 'c' }); // s1: abc

    s1.append(" def"); // s1: "abc def"
    s1.insert(2, "mountain", 4); // s1: "abmounc def"
    s1.replace(2, 5, s10, 3, 3);  // s1: "abgon def"
    s1.erase(1, 4); // s1: "a def"
    s10.substr(2, 4); // agon

    s1 = "abc";
    s1.c_str();  // "abc\0"
    s1.data();   // before C11 "abc"; after C11 synonymous to c_str()

    s1.swap(s10); // swap two strings

    // Member function Algorithms: copy, find, compare

    s1 = "abcdefg";
    char buf[20];
    size_t len = s1.copy(buf, 3);  // buf: abc len = 3   NOTE: there is no null terminator
    len = s1.copy(buf, 4 /*size*/, 2/*start point*/); // buf: cdef len = 4
    
    s1 = "If a job is worth doing, it's worth doing well";
    size_t found = s1.find("doing"); // found == 17
    found = s1.find("doing", 20); // found == 35
    found = s1.find("doing well", 0); // found == 35
    found = s1.find("doing well", 0, 5 /* search first 5 char only */); // found == 17

    found = s1.find_first_of("doing");  // search firt d, o, i, n, or, g 
                                        // found = 6 => 'o'
    found = s1.find_first_of("doing", 10); // found = 12
    found = s1.find_first_of("doing", 10, 1); // search only 'd' found = 17

    found = s1.find_last_of("doing"); // found = 39   'g'

    found = s1.find_first_not_of("doing"); // found = 0
    found = s1.find_last_not_of("doing"); // found = 44

    s1.compare(s2); // return posive if (s1 > s2); negative if (s1 < s2); 0 if (s2 == s1)
    if (s1 > s2) {}   // if (s1.compare(s2) > 0)
    s1.compare(3/* position */, 2 /* size */, s2);

    string ss = s1 + s2;


    // Non-member functions
    cout << s1 << endl;
    cin >> s1;
    getline(cin, s1);  // default delimeter of '\n'
    getline(cin, s1, ';'); // delimeter is ';'

    // convert a number into a string
    s1 = to_string(8);
    s1 = to_string(2.3e7); // s1: 23000000.000000
    s1 = to_string(0xa6); // s1: 164
    s1 = to_string(034); // s1: 28

    // convert a string into a number
    s1 = "190";
    int i = stoi(s1);
    s1 = "190 moneys";
    size_t pos;
    i = stoi(s1, &pos); // i = 190, pos = 3

    s1 = "a monkey";
    i = stoi(s1, &pos);  // exception of invalid_argument
    i = stoi(s1, &pos, 16 /*hexa*/);  // i = 10

    // stol, stod, stof, etc

    // stringstream

    // <string>
    // String and Algorithms

    s1 = "Variety is the spice of life.";
    int num = count(s1.begin(), s1.end(), 'e'); // 4
    num = count_if(s1.begin(), s1.end(), [](char c) {return (c <= 'e' && c >= 'a'); }); //  6

    s1 = "Goodness is better than beauty.";
    string::iterator itr = search_n(s1.begin(), s1.begin() + 20, 2, 's'); // itr -> first 's'
    s1.erase(itr, itr + 5);
    s1.insert(itr, 3, 'x');
    s1.replace(itr, itr + 3, 3, 'y');  // replacing substring

    is_permutation(s1.begin(), s1.end(), s2.begin());  
    replace(s1.begin(), s1.end(), 'e', ' '); // replacing characters
    transform(s1.begin(), s1.end(), s2.begin(),
        [](char c){
            if (c < 'n')
                return 'a';
            else
                return 'z';
        });

    s1 = "abcdefg";
    rotate(s1.begin(), s1.begin() + 3, s1.end()); // defgabc

    string s;
    u16string s16;  // string char16_t
    u32string s32;  // string char32_t
    wstring sw;     // string wchar_t

    // to_wstring() this is only for wstring class

}

#define DWSectionBufSize 1024
void * TestTTT()
{
    const wstring& szBrowser = L"test2";
    wstring m_strIniFile = L"c:\\temp\\test.ini";
    void * rpBCobj = NULL;

    if (rpBCobj == NULL)
    {
        
        // ATLTRACE("LookUp(%s)\n", szBrowser.c_str());

        // Get Browser Properties
        TCHAR *szSection = new TCHAR[DWSectionBufSize];
        if (!szSection)
        {
            ATLTRACE("CCapMap::LookUp() Not enough memory");
            return rpBCobj;
        }
        TCHAR *szSave = szSection;

        if (GetPrivateProfileSection
        (
            szBrowser.c_str(),		// section
            szSection,				// return buffer
            DWSectionBufSize,		// size of return buffer
            m_strIniFile.c_str()	// .INI name
        ) == 0)
        {
            // If this call fails, that means the default browser does not exist either, so
            // everything is "unknown".
            //
            wstring szT = L"test";
            if (GetPrivateProfileSection
            (
                szT.c_str(),			// section
                szSection,				// return buffer
                DWSectionBufSize,		// size of return buffer
                m_strIniFile.c_str()	// .INI name
            ) == 0)
            {
                ATLTRACE("GPPS(%s) failed, err=%d\n",
                    szT.c_str(), GetLastError());
                if (szSave)
                {
                    delete[] szSave;
                    szSave = NULL;
                }
                return rpBCobj;
            }
        }

        // Loop through szSection, which contains all the key=value pairs and add them
        // to the browser instance property list.  If we find a "Parent=" Key, save the
        // value to add the parent's properties later.
        //
        TCHAR *szParent;
        do
        {
            szParent = NULL;
            TCHAR *szKeyAndValue = szSection;
            while (szKeyAndValue < (szSection + DWSectionBufSize) && *szKeyAndValue)
            {
                TCHAR *szKey = szKeyAndValue;					// save the key
                TCHAR *szValue = _tcschr(szKey, '=');			// find address of value part (-1)
                szKeyAndValue += _tcslen(szKeyAndValue) + 1;	// advance KeyAndValue to the next pair

                if (szKeyAndValue >= (szSection + DWSectionBufSize))
                {
                    ATLTRACE("CCapMap::LookUp szSection buffer overflow");
                    if (szSave)
                    {
                        delete[] szSave;
                        szSave = NULL;
                    }
                    return rpBCobj;
                }

                if (szValue == NULL)
                    continue;

                *szValue++ = '\0';								// separate key and value with NUL; advance

                if (_tcsicmp(szKey, _T("Parent")) == 0)
                    szParent = szValue;
            }

            // We stored all the attributes on this level.  Ascend to parent level (if it exists)
            if (szParent)
            {
                if (GetPrivateProfileSection
                (
                    szParent,				// section
                    szSection,				// return buffer
                    DWSectionBufSize,		// size of return buffer
                    m_strIniFile.c_str()	// .INI name
                ) == 0)
                {
                    // If this call fails, quit now.
                    //
                    wstring szT = L"test2";
                    if (GetPrivateProfileSection
                    (
                        szT.c_str(),			// section
                        szSection,				// return buffer
                        DWSectionBufSize,		// size of return buffer
                        m_strIniFile.c_str()	// .INI name
                    ) == 0)
                    {
                        ATLTRACE("GPPS(%s) failed, err=%d\n",
                            szT.c_str(), GetLastError());
                        /*if (szSave)
                        {
                            delete[] szSave;
                            szSave = NULL;
                        }
                        return rpBCobj; */
                    }
                }
            }
        } while (szParent);
        if (!szSave)
        {
            ;
        }
        if (szSave)
        {
            delete[] szSave;
            szSave = NULL;
        }
    }

    return rpBCobj;
}

#define IIS_VERSION METADATA_MAX_NAME_LEN

#if IIS_VERSION >= 60
#define LOCAL_MAX_SIZE METADATA_MAX_NAME_LEN + 1
#else // IIS_VERSION < 60
# define LOCAL_MAX_SIZE 32
#endif // IIS_VERSION < 60

void TestStandardLibrary(int newArgc, char **argv)
{
    cout << LOCAL_MAX_SIZE << endl;
    UNREFERENCED_PARAMETER(newArgc);
    UNREFERENCED_PARAMETER(argv);
    TestTTT();
    //General();
    //TestManipuator();
    //StreamBuffer();
    //StringStream();
    //MyOwnStreamClass();
    //TestString();
}