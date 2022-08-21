#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <tuple>
#include <vector>

/**
 * An abstract template base of the KDTree class
 */
template <typename...>
class KDTree;

/**
 * A partial template specialization of the KDTree class
 * The time complexity of functions are based on n and k
 * n is the size of the KDTree
 * k is the number of dimensions
 * @typedef Key         key type
 * @typedef Value       value type
 * @typedef Data        key-value pair
 * @static  KeySize     k (number of dimensions)
 */
template <typename ValueType, typename... KeyTypes>
class KDTree<std::tuple<KeyTypes...>, ValueType> {
public:
    typedef std::tuple<KeyTypes...> Key;
    typedef ValueType Value;
    typedef std::pair<const Key, Value> Data;
    static inline constexpr size_t KeySize = std::tuple_size<Key>::value;
    static_assert(KeySize > 0, "Can not construct KDTree with zero dimension");

protected:
    struct Node {
        Data data;
        Node *parent;
        Node *left = nullptr;
        Node *right = nullptr;

        Node(const Key &key, const Value &value, Node *parent) : data(key, value), parent(parent) {}

        const Key &key() { return data.first; }

        Value &value() { return data.second; }
    };

public:
    /**
     * A bi-directional iterator for the KDTree
     * ! ONLY NEED TO MODIFY increment and decrement !
     */
    class Iterator {
    private:
        KDTree *tree;
        Node *node;

        Iterator(KDTree *tree, Node *node) : tree(tree), node(node) {}

        /**
         * Increment the iterator
         * Time complexity: O(log n)
         */
        void increment() {
            if (*this == this->tree->end()) throw std::range_error("Out of range!");
            if (this->node->right) {
                this->node = this->node->right;
                while (this->node->left) {
                    this->node = this->node->left;
                }
            } else {
                while (this->node != this->tree->root && this->node != this->node->parent->left) {
                    this->node = this->node->parent;
                }
                if (this->node == this->tree->root)
                    this->node = nullptr;
                else
                    this->node = this->node->parent;
            }
        }

        /**
         * Decrement the iterator
         * Time complexity: O(log n)
         */
        void decrement() {
            if (this == this->tree->begin()) throw std::range_error("Out of range!");
            if (this == this->tree->end()) {
                this->node = this->tree->root;
                while (this->node->right) this->node = this->node->right;
            }
            if (this->node->left) {
                this->node = this->node->left;
                while (this->node->right) {
                    this->node = this->node->right;
                }
            } else {
                while (this->node != this->tree->root && this->node != this->node->parent->right) {
                    this->node = this->node->parent;
                }
                if (this->node == this->tree->root)
                    this->node = nullptr;
                else
                    this->node = this->node->parent;
            }
        }

    public:
        friend class KDTree;

        Iterator() = delete;

        Iterator(const Iterator &) = default;

        Iterator &operator=(const Iterator &) = default;

        Iterator &operator++() {
            increment();
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            increment();
            return temp;
        }

        Iterator &operator--() {
            decrement();
            return *this;
        }

        Iterator operator--(int) {
            Iterator temp = *this;
            decrement();
            return temp;
        }

        bool operator==(const Iterator &that) const { return node == that.node; }

        bool operator!=(const Iterator &that) const { return node != that.node; }

        Data *operator->() { return &(node->data); }

        Data &operator*() { return node->data; }
    };

protected:              // DO NOT USE private HERE!
    Node *root = nullptr;  // root of the tree
    size_t treeSize = 0;   // size of the tree

    /**
     * Find the node with key
     * Time Complexity: O(k log n)
     * @tparam DIM current dimension of node
     * @param key
     * @param node
     * @return the node with key, or nullptr if not found
     */
    template <size_t DIM>
    Node *find(const Key &key, Node *node) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (!node) return nullptr;
        if (node->key() == key) return node;
        if (std::get<DIM>(key) < std::get<DIM>(node->key())) {
            return find<DIM_NEXT>(key, node->left);
        } else {
            return find<DIM_NEXT>(key, node->right);
        }
    }

    /**
     * Insert the key-value pair, if the key already exists, replace the value only
     * Time Complexity: O(k log n)
     * @tparam DIM current dimension of node
     * @param key
     * @param value
     * @param node
     * @param parent
     * @return whether insertion took place (return false if the key already
     * exists)
     */
    template <size_t DIM>
    bool insert(const Key &key, const Value &value, Node *&node, Node *parent) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (!node) {
            node = new Node(key, value, parent);
            ++treeSize;
            return true;
        }
        if (node->key() == key) {
            node->value() = value;
            return false;
        }
        if (std::get<DIM>(key) < std::get<DIM>(node->key())) {
            return insert<DIM_NEXT>(key, value, node->left, node);
        } else {
            return insert<DIM_NEXT>(key, value, node->right, node);
        }
    }

    /**
     * Compare two keys on a dimension
     * Time Complexity: O(1)
     * @tparam DIM comparison dimension
     * @tparam Compare
     * @param a
     * @param b
     * @param compare
     * @return relationship of two keys on a dimension with the compare function
     */
    template <size_t DIM, typename Compare>
    static bool compareKey(const Key &a, const Key &b, Compare compare = Compare()) {
        if (std::get<DIM>(a) != std::get<DIM>(b)) {
            return compare(std::get<DIM>(a), std::get<DIM>(b));
        }
        return compare(a, b);
    }

    /**
     * Compare two nodes on a dimension
     * Time Complexity: O(1)
     * @tparam DIM comparison dimension
     * @tparam Compare
     * @param a
     * @param b
     * @param compare
     * @return the minimum / maximum of two nodes
     */
    template <size_t DIM, typename Compare>
    static Node *compareNode(Node *a, Node *b, Compare compare = Compare()) {
        if (!a) return b;
        if (!b) return a;
        return compareKey<DIM, Compare>(a->key(), b->key(), compare) ? a : b;
    }

    /**
     * Find the minimum node on a dimension
     * Time Complexity: ?
     * @tparam DIM_CMP comparison dimension
     * @tparam DIM current dimension of node
     * @param node
     * @return the minimum node on a dimension
     */
    template <size_t DIM_CMP, size_t DIM>
    Node *findMin(Node *node) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (!node) {
            return nullptr;
        }
        Node *min = findMin<DIM_CMP, DIM_NEXT>(node->left);
        if (DIM != DIM_CMP) {
            Node *rightMin = findMin<DIM_CMP, DIM_NEXT>(node->right);
            min = compareNode<DIM_CMP, std::less<>>(min, rightMin);
        }
        return compareNode<DIM_CMP, std::less<>>(min, node);
    }

    /**
     * Find the maximum node on a dimension
     * Time Complexity: ?
     * @tparam DIM_CMP comparison dimension
     * @tparam DIM current dimension of node
     * @param node
     * @return the maximum node on a dimension
     */
    template <size_t DIM_CMP, size_t DIM>
    Node *findMax(Node *node) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (!node) {
            return nullptr;
        }
        Node *max = findMax<DIM_CMP, DIM_NEXT>(node->right);
        if (DIM != DIM_CMP) {
            Node *leftMax = findMax<DIM_CMP, DIM_NEXT>(node->left);
            max = compareNode<DIM_CMP, std::greater<>>(max, leftMax);
        }
        return compareNode<DIM_CMP, std::greater<>>(max, node);
    }

    template <size_t DIM>
    Node *findMinDynamic(size_t dim) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (dim >= KeySize) {
            dim %= KeySize;
        }
        if (dim == DIM) return findMin<DIM, 0>(root);
        return findMinDynamic<DIM_NEXT>(dim);
    }

    template <size_t DIM>
    Node *findMaxDynamic(size_t dim) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (dim >= KeySize) {
            dim %= KeySize;
        }
        if (dim == DIM) return findMax<DIM, 0>(root);
        return findMaxDynamic<DIM_NEXT>(dim);
    }

    /**
     * Erase a node with key (check the pseudocode in project description)
     * Time Complexity: max{O(k log n), O(findMin)}
     * @tparam DIM current dimension of node
     * @param node
     * @param key
     * @return nullptr if node is erased, else the (probably) replaced node
     */
    template <size_t DIM>
    Node *erase(Node *node, const Key &key) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (!node) {
            return nullptr;
        }
        if (node->key() == key) {
            if (!node->left && !node->right) {
                if (node->parent) {
                    if (node == node->parent->left) {
                        node->parent->left = nullptr;
                    } else {
                        node->parent->right = nullptr;
                    }
                } else {
                    root = nullptr;
                }
                delete node;
                --treeSize;
                return nullptr;
            } else if (node->right) {
                Node *minNode = findMin<DIM, DIM_NEXT>(node->right);
                Node *temp = node;
                node = new Node(minNode->key(), minNode->value(), temp->parent);
                node->right = temp->right;
                node->right->parent = node;
                if (temp->left) {
                    node->left = temp->left;
                    node->left->parent = node;
                }
                if (temp == root) {
                    root = node;
                }
                delete temp;
                node->right = erase<DIM_NEXT>(node->right, node->key());
            } else if (node->left) {
                Node *maxNode = findMax<DIM, DIM_NEXT>(node->left);
                Node *temp = node;
                node = new Node(maxNode->key(), maxNode->value(), temp->parent);
                node->left = temp->left;
                node->left->parent = node;
                if (temp == root) root = node;
                delete temp;
                node->left = erase<DIM_NEXT>(node->left, node->key());
            }
        } else {
            if (std::get<DIM>(key) < std::get<DIM>(node->key())) {
                node->left = erase<DIM_NEXT>(node->left, key);
            } else {
                node->right = erase<DIM_NEXT>(node->right, key);
            }
        }
        return node;
    }

    template <size_t DIM>
    Node *eraseDynamic(Node *node, size_t dim) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (dim >= KeySize) {
            dim %= KeySize;
        }
        if (dim == DIM) {
            return erase<DIM>(node, node->key());
        }
        return eraseDynamic<DIM_NEXT>(node, dim);
    }

    // TODO: define your helper functions here if necessary

    Node *copyFrom(Node *&node, Node *copyNode, Node *parent) {
        if (!copyNode) return nullptr;
        node = new Node(copyNode->key(), copyNode->value(), parent);
        node->left = copyFrom(node->left, copyNode->left, node);
        node->right = copyFrom(node->right, copyNode->right, node);
        return node;
    }

    static bool uniqueComp(const Data &a, const Data &b) { return a.first == b.first; }

    template <size_t DIM>
    static bool dataComp(const Data &a, const Data &b) {
        if (std::get<DIM>(a.first) != std::get<DIM>(b.first)) {
            return std::get<DIM>(a.first) < std::get<DIM>(b.first);
        }
        return a.first < b.first;
    }

    template <size_t DIM>
    Node *vectorConstructor(std::vector<std::pair<Key, Value>> &v, int left, int right, Node *parent) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (left > right) return nullptr;
        int mid = (left + right) / 2;
        std::nth_element(v.begin() + left, v.begin() + mid, v.begin() + right, dataComp<DIM>);
        std::pair<Key, Value> midNode = v[mid];
        Node *newNode = new Node(midNode.first, midNode.second, parent);
        newNode->left = vectorConstructor<DIM_NEXT>(v, left, mid - 1, newNode);
        newNode->right = vectorConstructor<DIM_NEXT>(v, mid + 1, right, newNode);
        return newNode;
    }

    void range_search_helper (Node* root, int dim,int (&position)[2], int (&Searchrange)[2][2], int TreeRange[2][2], std::vector<Value> &result){
        if (!root) return;
        if (TreeRange[0][0] >Searchrange[0][1] || TreeRange[0][1] <Searchrange[0][0] ||TreeRange[1][0] >Searchrange[1][1] ||TreeRange[1][1] < Searchrange[1][0] ) {
            return;
        }

        int x = std::get<0>(root->key());
        int y = std::get<1>(root->key());
        int distance_square = (x-position[0])*(x-position[0])+(y-position[1])*(y-position[1]);
        if (x != position[0] || y != position[1]){
            if (distance_square <= (Searchrange[0][1]-position[0])*(Searchrange[0][1]-position[0])) result.push_back(root->value());
        }

        if (dim==0) {
            int leftTreeRange[2][2] = {
                    {TreeRange[0][0],x},
                    {TreeRange[1][0],TreeRange[1][1]}
            };
            range_search_helper(root->left,(dim+1)%2,position, Searchrange,leftTreeRange,result);

            int rightTreeRange[2][2] = {
                    {x,TreeRange[0][1]},
                    {TreeRange[1][0],TreeRange[1][1]}
            };
            range_search_helper(root->right,(dim+1)%2,position, Searchrange,rightTreeRange,result);
        }

        else if (dim==1) {
            int leftTreeRange[2][2] = {
                    {TreeRange[0][0],TreeRange[0][1]},
                    {TreeRange[1][0],y}
            };
            range_search_helper(root->left,(dim+1)%2,position, Searchrange,leftTreeRange,result);

            int rightTreeRange[2][2] = {
                    {TreeRange[0][0],TreeRange[0][1]},
                    {y,TreeRange[1][1]}
            };
            range_search_helper(root->right,(dim+1)%2,position, Searchrange,rightTreeRange,result);
        }
    }

public:
    KDTree() = default;

    /**
     * Time complexity: O(kn log n)
     * @param v we pass by value here because v need to be modified
     */
    explicit KDTree(std::vector<std::pair<Key, Value>> v) {
        root = nullptr;
        treeSize = 0;
        std::stable_sort(v.begin(), v.end(), dataComp<0>);
        auto ip = std::unique(v.rbegin(), v.rend(), uniqueComp);
        v.assign(ip.base(), v.end());
        root = vectorConstructor<0>(v, 0, static_cast<int>(v.size()) - 1, nullptr);
        treeSize = v.size();
    }

    /**
     * Time complexity: O(n)
     */
    KDTree(const KDTree &that) {
        root = nullptr;
        treeSize = that.treeSize;
        root = copyFrom(root, that.root, nullptr);
    }

    /**
     * Time complexity: O(n)
     */
    KDTree &operator=(const KDTree &that) {
        if (this != &that) {
            for (auto it = this->begin(); it != this->end(); ++it) {
                delete it.node;
            }
            root = copyFrom(root, that.root, nullptr);
            treeSize = that.treeSize;
        }
        return *this;
    }

    /**
     * Time complexity: O(n)
     */
    ~KDTree() {
        for (auto it = this->begin(); it != this->end(); ++it) {
            delete it.node;
        }
        treeSize = 0;
    }

    Iterator begin() {
        if (!root) return end();
        auto node = root;
        while (node->left) node = node->left;
        return Iterator(this, node);
    }

    Iterator end() { return Iterator(this, nullptr); }

    Iterator find(const Key &key) { return Iterator(this, find<0>(key, root)); }

    void insert(const Key &key, const Value &value) { insert<0>(key, value, root, nullptr); }

    template <size_t DIM>
    Iterator findMin() {
        return Iterator(this, findMin<DIM, 0>(root));
    }

    Iterator findMin(size_t dim) { return Iterator(this, findMinDynamic<0>(dim)); }

    template <size_t DIM>
    Iterator findMax() {
        return Iterator(this, findMax<DIM, 0>(root));
    }

    Iterator findMax(size_t dim) { return Iterator(this, findMaxDynamic<0>(dim)); }

    bool erase(const Key &key) {
        auto prevSize = treeSize;
        erase<0>(root, key);
        return prevSize > treeSize;
    }

    Iterator erase(Iterator it) {
        if (it == end()) return it;
        auto node = it.node;
        if (!it.node->left && !it.node->right) {
            it.node = it.node->parent;
        }
        size_t depth = 0;
        auto temp = node->parent;
        while (temp) {
            temp = temp->parent;
            ++depth;
        }
        eraseDynamic<0>(node, depth % KeySize);
        return it;
    }

    size_t size() const { return treeSize; }

    /**
   * find the nodes within the distance around a certain point
   * @param key: the location of the point
   * @param distance: the distance
   * @return a vector contains the value of nodes that satisfy the requirement
   */
    std::vector<Value> *Range_Search(Key key, int distance) {
        int x = std::get<0>(key);
        int y = std::get<1>(key);
        auto result = new std::vector<Value>;
        int position[2] = {x,y};
        int searchRange[2][2] = {
                {x-distance,x+distance},
                {y-distance,y+distance}
        };
        int treeRange[2][2] = {
                {0,500},
                {0,500}
        };
        range_search_helper(root,0,position,searchRange,treeRange,*result);
        return result;
    }

};

