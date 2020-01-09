#ifndef LAB6_QUEUE_H
#define LAB6_QUEUE_H
#ifndef QUEUE_H
#define QUEUE_H
#include <memory>
#include <exception>
#include <cstdint>
typedef unsigned long long ull;
namespace countainer {
    template <typename T, typename my_allocator>
    class queue;
    template <typename T>
    class lst_node;
    template <typename T, typename my_allocator>
    class iterator;

    template <typename T>
    struct lst_node {
        lst_node() = default;
        lst_node(T new_value) : value(new_value) {}
        T value;
        std::shared_ptr<lst_node> next = nullptr;
        std::weak_ptr<lst_node> prev;
    };

    template<typename T, typename my_allocator = std::allocator<T>>
    class queue {
    public:
        using value_type = T;
        using size_type = ull;
        using reference = value_type&;

        friend iterator<T, my_allocator>;

        using allocator_type = typename my_allocator::template rebind<lst_node<T>>::other;

        struct deleter {
            deleter(allocator_type* allocator) : allocator_(allocator) {}
            void operator() (lst_node<T>* ptr) {
                if (ptr != nullptr) {
                    std::allocator_traits<allocator_type>::destroy(*allocator_,ptr);
                    allocator_->deallocate(ptr, 1);
                }
            }
        private:
            allocator_type* allocator_;
        };

    public:
        queue() {
            lst_node<T>* ptr = allocator_.allocate(1);
            std::allocator_traits<allocator_type>::construct(allocator_, ptr);
            std::shared_ptr<lst_node<T>> new_elem(ptr, deleter(&allocator_));
            tail_ = new_elem;
            head_ = tail_;
            size_ = 0;
        }

        queue(const queue& q) = delete;
        queue& operator = (const queue&) = delete;
        void pop() {
            if (empty()) {
                throw std::out_of_range("empty");
            }
            head_ = head_->next;
            size_--;
        }

        reference top() {
            if (empty()) {
                throw std::logic_error("empty");
            }
            return head_->value;
        }

        size_type size() {
            return size_;
        }

        bool empty() {
            return head_ == tail_;
        }

        iterator<T, my_allocator> begin() {
            return iterator<T,my_allocator>(head_, this);
        }

        iterator<T, my_allocator> end() {
            return iterator<T, my_allocator>(tail_, this);
        }

        void push(const T &value) {
            lst_node<T>* ptr = allocator_.allocate(1);
            std::allocator_traits<allocator_type>::construct(allocator_, ptr, value);
            std::shared_ptr<lst_node<T>> new_elem(ptr, deleter(&allocator_));
            if (empty()) {
                head_ = new_elem;
                head_->next = tail_;
                tail_->prev = head_;
            } else {
                tail_->prev.lock()->next = new_elem;
                new_elem->prev = tail_->prev;
                new_elem->next = tail_;
                tail_->prev = new_elem;
            }
            size_++;
        }

        void it_rmv(iterator<T, my_allocator> it) {
            std::shared_ptr<lst_node<T>> tmp = it.item_.lock();
            if (it == end()) {
                throw std::logic_error("can't remove end iterator");
            }
            if (it == begin()) {
                pop();
                return ;
            }
            std::shared_ptr<lst_node<T>> next_tmp = tmp->next;
            std::weak_ptr<lst_node<T>> prev_tmp = tmp->prev;
            prev_tmp.lock()->next = next_tmp;
            next_tmp->prev = prev_tmp;
            size_--;
        }

        void it_insert(iterator<T, my_allocator> it, const T& value) {
            std::shared_ptr <lst_node<T>> it_ptr = it.item_.lock();
            if (it == end()) {
                push(value);
                return;
            }
            lst_node<T>* ptr = allocator_.allocate(1);
            std::allocator_traits<allocator_type>::construct(allocator_, ptr, value);
            std::shared_ptr<lst_node<T>> new_elem(ptr, deleter(&allocator_));
            if (it == begin()) {
                new_elem->next = head_;
                head_->prev = new_elem;
                head_ = new_elem;
                size_++;
                return ;
            }
            std::shared_ptr <lst_node<T>> ptr_next = it_ptr;
            std::weak_ptr <lst_node<T>> ptr_prev = it_ptr -> prev;
            new_elem->prev = ptr_prev;
            ptr_prev.lock()->next = new_elem;
            new_elem->next = ptr_next;
            ptr_next->prev = new_elem;
            size_++;

        }

    private:
        allocator_type allocator_;
        std::shared_ptr<lst_node<T>> head_;
        std::shared_ptr<lst_node<T>> tail_;
        int size_;
    };

    template<typename T, typename my_allocator>
    class iterator {
        friend queue<T, my_allocator>;
    public:
        using value_type = T;
        using reference = T&;
        using pointer = T*;
        using difference_type = ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;

        iterator(std::shared_ptr<lst_node<T>> init_ptr,const queue<T, my_allocator>* ptr) : item_(init_ptr), lst_(ptr) {}

        iterator(const iterator& it) {
            item_ = it.item_;
            lst_ = it.lst_;
        }

        iterator& operator= (const iterator& it) {
            item_ = it.item_;
            return *this;
        }

        iterator& operator++ () {
            std::shared_ptr<lst_node<T>> tmp = item_.lock();
            if (tmp) {
                if (tmp->next == nullptr) {
                    throw std::logic_error("out of bounds");
                }
                tmp = tmp->next;
                item_ = tmp;
                return  *this;
            }
            throw std::logic_error("smt strange");
        }
        iterator operator++ (int) {
            iterator res(*this);
            ++(*this);
            return res;
        }
        reference operator*() {
            return item_.lock()->value;
        }

        pointer operator->() {
            return &item_->value;
        }

        bool operator!= (const iterator& example) {
            return !(*this == example);
        }

        bool operator== (const iterator& example) {
            return item_.lock() == example.item_.lock();
        }

    private:
        std::weak_ptr<lst_node<T>> item_;
        const queue<T, my_allocator>* lst_;
    };
}
#endif
#endif //LAB6_QUEUE_H
