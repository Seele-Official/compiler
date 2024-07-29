#ifndef STRING_H
#define STRING_H

#include <iostream>
#include <stdexcept>

namespace String {

    class string {
    private:
        char *str;
        size_t length;
        size_t realLength;

    public:
        string();
        string(size_t realLength);
        string(const char ch);
        string(const char *str);
        string(const string &str);
        ~string();

        size_t getLength() const;
        char operator[](size_t index) const;
        void operator=(const char *str);
        void operator=(const string &str);

        bool operator==(const string &str) const;
        bool operator==(const char *str) const;
        bool operator==(const char ch) const;

        template <typename T>
        bool operator!=(const T &str) const;

        friend std::ostream &operator<<(std::ostream &os, const string &str);

        void push_back(char c);
        string slice(size_t start, size_t end) const;
        void clear();
        bool isEmpty() const;
        bool isNumber() const;
        bool isAlpha() const;
        bool isAlphaNumeric() const;
        bool isIdentifier() const;
    };


    template <typename T>
    bool string::operator!=(const T &str) const {
        return !(*this == str);
    }

}

#endif // STRING_H
