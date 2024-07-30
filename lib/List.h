#ifndef List_H
#define List_H
#include <iostream>
#include <initializer_list>
#include <stdexcept>
#include <utility>
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
            node(T &&value, node *next, node *prev){
                this->value = std::move(value);
                this->next = next;
                this->prev = prev;
            }

        };
        node *head;
        node *tail;
        size_t length;
    public:
        class iterator {
            private:
                node* current;

            public:
                iterator(node* ptr) : current(ptr) {}

                T& operator*() const {
                    return current->value;
                }

                iterator& operator++() {
                    current = current->next;
                    return *this;
                }

                iterator operator++(int) {
                    iterator temp = *this;
                    ++(*this);
                    return temp;
                }

                iterator& operator--() {
                    current = current->prev;
                    return *this;
                }

                iterator operator--(int) {
                    iterator temp = *this;
                    --(*this);
                    return temp;
                }

                bool operator==(const iterator& other) const {
                    return current == other.current;
                }

                bool operator!=(const iterator& other) const {
                    return current != other.current;
                }
        };

        // 迭代器相关方法
        iterator begin() {
            return iterator(head);
        }

        iterator end() {
            return iterator(nullptr);
        }


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

        list<T>& operator=(const list<T> &l){
            if(this == &l){
                return *this;
            }
            node *current = this->head;
            while (current != nullptr)
            {
                node *next = current->next;
                delete current;
                current = next;
            }
            this->head = nullptr;
            this->tail = nullptr;
            this->length = 0;
            current = l.head;
            while (current != nullptr)
            {
                this->push_back(current->value);
                current = current->next;
            }
            return *this;
        }
        list<T>& operator=(list<T> &&other){
            if(this == &other){
                return *this;
            }
            node *current = this->head;
            while (current != nullptr)
            {
                node *next = current->next;
                delete current;
                current = next;
            }
            this->head = other.head;
            this->tail = other.tail;
            this->length = other.length;
            other.head = nullptr;
            other.tail = nullptr;
            other.length = 0;
            return *this;
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
        void push_back(T &&value)
        {
            node *newNode = new node(std::move(value), nullptr, this->tail);
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
        void push_front(T &&value)
        {
            node *newNode = new node(std::move(value), this->head, nullptr);
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
        friend std::ostream &operator<<(std::ostream &os, const list &l)
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
        list(list &&other);


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
    list<T>::list(list &&other) : list()
    {
        this->head = other.head;
        this->tail = other.tail;
        this->length = other.length;
        other.head = nullptr;
        other.tail = nullptr;
        other.length = 0;
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

#endif // List_H