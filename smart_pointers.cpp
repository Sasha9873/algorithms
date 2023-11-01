#include <iostream>
 
class IntWrapper {
public:
    IntWrapper(int x) : x(x) {}
    ~IntWrapper() {
        std::cout << "LolKek";
    }
private:
    int x;
};
 
template <typename T, typename U>
struct is_same {
    static constexpr bool v = false;
};
 
template <typename T>
struct is_same<T, T> {
    static constexpr bool v = true;
};
 
template <typename T, typename U>
static constexpr bool is_same_v = is_same<T, U>::v;
 
template <typename T>
struct remove_extent {
    using t = T;
};
 
template <typename T>
struct remove_extent<T[]> {
    using t = T;
};
 
template <typename T>
using remove_extent_t = typename remove_extent<T>::t;
 
template <typename T>
struct ControlBlock {
    using pointer = remove_extent_t<T>*;
    ControlBlock(pointer x) : ptr(x) {
    }
 
    void destroy() {
        if constexpr (is_same_v<remove_extent_t<T>, T>) {
            delete ptr;
        } else {
            delete []ptr;
        }
    }
 
    ControlBlock& operator=(ControlBlock<T>&& other) noexcept {
        if (this != std::addressof(other)) {
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    };
 
    ControlBlock(ControlBlock<T>&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr; // important!!!
    }
 
    ~ControlBlock() {
        destroy();
    }
 
    pointer ptr;
};
 
 
template <typename T>
class UniquePtr {
public:
    using pointer = remove_extent_t<T>*;
    UniquePtr(pointer ptr) : cb(ptr) {
    }
 
    UniquePtr& operator=(const UniquePtr<T>& other) = delete;
    UniquePtr(const UniquePtr<T>& other) = delete;
    UniquePtr& operator=(UniquePtr<T>&& other) noexcept = default;
    UniquePtr(UniquePtr<T>&& other) = default;
 
    ~UniquePtr() = default;
private:
    ControlBlock<T> cb;
};
 
 
template <typename T>
struct SharedControlBlock {
    using pointer = remove_extent_t<T>*;
 
    SharedControlBlock(pointer p)
        : ptr(p), shared_counter(1), weak_counter(0) {
    }
 
    void destroy() {
        if constexpr (is_same_v<remove_extent_t<T>, T>) {
            delete ptr;
        } else {
            delete []ptr;
        }
        ptr = nullptr;
    }
 
    ~SharedControlBlock() {
        destroy();
    }
 
    size_t shared_counter;
    size_t weak_counter;
    pointer ptr;
};
 
template<typename T>
class WeakPtr;
 
template <typename T>
class SharedPtr {
public:
    using pointer = remove_extent_t<T>*;
    using reference = remove_extent_t<T>&;
 
    SharedPtr() : cb(nullptr) {}
    SharedPtr(pointer ptr) : cb(new SharedControlBlock<T>(ptr)) {
    }
 
    SharedPtr(const SharedPtr<T>& other) : cb(other->cb) {
        IncrementCount();
    }
 
    SharedPtr<T>& operator=(const SharedPtr<T>& other) {
        if (this == std::addressof(other)) {
            return *this;
        }
        DecrementCount();
        cb = other.cb;
        IncrementCount();
        return *this;
    }
 
    reference operator*() {
        return *cb->ptr;
    }
 
    pointer operator->() {
        return cb->ptr;
    }
 
    reference operator[](size_t index) {
        return cb->ptr + index;
    }
 
    ~SharedPtr() {
        DecrementCount();
    }
 
 
    friend WeakPtr<T>;
private:
    SharedPtr(const WeakPtr<T>& ptr) : cb(ptr.cb) {
        if (!ptr.Expired()) {
            cb = nullptr;
        } else {
            ++cb->shared_counter;
        }
    }
 
    void DecrementCount() {
        if (cb == nullptr) {
            return;
        }
        --cb->shared_counter;
        if (cb->shared_counter == 0 && cb->weak_counter == 0) {
            delete cb;
        } else if (cb->shared_counter == 0) {
            cb->destroy();
        }
    }
 
    void IncrementCount() {
        if (cb == nullptr) {
            return;
        }
        ++cb->shared_counter;
    }
 
    SharedControlBlock<T>* cb;
};
 
template<typename T>
class WeakPtr {
public:
    using pointer = remove_extent_t<T>*;
    using reference = remove_extent_t<T>&;
    WeakPtr(const SharedPtr<T>& ptr) : cb(ptr->cb) {
        IncCount();
    }
 
    bool Expired() {
        return cb->shared_counter != 0;
    }
 
    SharedPtr<T> Lock() {
        return {*this};
    }
 
    ~WeakPtr() {
 
    }
private:
    void DecCount() {
        if (cb == nullptr) {
            return;
        }
        --cb->weak_counter;
        if (cb->weak_counter == 0 && cb->shared_counter == 0) {
            delete cb;
        }
    }
 
    void IncCount() {
        if (cb == nullptr) {
            return;
        }
        ++cb->weak_counter;
    }
 
    SharedControlBlock<T>* cb;
};
 
 
 
 
struct Node {
    int x;
    std::shared_ptr<Node*> next;
    std::weak_ptr<Node*> prev;  // not shared please
};
 
 
 
/* begin -> a
 * a -> b
 * a <- b
 */
int main() {
    //UniquePtr<IntWrapper> w(new IntWrapper(1));
    //auto w2 = std::move(w);
    //UniquePtr<int[]> w1(new int[100]);
 
    std::shared_ptr<IntWrapper> ptr(new IntWrapper(2));
    auto ptr1 = ptr;
    std::cout << ptr1.use_count();
 
    return 0;
}