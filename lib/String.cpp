#include "String.h"

namespace String{

    size_t string::getLength() const
    {
        return this->length;
    }
    char string::operator[](size_t index) const
    {
        if (index >= this->realLength)
        {
            throw std::out_of_range("Index out of range");
        }
        return this->str[index];
    }
    

    
    void string::operator=(const char *str)
    {
        if (this->str != nullptr)
        {
            delete[] this->str;
        }
        this->length = 0;
        while (str !=nullptr && str[this->length] != '\0')
        {
            this->length++;
        }
        this->realLength = this->length;
        this->str = new char[this->length];
        for (size_t i = 0; i < this->length; i++)
        {
            this->str[i] = str[i];
        }
    }
    void string::operator=(const string &str)
    {
        if (this->str != nullptr)
        {
            delete[] this->str;
        }
        this->length = str.getLength();
        this->realLength = this->length;
        this->str = new char[this->length];
        for (size_t i = 0; i < this->length; i++)
        {
            this->str[i] = str[i];
        }
    }
    
    string& string::operator=(string &&other){
        if(this == &other){
            return *this;
        }
        if (this->str != nullptr)
        {
            delete[] this->str;
        }
        this->str = other.str;
        this->length = other.length;
        this->realLength = other.realLength;
        other.str = nullptr;
        other.length = 0;
        other.realLength = 0;
        return *this;
    }


    bool string::operator==(const string &str) const
    {
        if (this->length != str.getLength())
        {
            return false;
        }
        for (size_t i = 0; i < this->length; i++)
        {
            if (this->str[i] != str[i])
            {
                return false;
            }
        }
        return true;
    }
    bool string::operator==(const char *str) const
    {
        size_t len = 0;
        while (str[len] != '\0')
        {
            len++;
        }
        if (this->length != len)
        {
            return false;
        }
        for (size_t i = 0; i < this->length; i++)
        {
            if (this->str[i] != str[i])
            {
                return false;
            }
        }
        return true;
    }
    bool string::operator==(const char ch) const
    {
        if (this->length != 1)
        {
            return false;
        }
        return this->str[0] == ch;
    }
    
    std::ostream &operator<<(std::ostream &os,const string &str)
    {

        // os << "adress:" << static_cast<void*>(str.str) << " values:";
        for (size_t i = 0; i < str.getLength(); i++)
        {
            os << str[i];
        }
        return os;
    }
    void string::push_back(char c)
    {
        if (this->length < this->realLength)
        {
            this->str[this->length] = c;
            this->length++;
        }
        else
        {
            this->realLength *= 2;
            char *newStr = new char[this->realLength];
            for (size_t i = 0; i < this->length; i++)
            {
                newStr[i] = this->str[i];
            }
            newStr[this->length] = c;
            this->length++;
            delete[] this->str;
            this->str = newStr;
            // throw std::out_of_range("String is full");
        }
    }

    string string::slice(size_t start, size_t end) const
    {
        if (start >= this->length || end > this->length || start > end)
        {
            throw std::out_of_range("Index out of range");
        }
        string newStr(end - start);
        for (size_t i = start; i < end; i++)
        {
            newStr.push_back(this->str[i]);
        }
        return newStr;
    }

    void string::clear()
    {
        this->length = 0;
    }
    bool string::isEmpty() const
    {
        return this->length == 0;
    }
    bool string::isNumber() const
    {
        for (size_t i = 0; i < this->length; i++)
        {
            if (this->str[i] < '0' || this->str[i] > '9')
            {
                return false;
            }
        }
        return true;
    }
    bool string::isAlpha() const
    {
        for (size_t i = 0; i < this->length; i++)
        {
            if ((this->str[i] < 'a' || this->str[i] > 'z') && (this->str[i] < 'A' || this->str[i] > 'Z'))
            {
                return false;
            }
        }
        return true;
    }
    bool string::isAlphaNumeric() const
    {
        for (size_t i = 0; i < this->length; i++)
        {
            if ((this->str[i] < 'a' || this->str[i] > 'z') && (this->str[i] < 'A' || this->str[i] > 'Z') && (this->str[i] < '0' || this->str[i] > '9'))
            {
                return false;
            }
        }
        return true;
    }
    bool string::isIdentifier() const
    {
        if (this->str[0] < 'a' || this->str[0] > 'z')
        {
            return false;
        }
        for (size_t i = 1; i < this->length; i++)
        {
            if ((this->str[i] < 'a' || this->str[i] > 'z') && (this->str[i] < 'A' || this->str[i] > 'Z') && (this->str[i] < '0' || this->str[i] > '9'))
            {
                return false;
            }
        }
        return true;
    }





    string::string()
    {
        this->str = nullptr;
        this->length = 0;
        this->realLength = 0;
    }

    string::string(size_t realLength)
    {
        this->realLength = realLength;
        this->str = new char[realLength];
        this->length = 0;
    }
    string::string(const char ch){
        this->length = 1;
        this->realLength = 1;
        this->str = new char[1];
        this->str[0] = ch;
    }
    string::string(const char *str) : string(){
        *this = str;
    }
    string::string(const string &str) : string(){
        *this = str;
    }
    string::string(string &&other) : string(){
        *this = std::move(other);
    }
    string::~string()
    {
        if (this->str != nullptr)
        {
            delete[] this->str;
        }
    }
}


