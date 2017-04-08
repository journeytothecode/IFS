#ifndef AVLTree_H_
#define AVLTree_H_

#include <vector>
#include <iostream> //for NULL bad_alloc and vector
using std::vector;
using std::bad_alloc;

template <class ItemType>
struct Node
{
	ItemType item;        // Data portion
	Node<ItemType>*left;   // Index to left child
	Node<ItemType>*right;  // Index to right child
	int height;

	int Height();
};

template <class ItemType>
class AVLSearchTree
{
public:
	Node<ItemType> *root;
	//private:
	//	#warning use size_t instead
	unsigned int numberOfNodes;
	AVLSearchTree();
	virtual ~AVLSearchTree();
public:
public:
	bool add(const ItemType& newData);
	bool remove(const ItemType& data); 
	
private:
	void balancePath(Node<ItemType>* &node);
		
	//rotation methods
	void leftRotate(Node<ItemType>* &nodeA, Node<ItemType>* &parentOfA);
	void rightRotate(Node<ItemType>* &nodeA, Node<ItemType>* &parentOfA);
		
	//helper methods
	void updateHeight(Node<ItemType>* node);
	int balanceFactor(Node<ItemType>* subtreePtr);
	/*Returns a path from the root leading to specified element*/
	public: std::vector<Node<ItemType>*>* getPath(ItemType item) const;
	Node<ItemType>* createNewNodeWithItem(ItemType item);

	bool isEmpty() const;
	int getHeight() const;
	int getNumberOfNodes() const;
	ItemType getRootData() const ;
	void setRootData(const ItemType& newData);
	void clear();
	ItemType* getEntry(const ItemType& anEntry) const ;
	bool contains(const ItemType& anEntry) const;

	//------------------------------------------------------------
	// Public Traversals Section.
	//------------------------------------------------------------
	void preorderTraverse(void visit(ItemType&)) const;
	void inorderTraverse(void visit(ItemType&)) const;
	void postorderTraverse(void visit(ItemType&)) const;

	//------------------------------------------------------------
	// Protected Utility Methods Section:

	void insertInOrder(Node<ItemType>* &subtrePtr, Node<ItemType> *newNodePtr);
	ItemType* searchForEntry(const ItemType& anEntry, Node<ItemType>* treePtr) const;
	bool findNodeToRemove(Node<ItemType>* &subtrePtr, ItemType data);
	void removeNode(Node<ItemType>*&);
	Node<ItemType>* locateInOrderSuccessor(Node<ItemType>* node);
	Node<ItemType>* createNewNode(ItemType item);

	// Recursive helper methods for the public methods.
	void preTraverse(void visit(ItemType&), Node<ItemType>* treePtr)const;
	void inTraverse(void visit(ItemType&), Node<ItemType>* treePtr)const;
	void postTraverse(void visit(ItemType&), Node<ItemType>* treePtr)const;
	void freeTree(Node<ItemType> *&root);
	void freeTree();
};

template <class ItemType>
std::vector<Node<ItemType>*>* AVLSearchTree<ItemType>::getPath(ItemType item) const
{
	Node<ItemType>*currNode = this->root;
	if(!currNode)
		return NULL;

	//	#warning do not forget to delete vector to which pathElements points in order to release allocated memory
	std::vector<Node<ItemType>*> *pathElements = new std::vector<Node<ItemType>*>;
	while(currNode->item != item && currNode != NULL)
	{
		//
		pathElements->push_back(currNode);

		if(currNode->item > item){
			currNode = currNode->left;
		}else if(currNode->item < item){
			currNode = currNode->right;
		}	
	}
	//push node that item belongs to
	pathElements->push_back(currNode);

	return pathElements;
}
template <class ItemType>
Node<ItemType>* AVLSearchTree<ItemType>::createNewNodeWithItem(ItemType item)
{
	Node<ItemType>* newNode = createNewNode(item);
	newNode->height = 1;
	return newNode;
}
template <class ItemType>
int max_(int first, int second)
{
	return (first > second) ? first : second ;
}

template <class ItemType>
int AVLSearchTree<ItemType>::balanceFactor(Node<ItemType>* subtreePtr)
{
	//bf = h(r) - h(l)
	int balanceF = subtreePtr->right->Height() - subtreePtr->left->Height();
	if (balanceF < -1){
		return -2;
	}
	else if (balanceF > 1){
		return 2;
	}
	else
		return balanceF;
}

template <class ItemType>
void AVLSearchTree<ItemType>::updateHeight(Node<ItemType>* node)
{
#ifdef _DEBUG_
	assert(node!=NULL);
#endif
	node->height = 1 + (node->left->Height() > node->right->Height()?node->left->Height():node->right->Height() );
}

//rotation

template <class ItemType>
void AVLSearchTree<ItemType>::leftRotate(Node<ItemType>* &nodeA, Node<ItemType>* &parentOfA)
{
	Node<ItemType>* A = nodeA;
	Node<ItemType>* B = nodeA->right;

	if (nodeA == this->root){
		//B is the new root
		this->root = B;
	}
	else{
		if (nodeA == parentOfA->left){
			parentOfA->left = B;
		}
		else {
			parentOfA->right = B;
		}
	}
	A->right = B->left;
	B->left = A;

	updateHeight(A);
	updateHeight(B);
}

template <class ItemType>
void AVLSearchTree<ItemType>::rightRotate(Node<ItemType>* &nodeA, Node<ItemType>* &parentOfA)
{
	Node<ItemType>* A = nodeA;
	Node<ItemType>* B = nodeA->left;

	if (nodeA == this->root){
		//B is the new root
		this->root = B;
	}
	else{
		if (nodeA == parentOfA->left){
			parentOfA->left = B;
		}
		else {
			parentOfA->right = B;
		}
	}
	A->left = B->right;
	B->right = A;

	updateHeight(A);
	updateHeight(B);
}

template <class ItemType>
void AVLSearchTree<ItemType>::balancePath(Node<ItemType>* &node)
{
	//1.Get the path from the node that contains e to the root 
	vector<Node<ItemType>*>* pathFromNodeEToRoot = getPath(node->item);
	Node<ItemType>* parentOfA;
	//2. For each node in the path leading to the root

	for (int i=pathFromNodeEToRoot->size()-1; i >= 0; i--)
	{
		//3. Update height of each node up to root 
		updateHeight(pathFromNodeEToRoot->at(i));
		//4. Get parentOfA
		if(i >= 1){
			parentOfA = pathFromNodeEToRoot->at(i-1);
		}else{
			//if node is root, parent is NULL
			parentOfA = NULL;
		}
		//4. Calculate balanceFactor of each node, and if AVL property is violated
		//rebalance the tree, else do nothing
		//cout << node->item << " " << endl;

		switch(balanceFactor(pathFromNodeEToRoot->at(i))){
		case -2:
			if (balanceFactor(pathFromNodeEToRoot->at(i)->left) > 0){
				//LR imbalance
				this->leftRotate(pathFromNodeEToRoot->at(i)->left, pathFromNodeEToRoot->at(i));
			}
			//
			this->rightRotate(pathFromNodeEToRoot->at(i), parentOfA);
			break;
		case 2:
			if (balanceFactor(pathFromNodeEToRoot->at(i)->right) < 0){
				//RL imbalance
				this->rightRotate(pathFromNodeEToRoot->at(i)->right, pathFromNodeEToRoot->at(i));
			}
			this->leftRotate(pathFromNodeEToRoot->at(i), parentOfA);
			break;
		}
	}
}

template <class ItemType>
bool AVLSearchTree<ItemType>::add(const ItemType& newData)
{
	//1. Create new node and return to newNode 
	Node<ItemType>*newNode = NULL;
	try{
		newNode = AVLSearchTree<ItemType>::createNewNodeWithItem(newData);
	}
	catch (bad_alloc e){
		//cout << e.what();
	}
	//2. Call inorderInsert method and pass root of the tree along with new node 
	insertInOrder(root, newNode);

	//3. Restore AVL property if is violated 
	balancePath(newNode);

	numberOfNodes++;
	return true;
}

template <class ItemType>
bool AVLSearchTree<ItemType>::remove(const ItemType& data)
{
	//BinarySearchTree<ItemType>::remove(data);
	if(root == NULL)
		return false;// Element is not in the tree

	// Locate the node to be deleted and also locate its parent node
	Node<ItemType>* currNode = root;
	Node<ItemType>* parentNode = NULL;

	while(currNode != NULL){

		if(data > currNode->item){
			parentNode = currNode;
			currNode= currNode->right;
		}else if (data < currNode->item){
			parentNode = currNode;
			currNode = currNode->left;
		}else{
			break; //Element is in the tree pointed by currNode
		}
	}

	if(currNode == NULL)
		return false;//element is not founded

	// Case 1: current has no left children
	if(currNode->left == NULL){
		// Connect the parent with the right child of the current node
		if(parentNode == NULL)
			root = currNode->right;
		else {
			if(data < parentNode->item )
				parentNode->left = currNode->right;
			else
				parentNode->right = currNode->right;

			balancePath(parentNode);	
		}
	}
	else 
	{
		// Case 2: The current node has a left child
		// Locate the rightmost node in the left subtree of
		// the current node and also its parent
		Node<ItemType>*parentOfRightMost = currNode;
		Node<ItemType>*rightMost = currNode->left;

		while(rightMost != NULL){
			parentOfRightMost = rightMost;
			rightMost = rightMost->right;
		}

		currNode->item = rightMost->item;

		//delete rightMost
		if(parentOfRightMost->right == rightMost)
			parentOfRightMost->right = rightMost->left;
		else
			// Special case: parentOfRightMost is current
			parentOfRightMost->left = rightMost->left; 

		// Balance the tree if necessary
		balancePath(parentOfRightMost); 
	}
	return true;	
}

template <class ItemType>
int Node<ItemType>::Height(){

	if(this)
		return this->height;

	return 0;
}

//Method section for BinarySearchTree class
template <class ItemType>
AVLSearchTree<ItemType>::AVLSearchTree() : root(NULL), numberOfNodes(0)
{

}

template <class ItemType>
AVLSearchTree<ItemType>::~AVLSearchTree()
{
	freeTree();
}

template <class ItemType>
void AVLSearchTree<ItemType>::freeTree()
{
	freeTree(root);
}
template <class ItemType>
void AVLSearchTree<ItemType>::freeTree(Node<ItemType> *&root)
{
	if (!root)
		return;
	if (root->left)
	{
		freeTree(root->left);
	}
	if (root->right)
	{
		freeTree(root->right);
	}
	if (root)
	{
		removeNode(root);
	}
}
template <class ItemType>
bool AVLSearchTree<ItemType>::isEmpty() const
{
	return this->root == NULL;
}
template <class ItemType>
int AVLSearchTree<ItemType>::getNumberOfNodes() const
{
	return this->numberOfNodes;
}
template <class ItemType>
ItemType AVLSearchTree<ItemType>::getRootData() const
{
	return root->item;
}

template <class ItemType>
void AVLSearchTree<ItemType>::setRootData(const ItemType& newData)
{
	this->root->item = newData;
}

template <class ItemType>
Node<ItemType>* AVLSearchTree<ItemType>::createNewNode(ItemType item)
{
	Node<ItemType>* newNode = new Node<ItemType>;
	try {
		newNode = new Node<ItemType>;
	}catch(std::bad_alloc e){
		std::cout << "Bad alloc!";
	}
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->item = item;
	return newNode;
}

template <class ItemType>
bool AVLSearchTree<ItemType>::findNodeToRemove(Node<ItemType>* &subtrePtr, ItemType data)
{
	//if located
	if (subtrePtr == NULL)
	{
		return false;
	}
	if (subtrePtr->item == data)
	{
		//
		std::cout << "node to delete: " << subtrePtr->item << " " << subtrePtr << std::endl;;
		removeNode(subtrePtr);
	}
	else if (data < subtrePtr->item)
	{
		findNodeToRemove(subtrePtr->left, data);
	}
	else
	{
		findNodeToRemove(subtrePtr->right, data);
	}
}

template <class ItemType>
void AVLSearchTree<ItemType>::removeNode(Node<ItemType>* &nodeToRemove)
{
	Node<ItemType>* delPtr;
	/*three cases */
	/*is leaf when two pointers are null*/
	const bool isLeaf = !nodeToRemove->left && !nodeToRemove->right;

	/*node has one children when one of the two pointers is not null and the other is */
	const bool hasOneChild = (nodeToRemove->left && !nodeToRemove->right) ||
		(!nodeToRemove->left && nodeToRemove->right);

	/*node has two children when two of its pointers are not null*/
	const bool hasTwoChildren = nodeToRemove->left && nodeToRemove->right;

	if (isLeaf)
	{	//1. node is leaf
		delete nodeToRemove;
		nodeToRemove = NULL;
	}
	else if(hasOneChild)
	{
		//2. node has only one children
		delPtr = nodeToRemove;
		if (nodeToRemove->left)
		{
			nodeToRemove = nodeToRemove->left;
			delPtr->left = NULL;
		}
		else
		{
			nodeToRemove = nodeToRemove->right;
			delPtr->right = NULL;
		}
		delete delPtr;
		delPtr = NULL;
	}
	else if(hasTwoChildren){
		//3. node have two children
		//locate in order successor
		Node<ItemType>* inOrderSuccessor = locateInOrderSuccessor(nodeToRemove->right);
		nodeToRemove->item = inOrderSuccessor->item;
		removeNode(inOrderSuccessor);
	}

}

template <class ItemType>
Node<ItemType>* AVLSearchTree<ItemType>::locateInOrderSuccessor(Node<ItemType>* node)
{
	if (node->left == NULL)
	{
		return node;
	}
	else
	{
		return locateInOrderSuccessor(node->left);
	}

}

template <class ItemType>
void AVLSearchTree<ItemType>::insertInOrder(Node<ItemType>* &subtrePtr, Node<ItemType>* newNodePtr)
{
	if(subtrePtr == NULL)
	{
		//the insertion point is found
		subtrePtr = newNodePtr;

	}else if(subtrePtr->item < newNodePtr->item)
	{
		//search right sub-tree of subtrePtr
		insertInOrder(subtrePtr->right, newNodePtr);
	}else
	{
		insertInOrder(subtrePtr->left, newNodePtr);
	}
}

template <class ItemType>
ItemType* AVLSearchTree<ItemType>::getEntry(const ItemType& anEntry) const
{
	if (!root)
		return NULL;
	return searchForEntry(anEntry, this->root);
}
template <class ItemType>
ItemType* AVLSearchTree<ItemType>::searchForEntry(const ItemType& anEntry, Node<ItemType>* treePtr)const
{
	if (treePtr == NULL)
	{
		return NULL;
	}
	if (anEntry == treePtr->item)
	{
		//item found
		return &treePtr->item;
	}
	else if (anEntry > treePtr->item)
	{
		return searchForEntry(anEntry, treePtr->right);
	}
	else {
		return searchForEntry(anEntry, treePtr->left);
	}

}
//traverse section
template <class ItemType>
void AVLSearchTree<ItemType>::preorderTraverse(void visit(ItemType&))const
{
	preTraverse(visit, this->root);
}
template <class ItemType>
void AVLSearchTree<ItemType>::inorderTraverse(void visit(ItemType&)) const
{
	inTraverse(visit, this->root);
}

template <class ItemType>
void AVLSearchTree<ItemType>::postorderTraverse(void visit(ItemType&))const
{
	postTraverse(visit, this->root);
}
template <class ItemType>
void AVLSearchTree<ItemType>::preTraverse(void visit(ItemType&), Node<ItemType>* treePtr) const
{
	if(treePtr != NULL)
	{
		visit(treePtr->item);
		preTraverse(visit, treePtr->left);
		preTraverse(visit, treePtr->right);
	}
}
template <class ItemType>
void AVLSearchTree<ItemType>::inTraverse(void visit(ItemType&), Node<ItemType>* treePtr) const
{
	if(treePtr != NULL)
	{
		inTraverse(visit, treePtr->left);
		visit(treePtr->item);
		inTraverse(visit, treePtr->right);
	}
}
template <class ItemType>
void AVLSearchTree<ItemType>::postTraverse(void visit(ItemType&), Node<ItemType>* treePtr) const
{
	if(treePtr != NULL)
	{
		postTraverse(visit, treePtr->left);
		postTraverse(visit, treePtr->right);
		visit(treePtr->item);
	}
}


#endif