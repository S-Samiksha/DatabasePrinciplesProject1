
const int MAX_SIZE = 3;

class Node {

    friend class BPTree;

    private:
        bool isLeaf;
        // n
        int size {0};
        vector <int> keys = vector <int> (MAX_SIZE);
        vector <Node*> childrenNodes = vector <Node*> (MAX_SIZE);
        
    public:
        Node ();
};
