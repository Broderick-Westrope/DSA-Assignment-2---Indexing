//	==================== MACROS ====================
#define LH +1     // Left High 
#define EH  0     // Even High 
#define RH -1     // Right High 

// 	NODE Definitions
template<class TYPE>
struct NODE
{
    TYPE data;
    NODE *left;
    NODE *right;
    int bal;
}; // NODE

// Class Declaration
template<class TYPE, class KTYPE>
class AvlTree
{
private:
    int count;
    NODE<TYPE> *tree;

    NODE<TYPE> *_insert(NODE<TYPE> *root,
                        NODE<TYPE> *newPtr,
                        bool &taller);

    NODE<TYPE> *leftBalance(NODE<TYPE> *root,
                            bool &taller);

    NODE<TYPE> *rotateLeft(NODE<TYPE> *root);

    NODE<TYPE> *rightBalance(NODE<TYPE> *root,
                             bool &taller);

    NODE<TYPE> *rotateRight(NODE<TYPE> *root);

    NODE<TYPE> *_delete(NODE<TYPE> *root,
                        KTYPE dltKey,
                        bool &shorter,
                        bool &success);

    NODE<TYPE> *dltLeftBalance(NODE<TYPE> *root,
                               bool &smaller);

    NODE<TYPE> *dltRightBalance(NODE<TYPE> *root,
                                bool &shorter);

    NODE<TYPE> *_retrieve(KTYPE key,
                          NODE<TYPE> *root);

    void _traversalInOrder(void (*process)(TYPE *dataProc),
                           NODE<TYPE> *root);

    void _destroyAVL(NODE<TYPE> *root);

//  	The following function is used for debugging.
    void _print(NODE<TYPE> *root, int level);

    void _closestNode(NODE<TYPE> *ptr, TYPE _node, int &minDiff, vector<pair<TYPE, int>> &minDiffNodes, int (*comparison)(TYPE, TYPE));

    void _traversalBreadth(void (*process)(TYPE), NODE<TYPE> *root);

public:
    AvlTree(void);

    ~AvlTree(void);

    bool AVL_Insert(TYPE dataIn);

    bool AVL_Delete(KTYPE dltKey);

    bool AVL_Retrieve(KTYPE key, TYPE &dataOut);

    void AVL_TraverseInOrder(void (*process)(TYPE *dataProc)); //in-order

    bool AVL_Update(KTYPE key, TYPE newData);

    bool AVL_Empty(void);

    bool AVL_Full(void);

    int AVL_Count(void);

//      The following function is used for debugging.
    void AVL_Print(void);

    vector<pair<TYPE, int>> AVL_GetClosestNodes(TYPE _node, int (*comparison)(TYPE, TYPE));

    void AVL_TraverseBreadth(void (*process)(TYPE));


    vector<pair<TYPE, int>> AVL_ScanForResults(int (*process)(TYPE, TYPE), TYPE _target);

    void _scanForResults(int (*process)(TYPE, TYPE), NODE<TYPE> *root, TYPE _target, vector<pair<TYPE, int>> &_v);

    void AVL_GetStrictResults(KTYPE bound1, KTYPE bound2, vector<TYPE> &);

    void AVL_GetStrictResults(KTYPE bound1, KTYPE bound2, priority_queue<TYPE> &);

    void _getNodesInRange(NODE<TYPE> *root, KTYPE bound1, KTYPE bound2, vector<TYPE> &_v);

    void _removeUncommon(NODE<TYPE> *ptr, double _bound, queue<TYPE> &_badEggs, bool (*evaluate)(TYPE, double));

    queue<TYPE> AVL_RemoveUncommon(bool (*evaluate)(TYPE, double), double _bound);

    void _getNodesInRange(NODE<TYPE> *root, KTYPE bound1, KTYPE bound2, priority_queue<TYPE> &_p);
}; // class AvlTree

/*	=================== Constructor ===================	
	Initializes private data.
	   Pre     class is being defined
	   Post    private data initialized
*/

template<class TYPE, class KTYPE>
AvlTree<TYPE, KTYPE>::AvlTree(void)
{
//	Statements 
    tree = NULL;
    count = 0;
}    //  Constructor

/*	==================== AVL_Insert ==================== 
	This function inserts new data into the tree.
	   Pre    dataIn contains data to be inserted
	   Post   data have been inserted or memory overflow 
	   Return success (true) or overflow (false)
*/

template<class TYPE, class KTYPE>
bool AvlTree<TYPE, KTYPE>::AVL_Insert(TYPE dataIn)
{
//	Local Definitions 
    NODE<TYPE> *newPtr;
    bool taller;

//	Statements 
    if (!(newPtr = new NODE<TYPE>))
        return false;
    newPtr->bal = EH;
    newPtr->right = NULL;
    newPtr->left = NULL;
    newPtr->data = dataIn;

    tree = _insert(tree, newPtr, taller);
    count++;
    return true;
}    //  Avl_Insert

/*	======================= _insert =======================
	This function uses recursion to insert the new data into 
	a leaf node in the AVL tree.
	   Pre    application has called AVL_Insert, which passes 
	          root and data pointers
	   Post   data have been inserted
	   Return pointer to [potentially] new root
*/

template<class TYPE, class KTYPE>
NODE<TYPE> *AvlTree<TYPE, KTYPE>
::_insert(NODE<TYPE> *root,
          NODE<TYPE> *newPtr,
          bool &taller)
{
//	Statements   
    if (!root)
    {
        root = newPtr;
        taller = true;
        return root;
    } //  if NULL tree

    if (newPtr->data.key < root->data.key)
    {
        root->left = _insert(root->left,
                             newPtr,
                             taller);
        if (taller)
            //  Left subtree is taller
            switch (root->bal)
            {
                case LH: // Was left high--rotate
                    root = leftBalance(root, taller);
                    break;

                case EH: // Was balanced--now LH
                    root->bal = LH;
                    break;

                case RH: // Was right high--now EH
                    root->bal = EH;
                    taller = false;
                    break;
            } // switch
    } //  new < node
    else
        //  new data >= root data
    {
        root->right = _insert(root->right,
                              newPtr,
                              taller);
        if (taller)
            // Right subtree is taller
            switch (root->bal)
            {
                case LH: // Was left high--now EH
                    root->bal = EH;
                    taller = false;
                    break;

                case EH: // Was balanced--now RH
                    root->bal = RH;
                    break;

                case RH: // Was right high--rotate
                    root = rightBalance(root, taller);
                    break;
            } //  switch
    } //  else new data >= root data
    return root;
}    //  _insert

/*	===================== leftBalance ===================== 
	The tree is out of balance to the left. This function 
	rotates the tree to the right.
	   Pre     the tree is left high 
	   Post    balance restored  
	   Returns potentially new root 
*/

template<class TYPE, class KTYPE>
NODE<TYPE> *AvlTree<TYPE, KTYPE>
::leftBalance(NODE<TYPE> *root,
              bool &taller)
{
// 	Local Definitions 
    NODE<TYPE> *rightTree;
    NODE<TYPE> *leftTree;

//	Statements 
    leftTree = root->left;
    switch (leftTree->bal)
    {
        case LH: // Left High--Rotate Right
            root->bal = EH;
            leftTree->bal = EH;

            // Rotate Right
            root = rotateRight(root);
            taller = false;
            break;
        case EH: // This is an error
            cout << "\n\a\aError in leftBalance\n";
            exit(100);
        case RH: // Right High - Requires double rotation:
            // first left, then right
            rightTree = leftTree->right;
            switch (rightTree->bal)
            {
                case LH:
                    root->bal = RH;
                    leftTree->bal = EH;
                    break;
                case EH:
                    root->bal = EH;
                    leftTree->bal = EH;
                    break;
                case RH:
                    root->bal = EH;
                    leftTree->bal = LH;
                    break;
            } //  switch rightTree

            rightTree->bal = EH;
            //  Rotate Left
            root->left = rotateLeft(leftTree);

            // Rotate Right
            root = rotateRight(root);
            taller = false;
    } // switch leftTree
    return root;
}    // leftBalance

/*	===================== rotateLeft ====================== 
	This function exchanges pointers so as to rotate the  
	tree to the left.
	   Pre  root points to tree to be rotated 
	   Post NODE rotated and new root returned 
*/

template<class TYPE, class KTYPE>
NODE<TYPE> *AvlTree<TYPE, KTYPE>
::rotateLeft(NODE<TYPE> *root)
{
//	Local Definitions 
    NODE<TYPE> *tempPtr;

//	Statements 
    tempPtr = root->right;
    root->right = tempPtr->left;
    tempPtr->left = root;

    return tempPtr;
}    //  rotateLeft

/*	===================== rotateRight ===================== 
	This function exchanges pointers to rotate the tree
	to the right.
	   Pre   root points to tree to be rotated 
	   Post  NODE rotated and new root returned 
*/

template<class TYPE, class KTYPE>
NODE<TYPE> *AvlTree<TYPE, KTYPE>
::rotateRight(NODE<TYPE> *root)
{
//	Local Definitions 
    NODE<TYPE> *tempPtr;

//	Statements 
    tempPtr = root->left;
    root->left = tempPtr->right;
    tempPtr->right = root;

    return tempPtr;
}    //  rotateRight

/*	====================  rightBalance ===================
	The tree is out-of-balance to the right. This function 
	rotates the tree to the left.
	   Pre     The tree is right high 
	   Post    Balance restored 
	   Returns potentially new root 
*/

template<class TYPE, class KTYPE>
NODE<TYPE> *AvlTree<TYPE, KTYPE>
::rightBalance(NODE<TYPE> *root, bool &taller)
{

//	Local Definitions 
    NODE<TYPE> *rightTree;
    NODE<TYPE> *leftTree;

//	Statements 
    rightTree = root->right;
    switch (rightTree->bal)
    {
        case LH: // Left High - Requires double rotation:
            //             first right, then left
            leftTree = rightTree->left;

            //  First Rotation - Left
            switch (leftTree->bal)
            {
                case RH:
                    root->bal = LH;
                    rightTree->bal = EH;
                    break;

                case EH:
                    root->bal = EH;
                    rightTree->bal = EH;
                    break;

                case LH:
                    root->bal = EH;
                    rightTree->bal = RH;
                    break;
            } //  switch
            leftTree->bal = EH;

            root->right = rotateRight(rightTree);
            root = rotateLeft(root);
            taller = false;
            break;

        case EH: // Deleting from balanced node
            root->bal = EH;
            taller = false;
            break;

        case RH: // Right High - Rotate Left
            root->bal = EH;
            rightTree->bal = EH;
            root = rotateLeft(root);
            taller = false;
            break;
    } // switch
    return root;
}    //  rightBalance

/*	====================== AVL_Delete ====================== 
	This function deletes a node from the tree and rebalances 
	it if necessary. 
	   Pre    dltKey contains key to be deleted
	   Post   the node is deleted and its space recycled
	          -or- an error code is returned 
	   Return success (true) or not found (false)
*/

template<class TYPE, class KTYPE>
bool AvlTree<TYPE, KTYPE>::AVL_Delete(KTYPE dltKey)
{
//	Local Definitions 
    bool shorter;
    bool success;

    NODE<TYPE> *newRoot;

//	Statements 
    newRoot = _delete(tree, dltKey, shorter, success);
    if (success)
    {
        tree = newRoot;
        count--;
    } // if
    return success;
}    // AVL_Delete

/*	======================== _delete ======================= 
	This function deletes a node from the tree and rebalances 
	it if necessary. 
	   Pre    dltKey contains key of node to be deleted
	          shorter is Boolean indicating tree is shorter
	   Post   the node is deleted and its space recycled
	          -or- if key not found, tree is unchanged 
	   Return true if deleted, false if not found
	          pointer to root
*/

template<class TYPE, class KTYPE>
NODE<TYPE> *AvlTree<TYPE, KTYPE>
::_delete(NODE<TYPE> *root,
          KTYPE dltKey,
          bool &shorter,
          bool &success)
{
//  Local Definitions 
    NODE<TYPE> *dltPtr;
    NODE<TYPE> *exchPtr;
    NODE<TYPE> *newRoot;

// 	Statements 
    if (!root)
    {
        shorter = false;
        success = false;
        return NULL;
    } //  if -- base case

    if (dltKey < root->data.key)
    {
        root->left = _delete(root->left, dltKey,
                             shorter, success);
        if (shorter)
            root = dltRightBalance(root, shorter);
    } // if less
    else if (dltKey > root->data.key)
    {
        root->right = _delete(root->right, dltKey,
                              shorter, success);
        if (shorter)
            root = dltLeftBalance(root, shorter);
    } //  if greater
    else
        //  Found equal node
    {
        dltPtr = root;
        if (!root->right)
            // Only left subtree
        {
            newRoot = root->left;
            success = true;
            shorter = true;
            delete (dltPtr);
            return newRoot;            //  base case
        } //  if true
        else if (!root->left)
            //  Only right subtree
        {
            newRoot = root->right;
            success = true;
            shorter = true;
            delete (dltPtr);
            return newRoot;        // base case
        } //  if
        else
            //  Delete NODE has two subtrees
        {
            exchPtr = root->left;
            while (exchPtr->right)
                exchPtr = exchPtr->right;

            root->data = exchPtr->data;
            root->left = _delete(root->left,
                                 exchPtr->data.key,
                                 shorter,
                                 success);
            if (shorter)
                root = dltRightBalance(root, shorter);
        } //  else

    } // equal node
    return root;
}    // _delete

// ================== dltLeftBalance ==================
/*	The tree is out-of-balance to the left (left high)--
	rotates the tree to the right.
	   Pre     The tree is left high 
	   Post    Balance has been restored 
	   Returns potentially new root 
*/

template<class TYPE, class KTYPE>
NODE<TYPE> *AvlTree<TYPE, KTYPE>
::dltLeftBalance(NODE<TYPE> *root,
                 bool &smaller)
{
//	Local Definitions 
    NODE<TYPE> *rightTree;
    NODE<TYPE> *leftTree;

//	Statements 
    switch (root->bal)
    {
        case RH:
            root->bal = EH;
            break;

        case EH:  // Delete occurred on right
            root->bal = LH;
            smaller = false;
            break;

        case LH:
            leftTree = root->left;
            switch (leftTree->bal)
            {
                case LH:
                case EH: // Rotate Single Left
                    if (leftTree->bal == EH)
                    {
                        root->bal = LH;
                        leftTree->bal = RH;
                        smaller = false;
                    } // if
                    else
                    {
                        root->bal = EH;
                        leftTree->bal = EH;
                    } // else

                    root = rotateRight(root);
                    break;

                case RH:    //Double Rotation
                    rightTree = leftTree->right;
                    switch (rightTree->bal)
                    {
                        case LH:
                            root->bal = RH;
                            leftTree->bal = EH;
                            break;
                        case EH:
                            root->bal = EH;
                            leftTree->bal = EH;
                            break;
                        case RH:
                            root->bal = EH;
                            leftTree->bal = LH;
                            break;
                    } //  switch
                    rightTree->bal = EH;
                    root->left = rotateLeft(leftTree);
                    root = rotateRight(root);
                    break;
            } //  switch : leftTree->bal

    } //  switch : root->bal
    return root;
}    // dltLeftBalance

/*	=================== dltRightBalance ==================  
	The tree is shorter after a delete on the left. 
	Adjust the balance factors and rotate the tree 
	to the left if necessary.
	   Pre     the tree is shorter 
	   Post    balance factors adjusted and balance restored 
	   Returns potentially new root 
*/

template<class TYPE, class KTYPE>
NODE<TYPE> *AvlTree<TYPE, KTYPE>
::dltRightBalance(NODE<TYPE> *root,
                  bool &shorter)
{
//  Local Definitions 
    NODE<TYPE> *rightTree;
    NODE<TYPE> *leftTree;

//	Statements
    switch (root->bal)
    {
        case LH: //  Deleted Left--Now balanced
            root->bal = EH;
            break;
        case EH: //  Now Right high
            root->bal = RH;
            shorter = false;
            break;
        case RH: //  Right High - Rotate Left
            rightTree = root->right;
            if (rightTree->bal == LH)
                // Double rotation required
            {
                leftTree = rightTree->left;

                switch (leftTree->bal)
                {
                    case LH:
                        rightTree->bal = RH;
                        root->bal = EH;
                        break;
                    case EH:
                        root->bal = EH;
                        rightTree->bal = EH;
                        break;
                    case RH:
                        root->bal = LH;
                        rightTree->bal = EH;
                        break;
                } // switch

                leftTree->bal = EH;

                // Rotate Right then Left
                root->right = rotateRight(rightTree);
                root = rotateLeft(root);
            } //  if rightTree->bal == LH
            else
            {
                //  Single Rotation Only
                switch (rightTree->bal)
                {
                    case LH:
                    case RH:
                        root->bal = EH;
                        rightTree->bal = EH;
                        break;
                    case EH:
                        root->bal = RH;
                        rightTree->bal = LH;
                        shorter = false;
                        break;
                } // switch rightTree->bal
                root = rotateLeft(root);
            } // else
    } //  switch root bal
    return root;
}    //  dltRightBalance

/*	==================== AVL_Retrieve ===================  
	Retrieve node searches the tree for the node containing 
	the requested key and returns pointer to its data.
	   Pre     dataOut is variable to receive data
	   Post    tree searched and data returned
	   Return  true if found, false if not found
*/

template<class TYPE, class KTYPE>
bool AvlTree<TYPE, KTYPE>
::AVL_Retrieve(KTYPE key, TYPE &dataOut)
{
//	Local Definitions
    NODE<TYPE> *node;

//	Statements 
    if (!tree)
        return false;

    node = _retrieve(key, tree);
    if (node)
    {
        dataOut = node->data;
        return true;
    } // if found
    else
        return false;
}    //  AVL_Retrieve

/*	===================== _retrieve ===================== 
	Retrieve searches tree for node containing requested 
	key and returns its data to the calling function.
	   Pre     AVL_Retrieve called: passes key to be located 
	   Post    tree searched and data pointer returned 
	   Return  address of matching node returned 
	           if not found, NULL returned 
*/

template<class TYPE, class KTYPE>
NODE<TYPE> *AvlTree<TYPE, KTYPE>::_retrieve(KTYPE key, NODE<TYPE> *root)
{
//	Statements 
    if (root)
    {
        if (key < root->data.key)
            return _retrieve(key, root->left);
        else if (key > root->data.key)
            return _retrieve(key, root->right);
        else
            // Found equal key
            return (root);
    } // if root
    else
        //Data not in tree
        return root;
}    //  _retrieve


template<class TYPE, class KTYPE>
bool AvlTree<TYPE, KTYPE>::AVL_Update(KTYPE key, TYPE newData)
{
//	Statements
    if (!tree)
        return false;

    NODE<TYPE> *node = _retrieve(key, tree);
    if (node)
    {
        node->data = newData;
        return true;
    } // if found
    else
        return false;
}

/*	==================== AVL_TraverseInOrder ====================
	Process tree using inorder traversal. 
	   Pre   process used to "visit" nodes during traversal 
	   Post  all nodes processed in LNR (inorder) sequence 
*/

template<class TYPE, class KTYPE>
void AvlTree<TYPE, KTYPE>
::AVL_TraverseInOrder(void (*process)(TYPE *dataProc))
{
//	Statements 
    _traversalInOrder(process, tree);
    return;
}    // end AVL_TraverseInOrder

/*	===================== _traversalInOrder =====================
	Traverse tree using inorder traversal. To process a
	node, we use the function passed when traversal is called.
	   Pre   tree has been created (may be null) 
	   Post  all nodes processed 
*/

template<class TYPE, class KTYPE>
void AvlTree<TYPE, KTYPE>
::_traversalInOrder(void(*process)(TYPE *dataproc),
                    NODE<TYPE> *root)
{
//	Statements 
    if (root)
    {
        _traversalInOrder(process, root->left);
        process(&root->data);
        _traversalInOrder(process, root->right);
    } //  if
    return;
}    //  _traversalInOrder

template<class TYPE, class KTYPE>
vector<pair<TYPE, int>> AvlTree<TYPE, KTYPE>::AVL_ScanForResults(int(*process)(TYPE n1, TYPE n2), TYPE _target)
{
    vector<pair<TYPE, int>> _v;
    _scanForResults(process, tree, _target, _v);
    return _v;
}

template<class TYPE, class KTYPE>
void AvlTree<TYPE, KTYPE>::_scanForResults(int(*process)(TYPE n1, TYPE n2), NODE<TYPE> *root, TYPE _target, vector<pair<TYPE, int>> &_v)
{
//	Statements
    if (root)
    {
        _scanForResults(process, root->left, _target, _v);
        int d = process(root->data, _target);
        _v.push_back(make_pair(root->data, d));
        _scanForResults(process, root->right, _target, _v);
    } //  if
}    //  _traversalInOrder


/*	==================== AVL_TraverseBreadth ====================
	Process tree using Breadth-First traversal.
	   Pre   process used to "visit" nodes during traversal
	   Post  all nodes processed in Breadth-First sequence
*/

template<class TYPE, class KTYPE>
void AvlTree<TYPE, KTYPE>
::AVL_TraverseBreadth(void (*process)(TYPE dataProc))
{
    //	Statements
    _traversalBreadth(process, tree);
    return;
}    // end AVL_TraverseInOrder

/*	===================== _traversalInOrder =====================
	Traverse tree using inorder traversal. To process a
	node, we use the function passed when traversal is called.
	   Pre   tree has been created (may be null)
	   Post  all nodes processed
*/

template<class TYPE, class KTYPE>
void AvlTree<TYPE, KTYPE>
::_traversalBreadth(void(*process)(TYPE dataproc), NODE<TYPE> *root)
{
    if (root == nullptr)
        return;

    queue<NODE<TYPE> *> q;
    q.push(root);

    int c = 0;
    while (!q.empty())
    {
        c++;
        NODE<TYPE> *node = q.front();
        process(node->data);
        q.pop();

        if (node->left != nullptr)
            q.push(node->left);

        if (node->right != nullptr)
            q.push(node->right);
    }
}


/*	=================== AVL_Empty ==================	
	Returns true if tree is empty, false if any data.
	   Pre      tree has been created; may be null 
	   Returns  true if tree empty, false if any data 
*/

template<class TYPE, class KTYPE>
bool AvlTree<TYPE, KTYPE>::AVL_Empty(void)
{
//	Statements 
    return (count == 0);
}    //  AVL_Empty

/*	=================== AVL_Full =================== 
	If there is no room for another node, returns true.
	   Pre      tree has been created
	   Returns  true if no room, false if room 
*/

template<class TYPE, class KTYPE>
bool AvlTree<TYPE, KTYPE>::AVL_Full(void)
{
//	Local Definitions 
    NODE<TYPE> *newPtr;

//	Statements 
    newPtr = new NODE<TYPE>;
    if (newPtr)
    {
        delete newPtr;
        return false;
    } // if
    else
        return true;
}    //  AVL_Full

/*	=================== AVL_Count ===================
	Returns number of nodes in tree.
	   Pre     tree has been created
	   Returns tree count 
*/

template<class TYPE, class KTYPE>
int AvlTree<TYPE, KTYPE>::AVL_Count(void)
{
// 	Statements 
    return (count);
}    //  AVL_Count

/*	=================== Destructor =================== 
	Deletes all data in tree and recycles memory.
	The nodes are deleted by calling a recursive
	function to traverse the tree in inorder sequence.
	   Pre      tree is a pointer to a valid tree 
	   Post     all data have been deleted 
*/

template<class TYPE, class KTYPE>
AvlTree<TYPE, KTYPE>::~AvlTree(void)
{
//	Statements 
    if (tree)
        _destroyAVL(tree);
}    // Destructor

/*	=================== _destroyAVL =================== 
	Deletes all data in tree and recycles memory.
	The nodes are deleted by calling a recursive
	function to traverse the tree in postorder sequence.   
	   Pre   tree is being destroyed 
	   Post  all data have been deleted 
*/

template<class TYPE, class KTYPE>
void AvlTree<TYPE, KTYPE>
::_destroyAVL(NODE<TYPE> *root)
{
//	Statements 
    if (root)
    {
        _destroyAVL(root->left);
        _destroyAVL(root->right);
        delete root;
    } // if
    return;
}    //  _destroyAVL

/*  ============================= AVL_Print ============================= 
	This function prints the AVL tree by calling a recursive inorder 
	traversal. NOTE: THIS IS NOT AN APPLICATION ADT FUNCTION. IT IS 
	USED ONLY FOR DEBUGGING PURPOSES.
	
	To correctly visualize the tree when turned sideways, the actual 
	traversal is RNL.
	Pre	 Tree must be initialized. Null tree is OK.
		 Level is node level: root == 0
	Post Tree has been printed.
*/
template<class TYPE, class KTYPE>
void AvlTree<TYPE, KTYPE>::AVL_Print(void)
{
/*  statements */
    cout << "Total number of words in the tree: " << to_string(AVL_Count()) << endl << endl;
    _print(tree, 0);
    return;
}   /* AVL_PRINT */

/*  ============================= _print ============================= 
	This function is not a part of the ADT. It is included to verify
	that the tree has been properly built by printing out the tree
	structure.
*/

/*  This function uses recursion to Search_PrintRow the tree. At each level, the
    level number is printed along with the node contents (an integer).
    Pre		root is the root of a tree or subtree
            level is the level of the tree: tree root is 0
    Post    Tree has been printed.
*/
template<class TYPE, class KTYPE>
void AvlTree<TYPE, KTYPE>::_print(NODE<TYPE> *root,
                                  int level)
{
    /* Local Definitions */
    int i;

    /* Statements */
    if (root)
    {
        _print(root->right, level + 1);

        cout << "bal " << setw(3) << root->bal
             << ": Level " << setw(3) << level;

        for (i = 0; i <= level; i++)
            cout << "....";
        cout << setw(3) << root->data.key;
        if (root->bal == LH)
            cout << " (LH)\n";
        else if (root->bal == RH)
            cout << " (RH)\n";
        else
            cout << " (EH)\n";

        _print(root->left, level + 1);
    } /* if */

} /* AVL_Print */

template<class TYPE, class KTYPE>
vector<pair<TYPE, int>> AvlTree<TYPE, KTYPE>::AVL_GetClosestNodes(TYPE _node, int (*comparison)(TYPE n1, TYPE n2))
{
    // Initialize minimum difference
    int minDiff = INT_MAX;
    vector<pair<TYPE, int>> minDiffNodes;

    // Find value of minDiffNodes (Closest key
    // in tree with k)
    _closestNode(tree, _node, minDiff, minDiffNodes, comparison);
    reverse(minDiffNodes.begin(), minDiffNodes.end());

    return minDiffNodes;
}

template<class TYPE, class KTYPE>
void AvlTree<TYPE, KTYPE>::_closestNode(NODE<TYPE> *ptr, TYPE _node, int &minDiff, vector<pair<TYPE, int>> &minDiffNodes, int (*comparison)(TYPE n1, TYPE n2))
{
    if (ptr == NULL)
        return;

    int diff = comparison(ptr->data, _node);

    // If the target itself is present (no difference in keys)
    if (diff == 0)
    {
        minDiffNodes.push_back(make_pair(ptr->data, diff));
        return;
    }

    // update minDiff and minDiffNodes by checking
    // current node value
    if (diff <= minDiff)
    {
        minDiff = diff;
    }
    minDiffNodes.push_back(
            make_pair(ptr->data, diff)); //! move to inside the above for loop if you want to only get prograssive results (less results but more accurate when not looking at occurences)

    // if the target is less than ptr->key then move in
    // left subtree else in right subtree
    if (_node.key < ptr->data.key)
        _closestNode(ptr->left, _node, minDiff, minDiffNodes, comparison);
    else
        _closestNode(ptr->right, _node, minDiff, minDiffNodes, comparison);
}

template<class TYPE, class KTYPE>
void AvlTree<TYPE, KTYPE>::AVL_GetStrictResults(KTYPE bound1, KTYPE bound2, vector<TYPE> &_v)
{
    _getNodesInRange(tree, bound1, bound2, _v);
}

template<class TYPE, class KTYPE>
void AvlTree<TYPE, KTYPE>::_getNodesInRange(NODE<TYPE> *root, KTYPE bound1, KTYPE bound2, vector<TYPE> &_v)
{
    /* base case */
    if (!root)
        return;

    /* Since the desired o/p is sorted,
        recurse for left subtree first
        If root->data is greater than bound1,
        then only we can get o/p keys
        in left subtree */
    if (bound1 < root->data.key)
        if (INCL_PHRASES || root->data.wordCount == 1)
            _getNodesInRange(root->left, bound1, bound2, _v);

    /* if root's data lies in range,
    then prints root's data */
    if (bound1 <= root->data.key && bound2 > root->data.key)
        if (INCL_PHRASES || root->data.wordCount == 1)
            _v.push_back(root->data);


    /* If root->data is smaller than bound2,
        then only we can get o/p keys
        in right subtree */
    if (bound2 > root->data.key)
        _getNodesInRange(root->right, bound1, bound2, _v);
}

template<class TYPE, class KTYPE>
void AvlTree<TYPE, KTYPE>::AVL_GetStrictResults(KTYPE bound1, KTYPE bound2, priority_queue<TYPE> &_p)
{
    _getNodesInRange(tree, bound1, bound2, _p);
}

template<class TYPE, class KTYPE>
void AvlTree<TYPE, KTYPE>::_getNodesInRange(NODE<TYPE> *root, KTYPE bound1, KTYPE bound2, priority_queue<TYPE> &_p)
{
    /* base case */
    if (!root)
        return;

    /* Since the desired o/p is sorted,
        recurse for left subtree first
        If root->data is greater than bound1,
        then only we can get o/p keys
        in left subtree */
    if (bound1 < root->data.key)
        _getNodesInRange(root->left, bound1, bound2, _p);

    /* if root's data lies in range,
    then prints root's data */
    if (bound1 <= root->data.key && bound2 > root->data.key)
        if (INCL_PHRASES || root->data.wordCount == 1)
            _p.push(root->data);

    /* If root->data is smaller than bound2,
        then only we can get o/p keys
        in right subtree */
    if (bound2 > root->data.key)
        _getNodesInRange(root->right, bound1, bound2, _p);
}

template<class TYPE, class KTYPE>
queue<TYPE> AvlTree<TYPE, KTYPE>::AVL_RemoveUncommon(bool (*evaluate)(TYPE node, double bound), double _bound)
{
    queue<TYPE> _badEggs;

    _removeUncommon(tree, _bound, _badEggs, evaluate);

    return _badEggs;
}

template<class TYPE, class KTYPE>
void AvlTree<TYPE, KTYPE>::_removeUncommon(NODE<TYPE> *ptr, double _bound, queue<TYPE> &_badEggs, bool (*evaluate)(TYPE node, double _bound))
{
    if (!ptr)
        return;

    queue<NODE<TYPE> *> q;
    q.push(ptr);

    while (!q.empty())
    {
        NODE<TYPE> *node = q.front();

        if (evaluate(node->data, _bound))
            _badEggs.push(node->data);
        q.pop();

        if (node->left != nullptr)
            q.push(node->left);

        if (node->right != nullptr)
            q.push(node->right);
    }
}