#include "String.hpp"

namespace String{

    size_t string::getLength() const
    {
        return this->length;
    }
    string& string::adjustLength(size_t realLength){
        this->realLength = realLength;
        char *ptr = new char[realLength];
        for (size_t i = 0; i < this->length; i++){
            ptr[i] = this->str[i];
        }
        if (this->str != nullptr)
            delete[] this->str;
        this->str = ptr;
        return *this;
    }
    char string::operator[](size_t index) const
    {
        if (index >= this->realLength)
        {
            throw std::out_of_range("Index out of range");
        }
        return this->str[index];
    }
    
    string& string::operator=(const char *str)
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
        return *this;
    }
    string& string::operator=(const string &str)
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
        return *this;
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

        for (size_t i = 0; i < str.getLength(); i++)
        {
            os << str[i];
        }
        return os;
    }
    std::istream &operator>>(std::istream &is, string &str){
        str.clear();
        char ch;
        while(is.get(ch)){
            str.push_back(ch);
        }
        return is;
    }
    
    string& string::push_back(char c)
    {
        if (this->length < this->realLength)
        {
            this->str[this->length] = c;
            this->length++;
        }
        else
        {
            this->adjustLength(this->realLength * 2 + 1);
            this->str[this->length] = c;
            this->length++;
            // this->realLength *= 2;
            // char *newStr = new char[this->realLength];
            // for (size_t i = 0; i < this->length; i++)
            // {
            //     newStr[i] = this->str[i];
            // }
            // newStr[this->length] = c;
            // this->length++;
            // delete[] this->str;
            // this->str = newStr;
            // // throw std::out_of_range("String is full");
        }
        return *this;
    }
    string& string::push_back(const string &other){
        while (this->length + other.getLength() > this->realLength){
            this->adjustLength(this->realLength * 2 + 1);
        }

        char *ptr = this->str + this->length;
        for (size_t i = 0; i < other.getLength(); i++){
            ptr[i] = other[i];
        }
        this->length += other.getLength();

        return *this;
    }
    
    string string::slice(size_t start, size_t end) const
    {
        if (start >= this->length || end > this->length || start > end)
        {
            throw std::out_of_range("Index out of range");
        }
        string newStr("", 0, end - start); 
        for (size_t i = start; i < end; i++)
        {
            newStr.push_back(this->str[i]);
        }
        return newStr;
    }
    
    string& strFormat(string &op, string &arg){

        return arg;
    }
    string string::format(std::initializer_list<string> args)
    {
        string stringBuffer("", 0, 128), op("", 0, 8);
        auto arg = args.begin();
        char ch;
        for (size_t i = 0; i < this->getLength(); i++){
            if ((ch = str[i]) != '{'){
                if (ch == '/'){ ch = str[++i]; }
                
                stringBuffer.push_back(ch);

            } else {
                size_t j = ++i;
                for (; (ch = str[j]) != '}'; j++){
                    op.push_back(ch);
                }
                i += j - i;

                op.clear();
                string str = std::move(*arg);
                stringBuffer.push_back(strFormat(op, str));
                // 假装已经完成了命令匹配
                if (arg != args.end()){
                    arg++;
                } else {
                    throw std::out_of_range("args out of range");
                }
            }
        }
        if (arg != args.end()){
            throw std::out_of_range("args out of range");
        }
        return stringBuffer;
    }
    
    string& string::clear()
    {
        this->length = 0;
        return *this;
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
    string::string(int number): string()
    {
        this->length = 0;
        int temp = number;
        while (temp != 0)
        {
            temp /= 10;
            this->length++;
        }
        if (number < 0)
        {
            this->length++;
        }
        this->realLength = this->length;
        this->str = new char[this->length];
        if (number < 0)
        {
            this->str[0] = '-';
            number = -number;

            for (size_t i = 0; i < this->length-1; i++)
            {
                this->str[this->length - i - 1] = number % 10 + '0';
                number /= 10;
            }
        }else{
            for (size_t i = 0; i < this->length; i++)
            {
                this->str[this->length - i - 1] = number % 10 + '0';
                number /= 10;
            }
        }
        
    }
    string::string(int64_t number): string()
    {
        this->length = 0;
        int64_t temp = number;
        while (temp != 0)
        {
            temp /= 10;
            this->length++;
        }
        if (number < 0)
        {
            this->length++;
        }
        this->realLength = this->length;
        this->str = new char[this->length];
        if (number < 0)
        {
            this->str[0] = '-';
            number = -number;

            for (size_t i = 0; i < this->length-1; i++)
            {
                this->str[this->length - i - 1] = number % 10 + '0';
                number /= 10;
            }
        }else{
            for (size_t i = 0; i < this->length; i++)
            {
                this->str[this->length - i - 1] = number % 10 + '0';
                number /= 10;
            }
        }
        
    }
    string::string(uint64_t number): string()
    {
        this->length = 0;
        uint64_t temp = number;
        while (temp != 0)
        {
            temp /= 10;
            this->length++;
        }
        this->realLength = this->length;
        this->str = new char[this->length];
        for (size_t i = 0; i < this->length; i++)
        {
            this->str[this->length - i - 1] = number % 10 + '0';
            number /= 10;
        }
    }
    string::string(const char *str, size_t length): string()
    {
        this->realLength = length; 
        this->length = length;
        this->str = new char[length];
        for (size_t i = 0; i < length; i++)
        {
            this->str[i] = str[i];
        }
    }
    string::string(const char *str, size_t length, size_t realLength): string()
    {
        this->realLength = realLength; 
        this->length = length;
        this->str = new char[realLength];
        for (size_t i = 0; i < length; i++)
        {
            this->str[i] = str[i];
        }
    }
    string::string(const char ch): string(){
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


