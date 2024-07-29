#include<stdexcept>
#include<iostream>

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



namespace List{
    template <typename T>
    class list
    {
    private:
        struct node
        {
            T value;
            node *next;
            node *prev;
            node(const T &value, node *next, node *prev){
                this->value = value;
                this->next = next;
                this->prev = prev;
            }

        };
        node *head;
        node *tail;
        size_t length;
    public:
        size_t getLength() const
        {
            return this->length;
        }
        
        T operator[](size_t index) const
        {
            if (index >= this->length)
            {
                throw std::out_of_range("Index out of range");
            }
            node *current = this->head;
            for (size_t i = 0; i < index; i++)
            {
                current = current->next;
            }
            return current->value;
        }


        void push_back(const T &value)
        {
            node *newNode = new node(value, nullptr, this->tail);
            if (this->tail != nullptr)
            {
                this->tail->next = newNode;
            }
            this->tail = newNode;
            if (this->head == nullptr)
            {
                this->head = newNode;
            }
            this->length++;
        }
        T pop_back() {
            if (this->tail == nullptr) {
                throw std::out_of_range("List is empty");
            }
            T value = this->tail->value;
            node *oldTail = this->tail;
            this->tail = this->tail->prev;
            if (this->tail != nullptr) {
                this->tail->next = nullptr;
            } else {
                this->head = nullptr;
            }
            delete oldTail;
            this->length--;
            return value;
        }
        void push_front(const T &value)
        {
            node *newNode = new node(value, this->head, nullptr);
            if (this->head != nullptr)
            {
                this->head->prev = newNode;
            }
            this->head = newNode;
            if (this->tail == nullptr)
            {
                this->tail = newNode;
            }
            this->length++;
        }
        T pop_front(){
            if(this->head == nullptr){
                throw std::out_of_range("List is empty");
            }
            T value = this->head->value;
            node *oldHead = this->head;
            this->head = this->head->next;
            if(this->head != nullptr){
                this->head->prev = nullptr;
            }else{
                this->tail = nullptr;
            }
            delete oldHead;
            this->length--;
            return value;
        }
        friend std::ostream &operator<<(std::ostream &os, list &l)
        {
            node *current = l.head;
            os << "[";
            if (current != nullptr)
            {
                os << current->value;
                current = current->next;
            }
            while (current != nullptr)
            {
                os << ", " << current->value;
                current = current->next;
            }
            os << "]";
            return os;
        }
        
        size_t find(const T &value){
            node *current = this->head;
            size_t index = 0;
            while (current != nullptr)
            {
                if (current->value == value)
                {
                    return index;
                }
                current = current->next;
                index++;
            }
            return (size_t)-1;
        }

        list();
        list(std::initializer_list<T> l);
        list(const list<T> &l);
        ~list();
    };

    template <typename T>
    list<T>::list()
    {
        this->head = nullptr;
        this->tail = nullptr;
        this->length = 0;
    }
    template <typename T>
    list<T>::list(std::initializer_list<T> l) : list()
    {
        for(auto i = l.begin(); i != l.end(); i++){
            this->push_back(*i);
        }
    }


    template <typename T>
    list<T>::list(const list<T> &l) : list()
    {
        node *current = l.head;
        while (current != nullptr)
        {
            this->push_back(current->value);
            current = current->next;
        }
    }


    template <typename T>
    list<T>::~list()
    {
        node *current = this->head;
        while (current != nullptr)
        {
            node *next = current->next;
            delete current;
            current = next;
        }
    }
}


