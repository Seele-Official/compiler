#ifndef STRING_HPP
#define STRING_HPP

#include<cstddef>
#include<ostream>


namespace String{
    class string
    {
    private:
        char *str;
        size_t length;
        size_t realLength;
    public:
        size_t getLength() const
        {
            return this->length;
        }
        char operator[](size_t index) const
        {
            if (index >= this->realLength)
            {
                throw std::out_of_range("Index out of range");
            }
            return this->str[index];
        }
        

        
        void operator=(const char *str)
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
        void operator=(const string &str)
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
        
        bool operator==(const string &str) const
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
        bool operator==(const char *str) const
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
        bool operator==(const char ch) const
        {
            if (this->length != 1)
            {
                return false;
            }
            return this->str[0] == ch;
        }
        
        template <typename T>
        bool operator!=(const T &str) const
        {
            return !(*this == str);
        }

        friend std::ostream &operator<<(std::ostream &os,const string &str)
        {
            for (size_t i = 0; i < str.getLength(); i++)
            {
                os << str[i];
            }
            return os;
        }
        void push_back(char c)
        {
            if (this->length < this->realLength)
            {
                this->str[this->length] = c;
                this->length++;
            }
            else
            {
                throw std::out_of_range("String is full");
            }
        }
        string slice(size_t start, size_t end) const
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

        void clear()
        {
            this->length = 0;
        }
        bool isEmpty() const
        {
            return this->length == 0;
        }
        bool isNumber() const
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
        bool isAlpha() const
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
        bool isAlphaNumeric() const
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
        bool isIdentifier() const
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
        string();
        string(size_t realLength);
        string(const char ch);
        string(const char *str);
        string(const string &str);
        ~string();
    };
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
    string::~string()
    {
        if (this->str != nullptr)
        {
            delete[] this->str;
        }
    }
}

// #include"String.tpp"
#endif // STRING_HPP