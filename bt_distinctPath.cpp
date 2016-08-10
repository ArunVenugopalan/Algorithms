// Find length of the longest distinct path in a binary tree
// Time complexity O(N) and space complexity O(log N)

#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>

struct tree {
    int x;
    tree * l;
    tree * r;
    tree():x(-1), l(nullptr), r(nullptr) { }
};


// Returns length of the longest unique/distinct path from root in tree T
int solution(tree* T) {
    using namespace std;

    if (T == nullptr) {
        return 0;
    }
    
    struct nodetracker_t {
        nodetracker_t() :node(nullptr), parent(nullptr) {}
        nodetracker_t(tree* _node, tree* _parent) :
            node(_node), parent(_parent) { }
        tree* node;
        tree* parent;
    };

    vector<tree*> uniquePath;
    unsigned int uniquePathMaxLen = 0;
    stack<nodetracker_t> nodetracker; 

    nodetracker.push(nodetracker_t(T, T));

    // check if we found a better distinct path than earlier
    auto update_max_path_len = [&](unsigned int len) {
        // could be replaced using compare-and-swap idiom 
        if (uniquePathMaxLen < len) {
            uniquePathMaxLen = len;
        }
    };

    // uniquePath keeps track of unique path from root to current node
    // Time complexity: Worst-case complexity of O(N). Amortized cost of O(log N)
    // Space complexity: O(log N)
    auto add_to_tracker = [&](nodetracker_t element) {
        if (element.parent) {
            auto it = std::find(uniquePath.begin(), uniquePath.end(), element.parent);
            if (it != uniquePath.end()) {
                auto top = uniquePath.end() -  1;
                if ( *top != element.parent) {
                    auto index = std::distance(uniquePath.begin(), it);
                    uniquePath.erase(uniquePath.begin() + index + 1, uniquePath.end());
                }
            }
        }

        if (element.node) {
            uniquePath.push_back(element.node);
        }

#if _DEBUG_
        cout << "uniquePath => { " << endl;
        for (auto element : uniquePath) {
            cout << element->x << " ";
        }
        cout << "}" << endl;
#endif
    };

    // Stack child if and only if different to parent, improves performance
    auto handle_child = [&](tree* child, tree* parent) {
        if (child->x != parent->x) {
            nodetracker.push(nodetracker_t(child, parent));
        }
        else {
            update_max_path_len(uniquePath.size());
        }
    };


    // Typical DFS but using modified stack structure
    while (!nodetracker.empty()) {
        nodetracker_t element = nodetracker.top();
        nodetracker.pop();
        
        if (std::find(uniquePath.begin(), uniquePath.end(), element.node) == uniquePath.end()) {
            add_to_tracker(element);
        }

        if ((element.node->l == nullptr) && (element.node->r == nullptr)) {
            update_max_path_len(uniquePath.size());
        }
        else {
            if (element.node->l) {
                handle_child(element.node->l, element.node);
            }

            if (element.node->r) {
                handle_child(element.node->r, element.node);
            }
        }        
    }
    
    return uniquePathMaxLen;
}

//////////////////////////////////////////////////////
// Rudimentary helpers to construct and visualize tree
//////////////////////////////////////////////////////
void printout(tree *node)
{
    if (node->l)
        printout(node->l);

    std::cout<<node->x<<" ";

    if (node->r)
        printout(node->r);
}
void insertLeft(tree *root, tree *child)
{
    tree *tmp = root;
    while (tmp->l)
    {
        tmp = tmp->l;
    }
    tmp->l = child;
}

void insertRight(tree *root, tree *child)
{
    tree *tmp = root;
    while (tmp->r)
    {
        tmp = tmp->r;
    }
    tmp->r = child;
}

int main()
{
    //Test case - 1
    using namespace std;

    tree *curr = nullptr, *root = nullptr;
    root = new tree;
    root->l = root->r = nullptr;
    root->x = 1;

    int leftArray[] = { 22, 33, 44, 54, 66, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
    int rightArray[] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
    int midArray[] = { 0, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };

    for (auto element : leftArray) {
        curr = new tree;
        curr->l = curr->r = nullptr;
        curr->x = element;

        insertLeft(root, curr);
    }

    for (auto element : rightArray) {
        curr = new tree;
        curr->l = curr->r = nullptr;
        curr->x = element;

        insertRight(root, curr);
    }

    for (auto element : midArray) {
        curr = new tree;
        curr->l = curr->r = nullptr;
        curr->x = element;

        insertRight(root->l, curr);
    }

    cout << "Tree In-order traversal => { " << endl;
    printout(root);
    cout << endl << " }" << endl;


    cout << "Max distinct path length is " << solution(root) << endl;
   return 0;
}
