#include <iostream>
#include <vector>
#include <memory>
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
        

        template<size_t N>
        void operator=(const char (&str)[N])
        {
            if (this->str != nullptr)
            {
                delete[] this->str;
            }
            this->length = N - 1;
            this->realLength = this->length;
            this->str = new char[this->length];
            for (size_t i = 0; i < this->length; i++)
            {
                this->str[i] = str[i];
            }
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
        
        template <size_t N>
        bool operator==(const char (&str)[N])
        {
            if (this->length != N - 1)
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
        bool operator==(const string &str)
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
        bool operator==(const char *str)
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
        template <typename T>
        bool operator!=(const T &str)
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
        string();
        string(size_t realLength);
        string(const char ch);
        template<size_t N>
        string(const char (&str)[N]);
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
    template<size_t N>
    string::string(const char (&str)[N]) : string(){
        *this = str[N];
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


String::string createObject() {
    String::string obj = "Hello, World!";
    return obj; 
}

int main() {
    String::string myObj = createObject();
    return 0;
}
class MemoryPool {
public:
    MemoryPool(size_t chunkSize, size_t initialSize = 1024) 
        : chunkSize(chunkSize), poolSize(initialSize), freeList(nullptr) {
        expandPool();
    }

    ~MemoryPool() {
        for (auto chunk : pool) {
            delete[] chunk;
        }
    }

    void* allocate() {
        if (!freeList) {
            expandPool();
        }
        void* freeChunk = freeList;
        freeList = *reinterpret_cast<void**>(freeList);
        return freeChunk;
    }

    void deallocate(void* ptr) {
        *reinterpret_cast<void**>(ptr) = freeList;
        freeList = ptr;
    }

private:
    void expandPool() {
        char* newChunk = new char[chunkSize * poolSize];
        pool.push_back(newChunk);

        for (size_t i = 0; i < poolSize; ++i) {
            deallocate(newChunk + i * chunkSize);
        }
    }

    size_t chunkSize;
    size_t poolSize;
    void* freeList;
    std::vector<char*> pool;
};

template <typename T>
class PoolAllocator {
public:
    using value_type = T;

    PoolAllocator(MemoryPool& pool) : pool(pool) {}

    T* allocate(size_t n) {
        if (n != 1) {
            throw std::bad_alloc();
        }
        return static_cast<T*>(pool.allocate());
    }

    void deallocate(T* ptr, size_t n) {
        pool.deallocate(ptr);
    }

private:
    MemoryPool& pool;
};

class MyClass {
public:
    MyClass(int a, double b) : a(a), b(b) {}

    void print() const {
        std::cout << "MyClass(a: " << a << ", b: " << b << ")\n";
    }

private:
    int a;
    double b;
};

int main() {
    constexpr size_t poolSize = 10;
    MemoryPool pool(sizeof(MyClass), poolSize);
    PoolAllocator<MyClass> allocator(pool);

    MyClass* obj = new (allocator.allocate(1)) MyClass(42, 3.14);
    obj->print();
    obj->~MyClass();
    allocator.deallocate(obj, 1);

    return 0;
}
