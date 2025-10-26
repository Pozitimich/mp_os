#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H

#include <stack>
#include <vector>
#include <new>
#include <logger.h>
#include <logger_guardant.h>
#include <allocator.h>
#include <allocator_guardant.h>
#include <search_tree.h>
#include <string>

template<
    typename tkey,
    typename tvalue>
class binary_search_tree:
    public search_tree<tkey, tvalue>
{

protected:
    
    struct node
    {
    public:
        tkey key;
        tvalue value;
        node *left_subtree;
        node *right_subtree;

    public:
        explicit node(
            const tkey &key,
            const tvalue &value);
        
        explicit node(
            const tkey &key,
            tvalue &&value);

        node(const node &other) = default;
        node &operator=(const node &other) = default;

        node(node &&other);
        node &operator=(node &&other);

    public:
        virtual ~node() noexcept = default;
        
    };

protected:
    std::string get_typename() const noexcept;

public:
    #pragma region iterators definition
    
    template<typename traverse_strategy>
    class iterator;

    class iterator_data {
        template<typename traverse_strategy>
        friend class iterator;

        friend void binary_search_tree<tkey, tvalue>::inject_data(
            iterator_data *,
            const std::stack<node*> &) const;

    protected:
        const tkey *_key;
        tvalue *_value;
        unsigned int _depth;
        bool _is_initialized;

        void inject_from_path(const std::stack<node*> &path);

    public:
        iterator_data() noexcept :
            _is_initialized(false) {}

        iterator_data(
            const tkey* &key,
            const tvalue* &value,
            unsigned int depth = 0);

    public:

        const tkey &get_key() const;

        tvalue &get_value();

        const tvalue &get_value() const;

        unsigned int get_depth() const;

        bool is_initialized() const noexcept;

    public:

        iterator_data(
            iterator_data const &other) noexcept = default;
    
        iterator_data(
            iterator_data &&other) noexcept = default;
        
        iterator_data &operator=(
            iterator_data const &other) noexcept = default;
        
        iterator_data &operator=(
            iterator_data &&other) noexcept = default;
            

        virtual ~iterator_data() noexcept;

    };
    
    #pragma region MY_ITER

    template<typename traverse_strategy>
    class iterator {
        traverse_strategy traverse;
    protected:
        std::stack<node*> _path;
        const binary_search_tree<tkey, tvalue> *_holder;
        iterator_data *_data;
    
    public:
        explicit iterator(
            const binary_search_tree<tkey, tvalue> *holder,
            binary_search_tree<tkey, tvalue>::
                node *subtree_root);

        explicit iterator(
            const binary_search_tree<tkey, tvalue> *holder,
            const std::stack<node*> &path);
    
    public:
        iterator(
            iterator<traverse_strategy> const &other);
        
        iterator<traverse_strategy> &operator=(
            const iterator<traverse_strategy> &other) = default;
    
        iterator(
            iterator<traverse_strategy> &&other) noexcept;
        
        iterator<traverse_strategy> &operator=(
            iterator<traverse_strategy> &&other) noexcept = default;

    public:
        bool operator==(
            const iterator<traverse_strategy> &other) const noexcept;
        
        bool operator!=(
            const iterator<traverse_strategy> &other) const noexcept;
        
        iterator<traverse_strategy> &operator++();
        
        const iterator<traverse_strategy> operator++(
            int not_used);
    
        ~iterator() noexcept = default;
    };

    template<typename traverse_strategy>
    class nconst_iterator : public iterator<traverse_strategy> {
    public:
        explicit nconst_iterator(
            const binary_search_tree<tkey, tvalue> *holder,
            binary_search_tree<tkey, tvalue>::
                node *subtree_root) :
                    iterator<traverse_strategy>(holder, subtree_root) {}
        
        explicit nconst_iterator(
            const binary_search_tree<tkey, tvalue> *holder,
            const std::stack<node*> &path) :
                iterator<traverse_strategy>(holder, path) {}
        
        nconst_iterator(
            const nconst_iterator<traverse_strategy> &other) : 
                iterator<traverse_strategy>(other) {}
        
        nconst_iterator(
            nconst_iterator<traverse_strategy> &&other) noexcept : 
                iterator<traverse_strategy>(std::move(other)) {}
        
    public:
        iterator_data* operator*() {
            return this->_data;
        }
    };
    
    template<typename traverse_strategy>
    class const_iterator : public iterator<traverse_strategy> {
    public:
        explicit const_iterator(
            const binary_search_tree<tkey, tvalue> *holder,
            binary_search_tree<tkey, tvalue>::
                node *subtree_root) :
                    iterator<traverse_strategy>(holder, subtree_root) {}

        explicit const_iterator(
            const binary_search_tree<tkey, tvalue> *holder,
            const std::stack<node*> &path) :
                iterator<traverse_strategy>(holder, path) {}
        
        const_iterator(
            const nconst_iterator<traverse_strategy> &other) : 
                iterator<traverse_strategy>(other) {}
        
        const_iterator(
            nconst_iterator<traverse_strategy> &&other) noexcept : 
                iterator<traverse_strategy>(std::move(other)) {}
    public:
        const iterator_data* operator*() const {
            return this->_data;
        }
    };

    #pragma endregion MY_ITER

    #pragma region iterators definition

    #pragma region prefix iterators definition

    struct prefix_traverse {
        void set_start(std::stack<node*> &_path);
        void take_step(std::stack<node*> &_path);
    };

    struct prefix_reverse_traverse {
        void set_start(std::stack<node*> &_path);
        void take_step(std::stack<node*> &_path);
    };

    #pragma endregion prefix iterators definition

    #pragma region infix iterators definition

    struct infix_traverse {
        void set_start(std::stack<node*> &_path);
        void take_step(std::stack<node*> &_path);
    };

    struct infix_reverse_traverse {
        void set_start(std::stack<node*> &_path);
        void take_step(std::stack<node*> &_path);
    };

    #pragma endregion infix iterators definition

    #pragma region postfix iterators definition

    struct postfix_traverse {
        void set_start(std::stack<node*> &_path);
        void take_step(std::stack<node*> &_path);
    };

    struct postfix_reverse_traverse {
        void set_start(std::stack<node*> &_path);
        void take_step(std::stack<node*> &_path);
    };

    #pragma endregion postfix iterators definition
    
    #pragma endregion iterators definition

    #pragma region target operations associated exception types

public:    
    class insertion_of_existent_key_attempt_exception final:
        public std::logic_error
    {
    private:
        tkey _key;
    
    public:
        explicit insertion_of_existent_key_attempt_exception(
            const tkey &key);
        
    public:
        const tkey &get_key() const noexcept;
    
    };
    
    class obtaining_of_nonexistent_key_attempt_exception final:
        public std::logic_error
    {
    private:
        tkey _key;
        
    public:
        explicit obtaining_of_nonexistent_key_attempt_exception(
            const tkey &key);
        
    public:
        const tkey &get_key() const noexcept;
        
    };
    
    class disposal_of_nonexistent_key_attempt_exception final:
        public std::logic_error
    {
    private:
        tkey _key;
    
    public:
        explicit disposal_of_nonexistent_key_attempt_exception(
            const tkey &key);
        
    public:
        const tkey &get_key() const noexcept;
    
    };
    
    #pragma endregion target operations associated exception types
    
    #pragma region template methods definition

protected:
    class template_method_basics:
        public logger_guardant
    {
    protected:
        binary_search_tree<tkey, tvalue> *_tree;
        
    public:
        explicit template_method_basics(
            binary_search_tree<tkey, tvalue> *tree);
        
    protected:
        std::stack<node **> find_path(
            const tkey &key) const;

        virtual void balance(
            std::stack<node **> &path,
            node* node_to_dispose = nullptr);
            
        [[nodiscard]] logger *get_logger() const noexcept override;
        
    };

    class insertion_template_method:
        public template_method_basics,
        public allocator_guardant
    {
    public:
        explicit insertion_template_method(
            binary_search_tree<tkey, tvalue> *tree);
        
    public:
        void insert(
            const tkey &key,
            const tvalue &value);
        
        void insert(
            const tkey &key,
            tvalue &&value);
        
    private:
        [[nodiscard]] allocator *get_allocator() const noexcept final;
    };
    
    class obtaining_template_method:
        public template_method_basics
    {
    public:
        explicit obtaining_template_method(
            binary_search_tree<tkey, tvalue> *tree);
    
    public:
        const tvalue &obtain(
            const tkey &key);

        std::vector<typename associative_container<tkey, tvalue>::
            key_value_pair> obtain_between(
                const tkey &lower_bound,
                const tkey &upper_bound,
                bool lower_bound_inclusive,
                bool upper_bound_inclusive);

    };
    
    class disposal_template_method:
        public template_method_basics,
        public allocator_guardant
    {
    public:
        explicit disposal_template_method(
            binary_search_tree<tkey, tvalue> *tree);
        
    public:
        virtual tvalue dispose(
            const tkey &key);
        
    private:
        [[nodiscard]] allocator *get_allocator() const noexcept final;
    };
    
    #pragma endregion template methods definition

protected:
    insertion_template_method *_insertion_template = nullptr;
    obtaining_template_method *_obtaining_template = nullptr;
    disposal_template_method *_disposal_template = nullptr;

protected:
    
    explicit binary_search_tree(
        typename binary_search_tree<tkey, tvalue>::
            insertion_template_method *insertion_template,
        typename binary_search_tree<tkey, tvalue>::
            obtaining_template_method *obtaining_template,
        typename binary_search_tree<tkey, tvalue>::
            disposal_template_method *disposal_template,
        std::function<int(const tkey &, const tkey &)> comparer,
        allocator *allocator,
        logger *logger,
        typename search_tree<tkey, tvalue>::
            insertion_of_existent_key_attempt_strategy insertion_strategy,
        typename search_tree<tkey, tvalue>::
            disposal_of_nonexistent_key_attempt_strategy disposal_strategy);

public:
    
    explicit binary_search_tree(
        std::function<int(const tkey &, const tkey &)> 
            comparer = std::less<tkey>(),
        allocator *allocator = nullptr,
        logger *logger = nullptr,
        typename binary_search_tree<tkey, tvalue>::
            insertion_of_existent_key_attempt_strategy 
                insertion_strategy = 
                binary_search_tree<tkey, tvalue>::
                insertion_of_existent_key_attempt_strategy::
                throw_an_exception,
        typename binary_search_tree<tkey, tvalue>::
            disposal_of_nonexistent_key_attempt_strategy 
                disposal_strategy = 
                binary_search_tree<tkey, tvalue>::
                disposal_of_nonexistent_key_attempt_strategy::
                throw_an_exception);

public:
    
    binary_search_tree(
        binary_search_tree<tkey, tvalue> const &other);
    
    binary_search_tree(
        binary_search_tree<tkey, tvalue> &&other) noexcept;
    
    binary_search_tree<tkey, tvalue> &operator=(
        binary_search_tree<tkey, tvalue> const &other);
    
    binary_search_tree<tkey, tvalue> &operator=(
        binary_search_tree<tkey, tvalue> &&other) noexcept;
    
    ~binary_search_tree() override;

private:

    void clear(node *&subtree_root);

    node *copy(const node *subtree_root);

    virtual size_t get_node_size() const noexcept;

    virtual void update_node_data(node *node) const noexcept;

    virtual void call_node_constructor(
        node *raw_space,
        const tkey &key,
        const tvalue &value);

    virtual void call_node_constructor(
        node *raw_space,
        const tkey &key,
        tvalue &&value);

    virtual void call_node_constructor(
        node* raw_space,
        const node &src);

    virtual void call_node_constructor(
        node* raw_space,
        node &&src);

    virtual void inject_data(
        iterator_data *destination,
        const std::stack<node*> &path) const;

    virtual iterator_data *create_iterator_data() const;

    virtual iterator_data *create_iterator_data(
        const std::stack<node*> &path) const;

public:
    
    void insert(
        const tkey &key,
        const tvalue &value) final;
    
    void insert(
        const tkey &key,
        tvalue &&value) final;
    
    const tvalue &obtain(
        const tkey &key) final;
    
    std::vector<typename associative_container<tkey, tvalue>::key_value_pair> obtain_between(
        const tkey &lower_bound,
        const tkey &upper_bound,
        bool lower_bound_inclusive,
        bool upper_bound_inclusive) final;
    
    tvalue dispose(
        const tkey &key) final;
    
public:
    
    #pragma region iterators requests definition
    
    nconst_iterator<prefix_traverse> begin_prefix() const noexcept;
    nconst_iterator<prefix_traverse> end_prefix() const noexcept;
    
    const_iterator<prefix_traverse> cbegin_prefix() const noexcept;
    const_iterator<prefix_traverse> cend_prefix() const noexcept;
    
    nconst_iterator<prefix_reverse_traverse> rbegin_prefix() const noexcept;
    nconst_iterator<prefix_reverse_traverse> rend_prefix() const noexcept;
    
    const_iterator<prefix_reverse_traverse> crbegin_prefix() const noexcept;
    const_iterator<prefix_reverse_traverse> crend_prefix() const noexcept;
    
    nconst_iterator<infix_traverse> begin_infix() const noexcept;
    nconst_iterator<infix_traverse> end_infix() const noexcept;
    
    const_iterator<infix_traverse> cbegin_infix() const noexcept;
    const_iterator<infix_traverse> cend_infix() const noexcept;
    
    nconst_iterator<infix_reverse_traverse> rbegin_infix() const noexcept;
    nconst_iterator<infix_reverse_traverse> rend_infix() const noexcept;
    
    const_iterator<infix_reverse_traverse> crbegin_infix() const noexcept;
    const_iterator<infix_reverse_traverse> crend_infix() const noexcept;
    
    nconst_iterator<postfix_traverse> begin_postfix() const noexcept;
    nconst_iterator<postfix_traverse> end_postfix() const noexcept;
    
    const_iterator<postfix_traverse> cbegin_postfix() const noexcept;
    const_iterator<postfix_traverse> cend_postfix() const noexcept;
    
    nconst_iterator<postfix_reverse_traverse> rbegin_postfix() const noexcept;
    nconst_iterator<postfix_reverse_traverse> rend_postfix() const noexcept;
    
    const_iterator<postfix_reverse_traverse> crbegin_postfix() const noexcept;
    const_iterator<postfix_reverse_traverse> crend_postfix() const noexcept;
    
    #pragma endregion iterators requests definition

protected:
    
    #pragma region subtree rotations definition
    
    void small_left_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool validate = true) const;
    
    void small_right_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool validate = true) const;
    
    void big_left_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool validate = true) const;
    
    void big_right_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool validate = true) const;
    
    void double_left_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool at_grandparent_first,
        bool validate = true) const;
    
    void double_right_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool at_grandparent_first,
        bool validate = true) const;
    
    #pragma endregion subtree rotations definition
    
};

#pragma region binary_search_tree<tkey, tvalue>::node methods implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::node::
node(
    tkey const &key,
    tvalue const &value):
        key(key),
        value(value),
        left_subtree(nullptr),
        right_subtree(nullptr)
{}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::node::
node(
    tkey const &key,
    tvalue &&value):
        key(key),
        value(std::move(value)),
        left_subtree(nullptr),
        right_subtree(nullptr)
{}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::node::
node(node &&other) : 
    key(other.key), 
    value(std::move(other.value)),
    left_subtree(other.left_subtree),
    right_subtree(other.right_subtree) 
{}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::node
&binary_search_tree<tkey, tvalue>::node::
operator=(node &&other) {
    key = other.key;
    value = std::move(other.value);
    left_subtree = other.left_subtree;
    right_subtree = other.right_subtree;
    return this;
} 

#pragma endregion binary_search_tree<tkey, tvalue>::node methods implementation

#pragma region iter_data implementation

template<typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::iterator_data::
inject_from_path(const std::stack<node*> &path) {
    if (path.empty() || path.top() == nullptr) {
        _is_initialized = false;
        return;
    }
    _is_initialized = true;
    _key = &path.top()->key;
    _value = &path.top()->value;
    _depth = path.size() - 1;
}

template<typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::iterator_data::
iterator_data(
    const tkey* &key,
    const tvalue* &value,
    unsigned int depth)
{
    if (key == nullptr || value == nullptr) {
        _is_initialized = false;
        return; 
    }
    _is_initialized = true;
    _key = key;
    _value = value;
    _depth = depth;
}


template<typename tkey, typename tvalue>
const tkey &binary_search_tree<tkey, tvalue>::iterator_data::
get_key() const {
    if (is_initialized()) {
        return *_key;
    }
    throw std::logic_error("[BST] [Iterator Data] [Get Key] Iterator data is not initialized\n");
}

template<typename tkey, typename tvalue>
const tvalue &binary_search_tree<tkey, tvalue>::iterator_data::
get_value() const {
    if (is_initialized()) {
        return *_value;
    }
    throw std::logic_error("[BST] [Iterator Data] [Get Key] Iterator data is not initialized\n");
}

template<typename tkey, typename tvalue>
tvalue &binary_search_tree<tkey, tvalue>::iterator_data::
get_value() {
    if (is_initialized()) {
        return *_value;
    }
    throw std::logic_error("[BST] [Iterator Data] [Get Key] Iterator data is not initialized\n");
}

template<
    typename tkey,
    typename tvalue>
unsigned int binary_search_tree<tkey, tvalue>::iterator_data::
get_depth() const {
    if (_is_initialized) {
        return _depth;
    }
    
    throw std::logic_error("Tried to read from uninitialized iterator data");
}

template<typename tkey, typename tvalue>
bool binary_search_tree<tkey, tvalue>::iterator_data::
is_initialized() const noexcept {
    return _is_initialized;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::iterator_data::
~iterator_data() noexcept {}

#pragma endregion iter_data implementation

#pragma region iterator implementation

template<typename tkey, typename tvalue>
template<typename traverse_strategy>
binary_search_tree<tkey, tvalue>::iterator<traverse_strategy>::
iterator(
    const binary_search_tree<tkey, tvalue> *holder,
    binary_search_tree<tkey, tvalue>::
        node *subtree_root
) : _holder(holder) 
{
    if (subtree_root != nullptr) {
        _path.push(subtree_root);
        traverse.set_start(_path);
    }
    this->_data = holder->create_iterator_data(_path);
}

template<typename tkey, typename tvalue>
template<typename traverse_strategy>
binary_search_tree<tkey, tvalue>::iterator<traverse_strategy>::
iterator(
    const binary_search_tree<tkey, tvalue> *holder,
    const std::stack<node*> &path
) : _holder(holder), _path(path) 
{    
    this->_data = holder->create_iterator_data(_path);
} 

template<typename tkey, typename tvalue>
template<typename traverse_strategy>
binary_search_tree<tkey, tvalue>::iterator<traverse_strategy>::
iterator(
    iterator<traverse_strategy> const &other
) : _path(other._path), 
    _holder(other._holder)
{
    if (_path.empty()) {
        _data = _holder->create_iterator_data();
    }
    else {
        _data = _holder->create_iterator_data();
    }
}

template<typename tkey, typename tvalue>
template<typename traverse_strategy>
bool binary_search_tree<tkey, tvalue>::iterator<traverse_strategy>::
operator==(iterator<traverse_strategy> const &other) const noexcept {
    if (_path.empty() != other._path.empty()) {
        return false;
    }
    if (_path.empty()) {
        return true;
    }
    return _path.top() == other._path.top();
}

template<typename tkey, typename tvalue>
template<typename traverse_strategy>
bool binary_search_tree<tkey, tvalue>::iterator<traverse_strategy>::
operator!=(iterator<traverse_strategy> const &other) const noexcept {
    return !(*this == other);
}

template<typename tkey, typename tvalue>
template<typename traverse_strategy>
typename binary_search_tree<tkey, tvalue>::iterator<traverse_strategy>
&binary_search_tree<tkey, tvalue>::iterator<traverse_strategy>::
operator++() {
    traverse.take_step(_path);
    _holder->inject_data(_data, _path);

    return *this;
}

template<typename tkey, typename tvalue>
template<typename traverse_strategy>
const typename binary_search_tree<tkey, tvalue>::iterator<traverse_strategy>
binary_search_tree<tkey, tvalue>::iterator<traverse_strategy>::
operator++(int not_used) {
    auto tmp = *this;
    ++*this;
    return tmp;
}

#pragma endregion iterator implementation

#pragma region iterators operator* implementation


#pragma endregion iterator operator* 

#pragma region prefix_traverse implementation

template<typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::prefix_traverse::
set_start(
    std::stack<typename binary_search_tree<tkey, tvalue>::node*>& _path
) {
    return;
}

template<typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::prefix_traverse::
take_step(
    std::stack<typename binary_search_tree<tkey, tvalue>::node*> &_path
) { 
    if (_path.empty()) {
        return; 
    }
    
    if (_path.top()->left_subtree != nullptr) {
        _path.push(_path.top()->left_subtree);
    }
    else if (_path.top()->right_subtree != nullptr) {
        _path.push(_path.top()->right_subtree);
    }
    else {
        while (true) {
            node* current = _path.top();
            _path.pop();
            
            if (_path.empty()) {
                return;
            }
            
            if (_path.top()->left_subtree == current 
                    && _path.top()->right_subtree != nullptr) {
                _path.push(_path.top()->right_subtree);
                return;
            }
        }
    }
}

#pragma endregion prefix_traverse implementation

#pragma region prefix_reverse_traverse implementation

template<typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::prefix_reverse_traverse
::set_start(
    std::stack<typename binary_search_tree<tkey, tvalue>::node*>& _path
) {
    if (_path.empty()) {
        return; 
    }

    while (true) {
        if (_path.top()->right_subtree != nullptr) {
            _path.push(_path.top()->right_subtree);
        }
        if (_path.top()->left_subtree != nullptr) {
            _path.push(_path.top()->left_subtree);
        }
        break;
    }
    
}

template<typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::prefix_reverse_traverse
::take_step(
    std::stack<typename binary_search_tree<tkey, tvalue>::node*>& _path
) {
    if (_path.empty()) {
        return; 
    }

    node* prev = _path.top();
    _path.pop();
    if (_path.empty()) {
        return;
    }
    if (_path.top()->left_subtree == nullptr ||
            _path.top()->left_subtree == prev)
    {
        return;
    }
    _path.push(_path.top()->left_subtree);

    while(true) {
        if (_path.top()->right_subtree != nullptr) {
            _path.push(_path.top()->right_subtree);
        }
        if (_path.top()->left_subtree != nullptr) {
            _path.push(_path.top()->left_subtree);
        }
        break; 
    }
}

#pragma endregion prefix_reverse_traverse implementation

#pragma region infix_traverse implementation

template<typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::infix_traverse
::set_start(
    std::stack<typename binary_search_tree<tkey, tvalue>::node*>& _path
) {
    if (_path.empty()) {
        return;
    }
    while (_path.top()->left_subtree != nullptr) {
        _path.push(_path.top()->left_subtree);
    }
}

template<typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::infix_traverse
::take_step(
    std::stack<typename binary_search_tree<tkey, tvalue>::node*>& _path
) {
    if (_path.empty()) {
        return;
    }

    if (_path.top()->right_subtree != nullptr) {
        _path.push(_path.top()->right_subtree);
        while (_path.top()->left_subtree != nullptr) {
            _path.push(_path.top()->left_subtree);
        }
        return;
    }
    while (true) {
        node *prev = _path.top();
        _path.pop();
        if (_path.empty()) {
            return;
        }
        if (_path.top()->left_subtree == prev) {
            return;
        }
    }
}

#pragma endregion infix_traverse implementation

#pragma region infix_reverse_traverse implementation

template<typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::infix_reverse_traverse
::set_start(
    std::stack<typename binary_search_tree<tkey, tvalue>::node*>& _path
) {
    if (_path.empty()) {
        return;
    }
    while (_path.top()->right_subtree != nullptr) {
        _path.push(_path.top()->right_subtree);
    }
}

template<typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::infix_reverse_traverse
::take_step(
    std::stack<typename binary_search_tree<tkey, tvalue>::node*>& _path
) {
    if (_path.empty()) {
        return;
    }

    if (_path.top()->left_subtree != nullptr) {
        _path.push(_path.top()->left_subtree);
        while (_path.top()->right_subtree != nullptr) {
            _path.push(_path.top()->right_subtree);
        }
        return;
    }
    while (true) {
        node *prev = _path.top();
        _path.pop();
        if (_path.empty()) {
            return;
        }
        if (_path.top()->right_subtree == prev) {
            return;
        }
    }
}

#pragma endregion infix_reverse_traverse implementation

#pragma region postfix_traverse implementation

template<typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::postfix_traverse
::set_start(
    std::stack<typename binary_search_tree<tkey, tvalue>::node*>& _path
) {
    if (_path.empty()) {
        return; 
    }

    while (true) {
        if (_path.top()->left_subtree != nullptr) {
            _path.push(_path.top()->left_subtree);
        }
        else if (_path.top()->right_subtree != nullptr) {
            _path.push(_path.top()->right_subtree);
        }
        else {
            break;
        }
    }
    
}

template<typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::postfix_traverse
::take_step(
    std::stack<typename binary_search_tree<tkey, tvalue>::node*>& _path
) {
    if (_path.empty()) {
        return; 
    }

    node* prev = _path.top();
    _path.pop();
    if (_path.empty()) {
        return;
    }
    if (_path.top()->right_subtree == nullptr ||
        _path.top()->right_subtree == prev)
    {
        return;
    }
    _path.push(_path.top()->right_subtree);

    while(true) {
        if (_path.top()->left_subtree != nullptr) {
            _path.push(_path.top()->left_subtree);
        }
        else if (_path.top()->right_subtree != nullptr) {
            _path.push(_path.top()->right_subtree);
        }
        else {
            break;
        } 
    }
}

#pragma endregion postfix_traverse implementation

#pragma region postfix_reverse_traverse implementation

template<typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::postfix_reverse_traverse::
set_start(
    std::stack<typename binary_search_tree<tkey, tvalue>::node*>& _path
) {
    return;
}

template<typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::postfix_reverse_traverse::
take_step(
    std::stack<typename binary_search_tree<tkey, tvalue>::node*> &_path
) { 
    if (_path.empty()) {
        return; 
    }
    
    if (_path.top()->right_subtree != nullptr) {
        _path.push(_path.top()->right_subtree);
        return;
    }
    if (_path.top()->left_subtree != nullptr) {
        _path.push(_path.top()->left_subtree);
        return;
    }
    while (true) {
        node* current = _path.top();
        _path.pop();
        
        if (_path.empty()) {
            return;
        }
        
        if (_path.top()->right_subtree == current 
                && _path.top()->left_subtree != nullptr) {
            _path.push(_path.top()->left_subtree);
            return;
        }
    }
}

#pragma endregion postfix_reverse_traverse implementation

#pragma region template methods implementation

#pragma region binary_search_tree<tkey, tvalue>::template_method_basics implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::template_method_basics::template_method_basics(
    binary_search_tree<tkey, tvalue> *tree):
        _tree(tree)
{ }

template<
    typename tkey,
    typename tvalue>
std::stack<typename binary_search_tree<tkey, tvalue>::node**> binary_search_tree<tkey, tvalue>::template_method_basics::find_path(
    const tkey &key) const
{
    auto const &comparer = _tree->_keys_comparer;
    std::stack<node**> result_path;
    
    node **path_finder = reinterpret_cast<node**>(&(_tree->_root));
    int comparison_result;
    
    while (true)
    {
        result_path.push(path_finder);
        
        if (*path_finder == nullptr || (comparison_result = comparer(key, (*path_finder)->key)) == 0)
        {
            break;
        }
        
        path_finder = comparison_result < 0
                ? &((*path_finder)->left_subtree)
                : &((*path_finder)->right_subtree);
    }
    
    return result_path;
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::template_method_basics::balance(
    std::stack<node**> &path,
    node* node_to_dispose)
{ }

template<
    typename tkey,
    typename tvalue>
logger *binary_search_tree<tkey, tvalue>::template_method_basics::
get_logger() const noexcept {
    return this->_tree->get_logger();
}

#pragma endregion binary_search_tree<tkey, tvalue>::template_method_basics implementation

#pragma region search_tree<tkey, tvalue>::insertion_template_method implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(
    binary_search_tree<tkey, tvalue> *tree):
        binary_search_tree<tkey, tvalue>::template_method_basics::template_method_basics(tree)
{ }

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insertion_template_method::insert(
    const tkey &key,
    const tvalue &value)
{
    std::stack<node**> path = this->find_path(key);
    
    if (*(path.top()) != nullptr)
    {
        switch (this->_tree->_insertion_strategy)
        {
            case search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception:
                throw typename search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_exception(key);
            case search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::update_value:
                (*(path.top()))->value = value;
                break;
        }
        
        return;
    }
    
    *(path.top()) = reinterpret_cast<node*>(allocate_with_guard(this->_tree->get_node_size(), 1));
    this->_tree->call_node_constructor(*(path.top()), key, value);
    
    this->balance(path);
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insertion_template_method::insert(
    const tkey &key,
    tvalue &&value)
{
    std::stack<node**> path = this->find_path(key);
    
    if (*(path.top()) != nullptr)
    {
        switch (this->_tree->_insertion_strategy)
        {
            case search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception:
                throw typename search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_exception(key);
            case search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::update_value:
                (*(path.top()))->value = std::move(value);
                break;
        }
        
        return;
    }
    
    *(path.top()) = reinterpret_cast<node*>(allocate_with_guard(this->_tree->get_node_size(), 1));
    this->_tree->call_node_constructor(*(path.top()), key, std::move(value));
    
    this->balance(path);
}

template<
    typename tkey,
    typename tvalue>
allocator *binary_search_tree<tkey, tvalue>::insertion_template_method::
get_allocator() const noexcept {
    return this->_tree->get_allocator();
}

#pragma endregion search_tree<tkey, tvalue>::insertion_template_method implementation

#pragma region search_tree<tkey, tvalue>::obtaining_template_method implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::obtaining_template_method::
obtaining_template_method(binary_search_tree<tkey, tvalue> *tree):
        binary_search_tree<tkey, tvalue>::
            template_method_basics::template_method_basics(tree)
{ }

template<
    typename tkey,
    typename tvalue>
const tvalue &binary_search_tree<tkey, tvalue>::
obtaining_template_method::obtain(
    const tkey &key)
{
    std::stack<node**> path = this->find_path(key);
    
    if (*(path.top()) == nullptr) {
        throw typename search_tree<tkey, tvalue>::obtaining_of_nonexistent_key_attempt_exception(key);
    }
    
    const tvalue &got_value = (*(path.top()))->value;
    
    this->balance(path);
    
    return got_value;
}

template<
    typename tkey,
    typename tvalue>
std::vector<typename associative_container<tkey, tvalue>::key_value_pair> binary_search_tree<tkey, tvalue>::obtaining_template_method::obtain_between(
    const tkey &lower_bound,
    const tkey &upper_bound,
    bool lower_bound_inclusive,
    bool upper_bound_inclusive)
{
    auto const &comparer = this->_tree->_keys_comparer;
    std::vector<typename associative_container<tkey, tvalue>::key_value_pair> range;
    std::stack<node*> path;

    node *path_finder = reinterpret_cast<node*>(this->_tree->_root);
    int comparison_result;
    
    while (true) {
        path.push(path_finder);
        
        if (path_finder == nullptr) {
            break;
        }
        comparison_result = comparer(lower_bound, path_finder->key);
        if (comparison_result == 0) {
            break;
        }
        
        path_finder = comparison_result < 0
                ? path_finder->left_subtree
                : path_finder->right_subtree;
    }
    
    if (path.top() == nullptr) {
        path.pop();
        
        // if came from right subtree
        if (!path.empty() && comparer(lower_bound, path.top()->key) == 1) {
            do {
                path_finder = path.top();
                path.pop();
            }
            while (!path.empty() && path.top()->right_subtree == path_finder);
        }
    }
    
    if (path.empty()) {
        return range;
    }
    
    if (lower_bound_inclusive) {
        node *top = path.top();
        range.push_back({ top->key, top->value });
    }
    
    auto iter = const_iterator<infix_traverse>(this->_tree, path);
    ++iter;
    
    while(iter != this->_tree->cend_infix() && 
        (comparer((*iter)->get_key(), upper_bound)) < 
            (upper_bound_inclusive ? 1 : 0))
    {
        range.push_back({ (*iter)->get_key(), (*iter)->get_value() });
        ++iter;
    }
    
    return range;
}

#pragma endregion search_tree<tkey, tvalue>::obtaining_template_method implementation

#pragma region search_tree<tkey, tvalue>::disposal_template_method implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(
    binary_search_tree<tkey, tvalue> *tree):
        binary_search_tree<tkey, tvalue>::template_method_basics(tree)
{ }

template<
    typename tkey,
    typename tvalue>
tvalue binary_search_tree<tkey, tvalue>::disposal_template_method::dispose(
    const tkey &key)
{
    std::stack<node**> path = this->find_path(key);
    
    if (*(path.top()) == nullptr)
    {
        switch (this->_tree->_disposal_strategy)
        {
            case search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception:
                throw typename search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_exception(key);
            case search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::do_nothing:
                return tvalue();
        }
    }
    
    if ((*path.top())->left_subtree != nullptr && (*path.top())->right_subtree != nullptr)
    {
        auto *node_to_dispose = *(path.top());
        auto **current_node = &((*(path.top()))->left_subtree);
        
        while (*current_node != nullptr)
        {
            path.push(current_node);
            current_node = &((*current_node)->right_subtree);
        }
        
        std::swap(node_to_dispose->key, (*(path.top()))->key);
        std::swap(node_to_dispose->value, (*(path.top()))->value);
    }
    
    tvalue value = std::move((*(path.top()))->value);
    node *subtree = (*(path.top()))->left_subtree == nullptr
            ? (*path.top())->right_subtree
            : (*path.top())->left_subtree;
    
    node *node_to_dispose = *(path.top());
    
    *(path.top()) = subtree;
    this->balance(path, node_to_dispose);
    
    allocator::destruct(node_to_dispose);
    deallocate_with_guard(node_to_dispose);
    
    return value;
}

template<
    typename tkey,
    typename tvalue>
allocator *binary_search_tree<tkey, tvalue>::disposal_template_method::
get_allocator() const noexcept {
    return this->_tree->get_allocator();
}

#pragma endregion search_tree<tkey, tvalue>::disposal_template_method implementation

#pragma endregion template methods

#pragma region bst construction, assignment, destruction implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
    typename binary_search_tree<tkey, tvalue>::
        insertion_template_method *insertion_template,
    typename binary_search_tree<tkey, tvalue>::
        obtaining_template_method *obtaining_template,
    typename binary_search_tree<tkey, tvalue>::
        disposal_template_method *disposal_template,
    std::function<int(const tkey &, const tkey &)> comparer,
    allocator *allocator,
    logger *logger,
    typename search_tree<tkey, tvalue>::
        insertion_of_existent_key_attempt_strategy insertion_strategy,
    typename search_tree<tkey, tvalue>::
        disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
        search_tree<tkey, tvalue>(
            comparer, allocator, logger, 
            insertion_strategy, disposal_strategy),
        _insertion_template(insertion_template),
        _obtaining_template(obtaining_template),
        _disposal_template(disposal_template)
{ }

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
    std::function<int(const tkey &, const tkey &)> 
            comparer,
        allocator *allocator,
        logger *logger,
        typename binary_search_tree<tkey, tvalue>::
            insertion_of_existent_key_attempt_strategy 
                insertion_strategy,
        typename binary_search_tree<tkey, tvalue>::
            disposal_of_nonexistent_key_attempt_strategy 
                disposal_strategy) :
        search_tree<tkey, tvalue>(comparer, allocator, 
            logger, insertion_strategy, disposal_strategy)
{
    try
    {
        _insertion_template = new binary_search_tree<tkey, tvalue>::insertion_template_method(this);
        _obtaining_template = new binary_search_tree<tkey, tvalue>::obtaining_template_method(this);
        _disposal_template = new binary_search_tree<tkey, tvalue>::disposal_template_method(this);
    }
    catch (const std::bad_alloc& ex)
    {
        delete _insertion_template;
        delete _obtaining_template;
        delete _disposal_template;
        throw;
    }
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
    binary_search_tree<tkey, tvalue> const &other):
        search_tree<tkey, tvalue>(other._keys_comparer, other.get_allocator(), other.get_logger(),
                other._insertion_strategy, other._disposal_strategy)
{
    try
    {
        this->_root = copy(reinterpret_cast<node*>(other._root));
        _insertion_template = new binary_search_tree<tkey, tvalue>::insertion_template_method(this);
        _obtaining_template = new binary_search_tree<tkey, tvalue>::obtaining_template_method(this);
        _disposal_template = new binary_search_tree<tkey, tvalue>::disposal_template_method(this);
    }
    catch (const std::bad_alloc& ex)
    {
        clear(reinterpret_cast<node*&>(this->_root));
        delete _insertion_template;
        delete _obtaining_template;
        delete _disposal_template;
        throw;
    }
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
    binary_search_tree<tkey, tvalue> &&other) noexcept:
    search_tree<tkey, tvalue>(other._keys_comparer, other.get_allocator(), other.get_logger(),
            other._insertion_strategy, other._disposal_strategy)
{
    this->_root = other._root;
    
    _insertion_template = other._insertion_template;
    _obtaining_template = other._obtaining_template;
    _disposal_template = other._disposal_template;
    
    other._logger = nullptr;
    other._allocator = nullptr;
    other._root = nullptr;
    other._insertion_template = nullptr;
    other._obtaining_template = nullptr;
    other._disposal_template = nullptr;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue> &binary_search_tree<tkey, tvalue>::operator=(
    binary_search_tree<tkey, tvalue> const &other)
{
    if (this != &other)
    {
        clear(reinterpret_cast<node*&>(this->_root));
        this->_root = copy(reinterpret_cast<node*>(other._root));
        
        this->_keys_comparer = other._keys_comparer;
        this->_allocator = other._allocator;
        this->_logger = other._logger;
        
        this->_insertion_strategy = other._insertion_strategy;
        this->_disposal_strategy = other._disposal_strategy;
        
        *_insertion_template = *(other._insertion_template);
        *_obtaining_template = *(other._obtaining_template);
        *_disposal_template = *(other._disposal_template);
        
    }
    
    return *this;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue> &binary_search_tree<tkey, tvalue>::operator=(
    binary_search_tree<tkey, tvalue> &&other) noexcept
{
    if (this != &other)
    {
        clear(reinterpret_cast<node*&>(this->_root));
        delete _insertion_template;
        delete _obtaining_template;
        delete _disposal_template;
        
        this->_keys_comparer = std::move(other._keys_comparer);
        this->_allocator = other._allocator;
        this->_logger = other._logger;
        this->_root = other._root;
        this->_insertion_strategy = other._insertion_strategy;
        this->_disposal_strategy = other._disposal_strategy;
        
        _insertion_template = other._insertion_template;
        _obtaining_template = other._obtaining_template;
        _disposal_template = other._disposal_template;
        
        other._allocator = nullptr;
        other._logger = nullptr;
        other._root = nullptr;
        other._insertion_template = nullptr;
        other._obtaining_template = nullptr;
        other._disposal_template = nullptr;
    }
    
    return *this;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::~binary_search_tree()
{
    clear(reinterpret_cast<node*&>(this->_root));
    delete _insertion_template;
    delete _obtaining_template;
    delete _disposal_template;
}

#pragma endregion bst construction, assignment, destruction implementation

#pragma region bst extra functions implementation

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::clear(
    node* &subtree_root)
{
    if (subtree_root == nullptr)
    {
        return;
    }
    
    clear(subtree_root->left_subtree);
    clear(subtree_root->right_subtree);
    subtree_root->~node();
    this->deallocate_with_guard(subtree_root);
    
    subtree_root = nullptr;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::node *binary_search_tree<tkey, tvalue>::copy(
    const node *subtree_root)
{
    if (subtree_root == nullptr) {
        return nullptr;
    }
    
    node *subtree_root_copied = nullptr;
    
    try {
        subtree_root_copied = reinterpret_cast<node*>(this->allocate_with_guard(this->get_node_size(), 1));
        call_node_constructor(subtree_root_copied, *subtree_root);
        subtree_root_copied->left_subtree = copy(subtree_root->left_subtree);
        subtree_root_copied->right_subtree = copy(subtree_root->right_subtree);
    }
    catch (const std::bad_alloc& ex) {
        clear(subtree_root_copied);
        this->error_with_guard(get_typename() + "::copy(node const *) : bad alloc occurred while copying node.");
        throw;
    }
    
    return subtree_root_copied;
}

template<
    typename tkey,
    typename tvalue>
inline size_t binary_search_tree<tkey, tvalue>::get_node_size() const noexcept
{
    return sizeof(typename binary_search_tree<tkey, tvalue>::node);
}

template<
    typename tkey,
    typename tvalue>
inline void binary_search_tree<tkey, tvalue>::update_node_data(node *node) const noexcept
{ }

template<
    typename tkey,
    typename tvalue>
inline void binary_search_tree<tkey, tvalue>::call_node_constructor(
    node *raw_space,
    const tkey &key,
    const tvalue &value)
{
    allocator::construct(raw_space, key, value);
}

template<
    typename tkey,
    typename tvalue>
inline void binary_search_tree<tkey, tvalue>::call_node_constructor(
    node *raw_space,
    const tkey &key,
    tvalue &&value)
{
    allocator::construct(raw_space, key, std::move(value));
}

template<
    typename tkey,
    typename tvalue>
inline void binary_search_tree<tkey, tvalue>::
call_node_constructor(
    node *raw_space,
    const node &src
) {
    allocator::construct(raw_space, src);
}

template<
    typename tkey,
    typename tvalue>
inline void binary_search_tree<tkey, tvalue>::
call_node_constructor(
    node *raw_space,
    node &&src
) {
    allocator::construct(raw_space, std::move(src));
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::inject_data(
    iterator_data *destination,
    const std::stack<node*> &path
) const {
    destination->inject_from_path(path);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::
create_iterator_data() const {
    return new iterator_data;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::
create_iterator_data (
    const std::stack<node*> &path
) const {
    iterator_data* result = new iterator_data;
    inject_data(result, path);
    return result;
}

#pragma endregion bst extra functions implementation

#pragma region associative_container<tkey, tvalue> contract implementation

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insert(
    const tkey &key,
    const tvalue &value)
{
    // this->trace_with_guard(get_typename() + "::insert(const tkey &, const tvalue &) : called.")
    //     ->debug_with_guard(get_typename() + "::insert(const tkey &, const tvalue &) : called.")
    //     ->debug_with_guard(get_typename() + "::insert(const tkey &, const tvalue &) : inserting node with key \"" + 
    //         std::to_string(key) + "\"");
        
    _insertion_template->insert(key, value);
    
    // this->debug_with_guard(get_typename() + "::insert(const tkey &, const tvalue &) : now root has with key \"" + 
    //         std::to_string(reinterpret_cast<node*>(this->_root)->key) + "\"")
    //     ->trace_with_guard(get_typename() + "::insert(const tkey &, const tvalue &) : successfuly finished.")
    //     ->debug_with_guard(get_typename() + "::insert(const tkey &, const tvalue &) : successfuly finished.");
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insert(
    const tkey &key,
    tvalue &&value)
{
    // this->trace_with_guard(get_typename() + "::insert(const tkey &, tvalue &&) : called.")
    //     ->debug_with_guard(get_typename() + "::insert(const tkey &, tvalue &&) : called.")
    //     ->debug_with_guard(get_typename() + "::insert(const tkey &, const tvalue &) : inserting node with key \"" + 
    //         std::to_string(key) + "\"");
    
    _insertion_template->insert(key, std::move(value));
    
    // this->debug_with_guard(get_typename() + "::insert(const tkey &, const tvalue &) : now root has key \"" + 
    //         std::to_string(reinterpret_cast<node*>(this->_root)->key) + "\"")
    //     ->trace_with_guard(get_typename() + "::insert(const tkey &, tvalue &&) : successfuly finished.")
    //     ->debug_with_guard(get_typename() + "::insert(const tkey &, tvalue &&) : successfuly finished.");
}

template<
    typename tkey,
    typename tvalue>
const tvalue &binary_search_tree<tkey, tvalue>::obtain(
    const tkey &key)
{
    this->trace_with_guard(get_typename() + "::obtain(const tkey &) : called.")
        ->debug_with_guard(get_typename() + "::obtain(const tkey &) : called.");
    
    const tvalue &value = _obtaining_template->obtain(key);
    
    this->trace_with_guard(get_typename() + "::obtain(const tkey &) : successfuly finished.")
        ->debug_with_guard(get_typename() + "::obtain(const tkey &) : successfuly finished.");
    
    return value;
}

template<
    typename tkey,
    typename tvalue>
std::vector<typename associative_container<tkey, tvalue>::key_value_pair> binary_search_tree<tkey, tvalue>::obtain_between(
    const tkey &lower_bound,
    const tkey &upper_bound,
    bool lower_bound_inclusive,
    bool upper_bound_inclusive)
{
    this->trace_with_guard(get_typename() + "::obtain_between(const tkey &, const tkey &, bool, bool) : called.")
        ->debug_with_guard(get_typename() + "::obtain_between(const tkey &, const tkey &, bool, bool) : called.");
    
    auto values = _obtaining_template->obtain_between(lower_bound, upper_bound, lower_bound_inclusive, upper_bound_inclusive);
    
    this->trace_with_guard(get_typename() + "::obtain_between(const tkey &, const tkey &, bool, bool) : successfuly finished.")
        ->debug_with_guard(get_typename() + "::obtain_between(const tkey &, const tkey &, bool, bool) : successfuly finished.");
    
    return values;
}

template<
    typename tkey,
    typename tvalue>
tvalue binary_search_tree<tkey, tvalue>::dispose(
    const tkey &key)
{
    // this->trace_with_guard(get_typename() + "::dispose(const tkey &) : called.")
    //     ->debug_with_guard(get_typename() + "::dispose(const tkey &) : called.")
    //     ->debug_with_guard(get_typename() + "::dispose(const tkey &) : disposing node with key \"" + 
    //         std::to_string(key) + "\"");
    
    tvalue value = _disposal_template->dispose(key);
    
    // this->debug_with_guard(get_typename() + "::insert(const tkey &, const tvalue &) : now root has key \"" + 
    //         std::to_string(reinterpret_cast<node*>(this->_root)->key) + "\"")
    //     ->trace_with_guard(get_typename() + "::dispose(const tkey &) : successfuly finished.")
    //     ->debug_with_guard(get_typename() + "::dispose(const tkey &) : successfuly finished.");
    
    return value;
}

#pragma endregion associative_containers contract implementations

#pragma region iterators requesting implementation

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::
nconst_iterator<typename binary_search_tree<tkey, tvalue>::prefix_traverse> 
    binary_search_tree<tkey, tvalue>::
    begin_prefix() const noexcept
{
    return nconst_iterator<prefix_traverse>(this, reinterpret_cast<node*>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::
nconst_iterator<typename binary_search_tree<tkey, tvalue>::prefix_traverse> 
    binary_search_tree<tkey, tvalue>::
    end_prefix() const noexcept
{
    return nconst_iterator<prefix_traverse>(this, nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::
const_iterator<typename binary_search_tree<tkey, tvalue>::prefix_traverse> 
    binary_search_tree<tkey, tvalue>::
    cbegin_prefix() const noexcept
{
    return const_iterator<prefix_traverse>(this, reinterpret_cast<node*>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::
const_iterator<typename binary_search_tree<tkey, tvalue>::prefix_traverse> 
    binary_search_tree<tkey, tvalue>::
    cend_prefix() const noexcept
{
    return const_iterator<prefix_traverse>(this, nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::
nconst_iterator<typename binary_search_tree<tkey, tvalue>::prefix_reverse_traverse> 
    binary_search_tree<tkey, tvalue>::
    rbegin_prefix() const noexcept
{
    return nconst_iterator<prefix_reverse_traverse>(this, reinterpret_cast<node*>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::
nconst_iterator<typename binary_search_tree<tkey, tvalue>::prefix_reverse_traverse> 
    binary_search_tree<tkey, tvalue>::
    rend_prefix() const noexcept
{
    return nconst_iterator<prefix_reverse_traverse>(this, nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::
const_iterator<typename binary_search_tree<tkey, tvalue>::prefix_reverse_traverse> 
    binary_search_tree<tkey, tvalue>::
    crbegin_prefix() const noexcept
{
    return const_iterator<prefix_traverse>(this, reinterpret_cast<node*>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::
const_iterator<typename binary_search_tree<tkey, tvalue>::prefix_reverse_traverse> 
    binary_search_tree<tkey, tvalue>::
    crend_prefix() const noexcept
{
    return const_iterator<prefix_traverse>(this, nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::
nconst_iterator<typename binary_search_tree<tkey, tvalue>::infix_traverse> 
    binary_search_tree<tkey, tvalue>::
    begin_infix() const noexcept
{
    return nconst_iterator<infix_traverse>(this, reinterpret_cast<node*>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::
nconst_iterator<typename binary_search_tree<tkey, tvalue>::infix_traverse> 
    binary_search_tree<tkey, tvalue>::
    end_infix() const noexcept
{
    return nconst_iterator<infix_traverse>(this, nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::
const_iterator<typename binary_search_tree<tkey, tvalue>::infix_traverse> 
    binary_search_tree<tkey, tvalue>::
    cbegin_infix() const noexcept
{
    return const_iterator<infix_traverse>(this, reinterpret_cast<node*>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::
const_iterator<typename binary_search_tree<tkey, tvalue>::infix_traverse> 
    binary_search_tree<tkey, tvalue>::
    cend_infix() const noexcept
{
    return const_iterator<infix_traverse>(this, nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::
nconst_iterator<typename binary_search_tree<tkey, tvalue>::infix_reverse_traverse> 
    binary_search_tree<tkey, tvalue>::
    rbegin_infix() const noexcept
{
    return nconst_iterator<infix_reverse_traverse>(this, reinterpret_cast<node*>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::
nconst_iterator<typename binary_search_tree<tkey, tvalue>::infix_reverse_traverse> 
    binary_search_tree<tkey, tvalue>::
    rend_infix() const noexcept
{
    return nconst_iterator<infix_reverse_traverse>(this, nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::
const_iterator<typename binary_search_tree<tkey, tvalue>::infix_reverse_traverse> 
    binary_search_tree<tkey, tvalue>::
    crbegin_infix() const noexcept
{
    return nconst_iterator<infix_reverse_traverse>(this, reinterpret_cast<node*>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::
const_iterator<typename binary_search_tree<tkey, tvalue>::infix_reverse_traverse> 
    binary_search_tree<tkey, tvalue>::
    crend_infix() const noexcept
{
    return const_iterator<infix_reverse_traverse>(this, nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::
nconst_iterator<typename binary_search_tree<tkey, tvalue>::postfix_traverse> 
    binary_search_tree<tkey, tvalue>::
    begin_postfix() const noexcept
{
    return nconst_iterator<postfix_traverse>(this, reinterpret_cast<node*>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::
nconst_iterator<typename binary_search_tree<tkey, tvalue>::postfix_traverse> 
    binary_search_tree<tkey, tvalue>::
    end_postfix() const noexcept
{
    return nconst_iterator<postfix_traverse>(this, nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::
const_iterator<typename binary_search_tree<tkey, tvalue>::postfix_traverse> 
    binary_search_tree<tkey, tvalue>::
    cbegin_postfix() const noexcept
{
    return const_iterator<postfix_traverse>(this, reinterpret_cast<node*>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::
const_iterator<typename binary_search_tree<tkey, tvalue>::postfix_traverse> 
    binary_search_tree<tkey, tvalue>::
    cend_postfix() const noexcept
{
    return const_iterator<postfix_traverse>(this, nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::
nconst_iterator<typename binary_search_tree<tkey, tvalue>::postfix_reverse_traverse> 
    binary_search_tree<tkey, tvalue>::
    rbegin_postfix() const noexcept
{
    return nconst_iterator<postfix_reverse_traverse>(this, reinterpret_cast<node*>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::
nconst_iterator<typename binary_search_tree<tkey, tvalue>::postfix_reverse_traverse> 
    binary_search_tree<tkey, tvalue>::
    rend_postfix() const noexcept
{
    return nconst_iterator<postfix_reverse_traverse>(this, nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::
const_iterator<typename binary_search_tree<tkey, tvalue>::postfix_reverse_traverse> 
    binary_search_tree<tkey, tvalue>::
    crbegin_postfix() const noexcept
{
    return const_iterator<postfix_reverse_traverse>(this, reinterpret_cast<node*>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::
const_iterator<typename binary_search_tree<tkey, tvalue>::postfix_reverse_traverse> 
    binary_search_tree<tkey, tvalue>::
    crend_postfix() const noexcept
{
    return const_iterator<postfix_reverse_traverse>(this, nullptr);
}

#pragma endregion iterators request implementation

#pragma region subtree rotations implementation

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::small_left_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool validate) const
{
    this->trace_with_guard(get_typename() + "::small_left_rotation() : called.")
        ->debug_with_guard(get_typename() + "::small_left_rotation() : called.")
        ->debug_with_guard(get_typename() + "::small_left_rotation() : rotating node with key \"" + 
            std::to_string(subtree_root->key) + "\"");
    
    if (validate && (subtree_root == nullptr || subtree_root->right_subtree == nullptr))
    {
        throw std::logic_error("Cannot perform small left rotation");
    }
    
    binary_search_tree<tkey, tvalue>::node *new_subtree_root = subtree_root->right_subtree;
    
    subtree_root->right_subtree = new_subtree_root->left_subtree;
    new_subtree_root->left_subtree = subtree_root;
    subtree_root = new_subtree_root;
    
    update_node_data(new_subtree_root->left_subtree);
    update_node_data(new_subtree_root);
    
    this->trace_with_guard(get_typename() + "::small_left_rotation() : successfuly finished.")
        ->debug_with_guard(get_typename() + "::small_left_rotation() : successfuly finished.");
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::small_right_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool validate) const
{
    this->trace_with_guard(get_typename() + "::small_right_rotation() : called.")
        ->debug_with_guard(get_typename() + "::small_right_rotation() : called.")
        ->debug_with_guard(get_typename() + "::small_right_rotation() : rotating node with key \"" + 
            std::to_string(subtree_root->key) + "\"");
    
    if (validate && (subtree_root == nullptr || subtree_root->left_subtree == nullptr))
    {
        throw std::logic_error("Cannot perform small right rotation");
    }
    
    binary_search_tree<tkey, tvalue>::node *new_subtree_root = subtree_root->left_subtree;
    
    subtree_root->left_subtree = new_subtree_root->right_subtree;
    new_subtree_root->right_subtree = subtree_root;
    subtree_root = new_subtree_root;
    
    update_node_data(new_subtree_root->right_subtree);
    update_node_data(new_subtree_root);
    
    this->trace_with_guard(get_typename() + "::small_right_rotation() : successfuly finished.")
        ->debug_with_guard(get_typename() + "::small_right_rotation() : successfuly finished.");
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::big_left_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool validate) const
{
    this->trace_with_guard(get_typename() + "::big_left_rotation() : called.")
        ->debug_with_guard(get_typename() + "::big_left_rotation() : called.");
    
    if (validate && (subtree_root == nullptr ||
            subtree_root->right_subtree == nullptr || subtree_root->right_subtree->left_subtree == nullptr))
    {
        throw std::logic_error("Cannot perform big left rotation");
    }
    
    small_right_rotation(subtree_root->right_subtree, false);
    small_left_rotation(subtree_root, false);
    
    this->trace_with_guard(get_typename() + "::big_left_rotation() : successfuly finished.")
        ->debug_with_guard(get_typename() + "::big_left_rotation() : successfuly finished.");
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::big_right_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool validate) const
{
    this->trace_with_guard(get_typename() + "::big_right_rotation() : called.")
        ->debug_with_guard(get_typename() + "::big_right_rotation() : called.");
    
    if (validate && (subtree_root == nullptr ||
            subtree_root->left_subtree == nullptr || subtree_root->left_subtree->right_subtree == nullptr))
    {
        throw std::logic_error("Cannot perform big right rotation");
    }
    
    small_left_rotation(subtree_root->left_subtree, false);
    small_right_rotation(subtree_root, false);
    
    this->trace_with_guard(get_typename() + "::big_right_rotation() : successfuly finished.")
        ->debug_with_guard(get_typename() + "::big_right_rotation() : successfuly finished.");
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::double_left_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool at_grandparent_first,
    bool validate) const
{
    this->trace_with_guard(get_typename() + "::double_left_rotation() : called.")
        ->debug_with_guard(get_typename() + "::double_left_rotation() : called.");
    
    if (validate && (subtree_root == nullptr ||
            subtree_root->right_subtree == nullptr || subtree_root->right_subtree->right_subtree == nullptr))
    {
        throw std::logic_error("Cannot perform double right rotation");
    }
    
    if (at_grandparent_first)
    {
        small_left_rotation(subtree_root, false);
        small_left_rotation(subtree_root, false);
    }
    else
    {
        small_left_rotation(subtree_root->right, false);
        small_left_rotation(subtree_root, false);
    }
    
    this->trace_with_guard(get_typename() + "::double_left_rotation() : successfuly finished.")
        ->debug_with_guard(get_typename() + "::double_left_rotation() : successfuly finished.");
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::double_right_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool at_grandparent_first,
    bool validate) const
{
    this->trace_with_guard(get_typename() + "::double_right_rotation() : called.")
        ->debug_with_guard(get_typename() + "::double_right_rotation() : called.");
    
    if (validate && (subtree_root == nullptr ||
            subtree_root->left_subtree == nullptr || subtree_root->left_subtree->left_subtree == nullptr))
    {
        throw std::logic_error("Cannot perform double left rotation");
    }
    
    if (at_grandparent_first)
    {
        small_left_rotation(subtree_root, false);
        small_left_rotation(subtree_root, false);
    }
    else
    {
        small_left_rotation(subtree_root->left, false);
        small_left_rotation(subtree_root, false);
    }
    
    this->trace_with_guard(get_typename() + "::double_right_rotation() : successfuly finished.")
        ->debug_with_guard(get_typename() + "::double_right_rotation() : successfuly finished.");
}

#pragma endregion subtree rotations implementation

template<
    typename tkey,
    typename tvalue>
std::string binary_search_tree<tkey, tvalue>::
get_typename() const noexcept {
    return "binary_search_tree<tkey, tvalue>";
}

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H