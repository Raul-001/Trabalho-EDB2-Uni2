#include <iostream>
#include <memory>

enum class Color
{
    RED,
    BLACK
};

template <typename T>
struct Node
{
    T data;
    Color color;
    // Evita a necessidade de lidar com alocação manual
    std::shared_ptr<Node<T>> left, right, parent;

    Node(T value) : data(value), color(Color::RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

template <typename T>
class RedBlackTree
{
private:
    // Root será inicializada no Construtor como nullptr
    // A expansão do template se dá em tempo de execução
    // O que previne a necessidade de inicializar o Ctro.
    std::shared_ptr<Node<T>> root;

    // Nome autosugestivo
    void leftRotate(std::shared_ptr<Node<T>> x)
    {
        auto y = x->right;
        x->right = y->left;
        if (y->left != nullptr)
        {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr)
        {
            root = y;
        }
        else if (x == x->parent->left)
        {
            x->parent->left = y;
        }
        else
        {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    // Nome autosugestivo/2
    void rightRotate(std::shared_ptr<Node<T>> x)
    {
        auto y = x->left;
        x->left = y->right;
        if (y->right != nullptr)
        {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr)
        {
            root = y;
        }
        else if (x == x->parent->right)
        {
            x->parent->right = y;
        }
        else
        {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
    }

    // fixInsert de inserção balanceada, e não inserção fixa.
    // realiza o balanceamento da árvore após inserção
    void fixInsert(std::shared_ptr<Node<T>> k)
    {
        while (k->parent && k->parent->color == Color::RED)
        {
            if (k->parent == k->parent->parent->left)
            {
                auto uncle = k->parent->parent->right;
                if (uncle && uncle->color == Color::RED)
                {
                    k->parent->color = Color::BLACK;
                    uncle->color = Color::BLACK;
                    k->parent->parent->color = Color::RED;
                    k = k->parent->parent;
                }
                else
                {
                    if (k == k->parent->right)
                    {
                        k = k->parent;
                        leftRotate(k);
                    }
                    k->parent->color = Color::BLACK;
                    k->parent->parent->color = Color::RED;
                    rightRotate(k->parent->parent);
                }
            }
            else
            {
                auto uncle = k->parent->parent->left;
                if (uncle && uncle->color == Color::RED)
                {
                    k->parent->color = Color::BLACK;
                    uncle->color = Color::BLACK;
                    k->parent->parent->color = Color::RED;
                    k = k->parent->parent;
                }
                else
                {
                    if (k == k->parent->left)
                    {
                        k = k->parent;
                        rightRotate(k);
                    }
                    k->parent->color = Color::BLACK;
                    k->parent->parent->color = Color::RED;
                    leftRotate(k->parent->parent);
                }
            }
        }
        root->color = Color::BLACK;
    }

    // metodo autoevidente
    // insere nodo
    void insertNode(std::shared_ptr<Node<T>> node)
    {
        std::shared_ptr<Node<T>> y = nullptr;
        std::shared_ptr<Node<T>> x = root;

        while (x != nullptr)
        {
            y = x;
            if (node->data < x->data)
            {
                x = x->left;
            }
            else
            {
                x = x->right;
            }
        }

        node->parent = y;
        if (y == nullptr)
        {
            root = node;
        }
        else if (node->data < y->data)
        {
            y->left = node;
        }
        else
        {
            y->right = node;
        }

        fixInsert(node);
    }

    // percorre a árvore indo de nó em nó na esquerda até
    // achar o nulo
    std::shared_ptr<Node<T>> minimum(std::shared_ptr<Node<T>> node)
    {
        while (node->left != nullptr)
        {
            node = node->left;
        }
        return node;
    }

    // Remoção balanceada
    // notação análoga à fixInsert
    void fixRemove(std::shared_ptr<Node<T>> x)
    {
        while (x != root && x->color == Color::BLACK)
        {
            if (x == x->parent->left)
            {
                auto w = x->parent->right;
                if (w->color == Color::RED)
                {
                    w->color = Color::BLACK;
                    x->parent->color = Color::RED;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == Color::BLACK && w->right->color == Color::BLACK)
                {
                    w->color = Color::RED;
                    x = x->parent;
                }
                else
                {
                    if (w->right->color == Color::BLACK)
                    {
                        w->left->color = Color::BLACK;
                        w->color = Color::RED;
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = Color::BLACK;
                    w->right->color = Color::BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            }
            else
            {
                auto w = x->parent->left;
                if (w->color == Color::RED)
                {
                    w->color = Color::BLACK;
                    x->parent->color = Color::RED;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }
                if (w->right->color == Color::BLACK && w->left->color == Color::BLACK)
                {
                    w->color = Color::RED;
                    x = x->parent;
                }
                else
                {
                    if (w->left->color == Color::BLACK)
                    {
                        w->right->color = Color::BLACK;
                        w->color = Color::RED;
                        leftRotate(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = Color::BLACK;
                    w->left->color = Color::BLACK;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = Color::BLACK;
    }

    // u é o nó que será substituido
    // v é o que substituirá
    // é basicamente um swap
    void changeNodes(std::shared_ptr<Node<T>> u, std::shared_ptr<Node<T>> v)
    {
        if (u->parent == nullptr)
        {
            root = v;
        }
        else if (u == u->parent->left)
        {
            u->parent->left = v;
        }
        else
        {
            u->parent->right = v;
        }
        if (v != nullptr)
        {
            v->parent = u->parent;
        }
    }

    // remove o Nó
    void removeNode(std::shared_ptr<Node<T>> z)
    {
        std::shared_ptr<Node<T>> y = z;
        std::shared_ptr<Node<T>> x;
        Color originalColor = y->color;

        if (z->left == nullptr)
        {
            x = z->right;
            changeNodes(z, z->right);
        }
        else if (z->right == nullptr)
        {
            x = z->left;
            changeNodes(z, z->left);
        }
        else
        {
            y = minimum(z->right);
            originalColor = y->color;
            x = y->right;
            if (y->parent == z)
            {
                if (x != nullptr)
                {
                    x->parent = y;
                }
            }
            else
            {
                changeNodes(y, y->right);
                y->right = z->right;
                if (y->right != nullptr)
                {
                    y->right->parent = y;
                }
            }
            changeNodes(z, y);
            y->left = z->left;
            if (y->left != nullptr)
            {
                y->left->parent = y;
            }
            y->color = z->color;
        }
        if (originalColor == Color::BLACK)
        {
            fixRemove(x);
        }
    }

    // Para o valor dado, realiza uma busca recursiva
    // até encontra-lo
    std::shared_ptr<Node<T>> searchNode(std::shared_ptr<Node<T>> root, T target)
    {
        if (root == nullptr || root->data == target)
        {
            return root;
        }
        if (target < root->data)
        {
            return searchNode(root->left, target);
        }
        return searchNode(root->right, target);
    }

public:
    // Interfaces ao programador
    // Ctro.
    RedBlackTree() : root(nullptr) {}

    void insert(T value)
    {
        // Boa sorte em descobrir a tipagem sem auto
        auto node = std::make_shared<Node<T>>(value);
        insertNode(node);
    }

    // Interface que remove o nó para o programador
    void remove(T value)
    {
        auto node = searchNode(root, value);
        if (node != nullptr)
        {
            removeNode(node);
        }
    }

    // Para o valor dado, retorna um bool
    bool search(T value)
    {
        auto node = searchNode(root, value);
        return node != nullptr;
    }

    void printTree(std::shared_ptr<Node<T>> root, std::string indent = "", bool last = true)
    {
        if (root != nullptr)
        {
            // Direciona a string indent ao stream de saída
            std::cout << indent;
            if (last)
            {
                std::cout << "R----";
                indent += "   ";
            }
            else
            {
                std::cout << "L----";
                indent += "|  ";
            }

            std::string color = (root->color == Color::RED) ? "RED" : "BLACK";
            std::cout << root->data << "(" << color << ")" << std::endl;
            printTree(root->left, indent, false);
            printTree(root->right, indent, true);
        }
    }

    // Oferece uma interface ao programador
    void showTree()
    {
        printTree(root);
    }
};

int main()
{
    RedBlackTree<int> rbt;

    // valores do item 4.3
    rbt.insert(15);
    rbt.insert(18);
    rbt.insert(20);
    rbt.insert(35);
    rbt.insert(32);
    rbt.insert(38);
    rbt.insert(30);
    rbt.insert(40);
    rbt.insert(32);
    rbt.insert(45);
    rbt.insert(48);
    rbt.insert(52);
    rbt.insert(60);
    rbt.insert(50);

    std::cout << "Árvore Rubro-Negra após inserções: \n";
    rbt.showTree();

    // rbt.remove(15); // ta dando segFault

    return 0;
}
