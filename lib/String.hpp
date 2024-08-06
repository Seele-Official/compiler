#ifndef STRING_H
#define STRING_H
#include <utility>
#include <iostream>
#include <stdexcept>
#include <cstdint>
#include <initializer_list>
#include <vector>
namespace String {
    class string {
    private:
        char *str;
        size_t length;
        size_t realLength;
    public:
        string();
        string(int number);
        string(int64_t number);
        string(uint64_t number);
        string(const char *str, size_t length);
        string(const char *str, size_t length, size_t realLength);
        string(const char ch);
        string(const char *str);
        string(const string &str);
        string(string &&other);
        ~string();

        string& setRealLength(size_t realLength);
        size_t getLength() const;
        char operator[](size_t index) const;
        string& operator=(const char *str);
        string& operator=(const string &str);
        string& operator=(string &&other);

        bool operator==(const string &str) const;
        bool operator==(const char *str) const;
        bool operator==(const char ch) const;

        template <typename T>
        bool operator!=(const T &str) const;

        friend std::ostream &operator<<(std::ostream &os, const string &str);
        friend std::istream &operator>>(std::istream &is, string &str);
        string& push_back(char c);
        string& push_back(const string &other);

        string slice(size_t start, size_t end) const;
        string format(std::initializer_list<string> args);

        template <typename ...Args>
        string format(Args... args);

        string& clear();
        bool isEmpty() const;
        bool isNumber() const;
        bool isAlpha() const;
        bool isAlphaNumeric() const;
        bool isIdentifier() const;
    };



    string& strFormat(string &op, string &arg);

    template <typename T>
    bool string::operator!=(const T &str) const {
        return !(*this == str);
    }

    template <typename T>
    std::vector<string>& convertArgsToStrings(std::vector<string> &vec, T arg){
        vec.push_back(string(arg));
        return vec;
    }
    template <typename T, typename ...Args>
    std::vector<string>& convertArgsToStrings(std::vector<string> &vec, T arg, Args... args){
        vec.push_back(string(arg));
        return convertArgsToStrings(vec, args...);
        
    }


    static string stringBuffer("", 0, 64), op("", 0, 8);
    template <typename ...Args>
    string format(const string &str, Args... args){
        std::vector<string> vec;
        vec.reserve(sizeof...(args));
        std::cout << "sizeof...(args): " << sizeof...(args) << std::endl;
        convertArgsToStrings(vec, args...);
        stringBuffer.clear();
        char ch;
        size_t begin = 0;
        for (size_t i = 0; i < str.getLength(); i++){
            if ((ch = str[i]) != '{'){
                if (ch == '/'){ ch = str[++i]; }

                stringBuffer.push_back(ch);

            } else {
                if (begin >= vec.size()){

                    throw std::runtime_error("Too few arguments");

                } 
                size_t j = ++i;
                op.clear();
                for (; (ch = str[j]) != '}'; j++){
                    op.push_back(ch);
                }
                i += j - i;


                stringBuffer.push_back(std::move(strFormat(op, vec[begin++])));
                // 假装已经完成了格式化
            }
        }

        if (begin < vec.size()){

            throw std::runtime_error("Too many arguments");

        }
        

        return stringBuffer.slice(0, stringBuffer.getLength());
    }

    template <typename ...Args>
    string string::format(Args... args){
        return String::format(*this, args...);
    }
}

#endif // STRING_H
