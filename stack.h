
#ifndef LAB6_STACK_H
#define LAB6_STACK_H
#include <iterator>
#include <memory>

namespace container {

    template<class T,class Allocator = std::allocator<T>>
    class Stack {
    private:
        class StackNode;
    public:
        class ForwardIterator {
        public:
            using value_type = T;
            using reference = T &;
            using pointer = T *;
            using difference_type = ptrdiff_t;
            using iterator_category = std::forward_iterator_tag;

            ForwardIterator(StackNode *node) : Ptr(node) {};

            T &operator*();

            ForwardIterator &operator++();

            ForwardIterator operator++(int);

            bool operator==(const ForwardIterator &it) const;

            bool operator!=(const ForwardIterator &it) const;

        private:
            StackNode *Ptr;

            friend class Stack;
        };

        ForwardIterator begin();

        ForwardIterator end();

        T& Top();

        void Insert(int& index,const T &value );

        void InsertHelp(const ForwardIterator &it, const T &value);

        void Erase(const ForwardIterator &it);

        void Pop();

        void Push(const T &value);

        Stack() = default;

        Stack(const Stack &) = delete;

        Stack &operator=(const Stack &) = delete;

        size_t Size = 0;
    private:
        using allocator_type = typename Allocator::template rebind<StackNode>::other;

        struct deleter {
            deleter(allocator_type* allocator): allocator_(allocator) {}

            void operator() (StackNode* ptr) {
                if(ptr != nullptr){
                    std::allocator_traits<allocator_type>::destroy(*allocator_, ptr);
                    allocator_->deallocate(ptr, 1);
                }
            }

        private:
            allocator_type* allocator_;

        };

        using unique_ptr = std::unique_ptr<StackNode, deleter>;
        class StackNode {
        public:
            T Value;
            unique_ptr NextNode = {nullptr,deleter{&this->allocator_}};

            ForwardIterator next();

            StackNode(const T &value, unique_ptr next) : Value(value), NextNode(std::move(next)) {};
        };

        allocator_type allocator_{};
        unique_ptr Head = {nullptr,deleter{&this->allocator_}};


    };


    template<class T,class Allocator>
    typename Stack<T,Allocator>::ForwardIterator Stack<T,Allocator>::StackNode::next() {
        return {NextNode.get()};
    }

    template<class T,class Allocator>
    T &Stack<T,Allocator>::ForwardIterator::operator*() {
        return Ptr->Value;
    }

    template<class T,class Allocator>
    typename Stack<T,Allocator>::ForwardIterator &Stack<T,Allocator>::ForwardIterator::operator++() {
        *this = Ptr->next();
        return *this;
    }

    template<class T,class Allocator>
    typename Stack<T,Allocator>::ForwardIterator Stack<T,Allocator>::ForwardIterator::operator++(int) {
        ForwardIterator prev = *this;
        ++(*this);
        return prev;
    }

    template<class T,class Allocator>
    bool Stack<T,Allocator>::ForwardIterator::operator!=(const ForwardIterator &other) const {
        return Ptr != other.Ptr;
    }

    template<class T,class Allocator>
    bool Stack<T,Allocator>::ForwardIterator::operator==(const ForwardIterator &other) const {
        return Ptr == other.Ptr;
    }

    template<class T,class Allocator>
    typename Stack<T,Allocator>::ForwardIterator Stack<T,Allocator>::begin() {
        return Head.get();
    }

    template<class T,class Allocator>
    typename Stack<T,Allocator>::ForwardIterator Stack<T,Allocator>::end() {
        return nullptr;
    }


    template<class T,class Allocator>
    void Stack<T,Allocator>::Insert(int &index, const T &value) {
        int id = index - 1;
        if (index < 0 || index > this->Size) {
            throw std::logic_error("Out of bounds\n");
        }
        if (id == -1) {
            this->Push(value);
        }
        else {
            auto it = this->begin();
            for (int i = 0; i < id; ++i) {
                ++it;
            }
            this->InsertHelp(it,value);
        }
    }


    template<class T,class Allocator>
    void Stack<T,Allocator>::InsertHelp(const ForwardIterator &it, const T &value) {
        StackNode* newptr = this->allocator_.allocate(1);
        std::allocator_traits<allocator_type>::construct(this->allocator_,newptr,value,std::unique_ptr<StackNode,deleter>(
                nullptr,deleter{&this->allocator_}));
        unique_ptr newNode(newptr,deleter{&this->allocator_});
        if (it.Ptr == nullptr && Size != 0) {
            throw std::logic_error("Out of bounds");
        }
        if (Size == 0) {
            Head = std::move(newNode);
            ++Size;
        } else {
            newNode->NextNode = std::move(it.Ptr->NextNode);
            it.Ptr->NextNode = std::move(newNode);
            ++Size;
        }
    }

    template<class T,class Allocator>
    void Stack<T,Allocator>::Push(const T &value) {

        StackNode* newptr = this->allocator_.allocate(1);
        std::allocator_traits<allocator_type>::construct(this->allocator_,newptr,value, std::unique_ptr<StackNode,deleter>(
                nullptr,deleter{&this->allocator_}));
        unique_ptr newNode(newptr,deleter{&this->allocator_});
        newNode->NextNode = std::move(Head);
        Head = std::move(newNode);
        ++Size;
    }

    template<class T,class Allocator>
    T& Stack<T,Allocator>::Top() {
        if (Head.get()) {
            return Head->Value;
        } else {
            throw std::logic_error("Stack is empty");
        }
    }

    template<class T,class Allocator>
    void Stack<T,Allocator>::Pop() {
        if (Head.get() == nullptr) {
            throw std::logic_error("Stack is empty");
        }
        Head = std::move(Head->NextNode);
        --Size;
    }

    template<class T,class Allocator>
    void Stack<T,Allocator>::Erase(const Stack<T,Allocator>::ForwardIterator &it) {
        if (it.Ptr == nullptr) {
            throw std::logic_error("Invalid iterator");
        }
        Size--;
        if (it == this->begin()) {
            Head = std::move(Head->NextNode);

        } else {
            auto tmp = this->begin();
            while (tmp.Ptr->next() != it.Ptr) {
                ++tmp;
            }
            tmp.Ptr->NextNode = std::move(it.Ptr->NextNode);
        }
    }

}
#endif //LAB6_STACK_H
