#include <iterator>


template <class T>
class Allocator
{
public:

    T* allocate(size_t n){
        return static_cast<T*>(operator new(n * sizeof(T)));
    }

    void deallocate(T* p, size_t n){
        operator delete(p, n * sizeof(T));
    }

    template <class... Args>
    void construct(T* p, Args&&... args){
        new(p) T(std::forward<Args>(args)...);
    }

    void destroy(T* p){
        p->~T();
    }
};

template <class T>
class Iterator : public std::iterator<std::random_access_iterator_tag, T>
{
private:
    T* current;

public:
    explicit Iterator(T* p) : current(p) {}
    Iterator(const Iterator<T>& it) : current(it.current) {}

    bool operator==(const Iterator<T>& other) const {
        return current == other.current;
    }

    bool operator!=(const Iterator<T>& other) const {
        return !(*this == other);
    }

    bool operator<(const Iterator<T>& other) const {
        return current < other.current;
    }

    bool operator>(const Iterator<T>& other) const {
        return other < *this;
    }

    bool operator>=(const Iterator<T>& other) const {
        return !(*this < other);
    }

    bool operator<=(const Iterator<T>& other) const {
        return !(*this > other);
    }

    T& operator*() const {
        return *current;
    }

    Iterator<T>& operator++(){
        ++current;
        return *this;
    }

    Iterator<T>& operator--(){
        --current;
        return *this;
    }

    Iterator<T>& operator+=(size_t n){
        current += n;
        return *this;
    }

    Iterator<T>& operator-=(size_t n){
        current -= n;
        return *this;
    }

    Iterator<T> operator+(size_t n) const {
        Iterator<T> tmp(current + n);
        return tmp;
    }

    Iterator<T> operator-(size_t n) const {
        Iterator<T> tmp(current - n);
        return tmp;
    }

    Iterator<T> operator+(const Iterator<T>& other){
        Iterator<T> tmp(current + other.current);
        return tmp;
    }

    Iterator<T> operator-(const Iterator<T>& other){
        Iterator<T> tmp(current - other.current);
        return tmp;
    }

    friend Iterator<T> operator+(size_t n, const Iterator<T>& other){
        Iterator<T> tmp(other.current + n);
        return tmp;
    }

    friend Iterator<T> operator-(size_t n, const Iterator<T>& other){
        Iterator<T> tmp(other.current - n);
        return tmp;
    }

    T& operator[](size_t n){
        return current[n];
    }
};

template <class T>
class Vector
{    
private:
    Allocator<T> allocat;
    T* data_;
    size_t cur_size;
    size_t alloc_size;

public:

    explicit Vector(size_t n = 0)
        : data_(allocat.allocate(n)),
        cur_size(n), alloc_size(n) 
    {
        for (size_t i = 0; i < cur_size; ++i)
            allocat.construct(data_ + i);
    }

    ~Vector(){
        clear();
        allocat.deallocate(data_, alloc_size);
    }

    T& operator[] (size_t i){
        return data_[i];
    }

    const T& operator[] (size_t i) const {
        return data_[i];
    }

    void pop_back(){
        --cur_size;
        allocat.destroy(data_ + cur_size);
    }

    void push_back(T &&value){
        if (cur_size >= alloc_size)
            reserve(2 * alloc_size + 1);
        allocat.construct(data_ + cur_size, std::move(value));
        ++cur_size;
    }

    void push_back(const T& value){
        if (cur_size >= alloc_size)
            reserve(2 * alloc_size + 1);
        allocat.construct(data_ + cur_size, value);
        ++cur_size;
    }

    size_t size() const {
        return cur_size;
    }

    size_t capacity(){
        return alloc_size;
    }

    bool empty() const {
        return cur_size == 0;
    }

    Iterator<T> begin() const {
        return Iterator<T>(data_);
    }

    Iterator<T> end() const {
        return Iterator<T>(data_ + cur_size);
    }

    std::reverse_iterator<Iterator<T>> rbegin(){
        return std::reverse_iterator<Iterator<T>>(end());
    }

    std::reverse_iterator<Iterator<T>> rend(){
        return std::reverse_iterator<Iterator<T>>(begin());
    }

    void reserve(size_t new_cap){
        if (new_cap <= cur_size)
            return;

        T* tmp = allocat.allocate(new_cap);

        for (size_t i = 0; i < cur_size; ++i){
            allocat.construct(tmp + i, data_[i]);
            allocat.destroy(data_ + i);
        }
        allocat.deallocate(data_, alloc_size);

        data_ = tmp;
        alloc_size = new_cap;
    }

    void clear(){
        while (cur_size > 0){
            allocat.destroy(data_ + cur_size);
            --cur_size;
        }
    }

    void resize(size_t n){
        if (n < cur_size){
            for (T* p = data_ + n; p != data_ + cur_size; ++p){
                allocat.destroy(p);
            }
            cur_size = n;
            return;
        }

        if(n >= alloc_size)
            this->reserve(n);

        for (T* p = data_ + cur_size; p != data_ + n; ++p) {
            allocat.construct(p);
        }
        cur_size = n;
        return;
    }
};