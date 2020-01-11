#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

//AVL tree node
struct Node
{
    int value;
    int height;
    Node *left;
    Node *right;
};

int height(Node *h)
{//update height
    if (h == NULL)
    {
        return 0;
    }

    return h->height;
}

Node *R_rotation(Node *root)
{//right rotation
    Node *newroot = root->left;
    root->left = newroot->right;
    newroot->right = root;

    //update height
    root->height = max(height(root->left), height(root->right)) + 1;
    newroot->height = max(height(newroot->left), height(newroot->right)) + 1;

    return newroot;
}

Node *L_rotation(Node *root)
{//left rotation
    Node *newroot = root->right;
    root->right = newroot->left;
    newroot->left = root;

    //update height
    root->height = max(height(root->left), height(root->right)) + 1;
    newroot->height = max(height(newroot->left), height(newroot->right)) + 1;

    return newroot;
}

int getheight(Node *N)
{//return the height
    if (N == NULL)
    {
        return 0;
    }
    return height(N->left) - height(N->right);
}

Node* insert(Node* root, int value)
{//insert value to existed tree
    if (root == NULL)
    {//adding value to tree first time
	    Node* temp = new Node;

	    temp->value = value;
	    temp->left = NULL;
	    temp->right = NULL;
	    temp->height = 1;

	    return temp;
    }

    if (value < root->value)
    {//insert smaller value
        root->left  = insert(root->left, value);
    }
    else if (value > root->value)
    {//insert greater value
        root->right = insert(root->right, value);
    }
    else
    {//when the value is already in the tree
        return root;
    }

    //update height
    root->height = max(height(root->left),height(root->right)) + 1;


    // unbalanced tree
    int balance = getheight(root);

    if (balance > 1)
    {
    	if(value < root->left->value)
    	{// left-left
        	return R_rotation(root);	
    	}
    	else
    	{//left-right
    		root->left =  L_rotation(root->left);
        	return R_rotation(root);
    	}
    	
    }

    if (balance < -1)
    { 
    	if(value < root->right->value)
    	{// right-left
	        root->right = R_rotation(root->right);
	        return L_rotation(root);
    	}
    	else
    	{//right-right
    		return L_rotation(root);
    	}
    	
    }

    return root;
}

Node* midvalue(Node* root)
{//fint the rightmost value
    Node* temp = root;
 
    while (temp->right != NULL)
    {
        temp = temp->right;
    }
 
    return temp;
}

Node* deletevalue(Node* root, int value)
{//delete value in the tree
    if (root == NULL)
    {
        return root;
    }
 
    if (value < root->value)
    {//delete smaller value
        root->left = deletevalue(root->left, value);
    }
    else if(value > root->value)
    {//delete greater value 
        root->right = deletevalue(root->right, value);
    }
    else
    {//one child or no child
        if(root->left == NULL || root->right == NULL)
        {
            Node *temp = root->left ? root->left : root->right;
 
            if (temp == NULL)
            {//no child
                temp = root;
                root = NULL;
            }
            else
            {//one child
            	*root = *temp; 
            }

            free(temp);
        }
        else
        {// tree with two children
            Node* temp = midvalue(root->left);
            root->value = temp->value;
            root->left = deletevalue(root->left, temp->value);
        }
    }
 
    if (root == NULL)
    {
      return root;
    }
 
    //update height
    root->height = 1 + max(height(root->left),height(root->right));
 
    //unbalanced tree
    int balance = getheight(root);

    if (balance > 1)
    {
    	if (getheight(root->left) >= 0)
    	{// left-left
        	return R_rotation(root);	
    	}
    	else
    	{// left-right
    		root->left =  L_rotation(root->left);
        	return R_rotation(root);
    	}
    	
    }
 	
	if (balance < -1)
    { 
    	if (getheight(root->right) > 0)
    	{// right-left 
	        root->right = R_rotation(root->right);
	        return L_rotation(root);
    	}
    	else
    	{// right-right
    		return L_rotation(root);
    	}
    }

    return root;
}



void inorder(Node *root)
{//print as inorder
	if(root != NULL)
	{
		inorder(root->left);
		cout << root->value << " ";
        inorder(root->right);
	}
}

void preorder(Node *root)
{//print as preorder
	if(root != NULL)
	{
		cout << root->value << " ";
		preorder(root->left);
        preorder(root->right);
	}
}

void postorder(Node *root)
{//print as postorder
	if(root != NULL)
	{
		postorder(root->left);
        postorder(root->right);
        cout << root->value << " ";
	}
}


int convertstr(string str)
{//convert str to int
	stringstream convert;

    convert.clear();
    convert.str("");
    convert << str;
    int numb;
    convert >> numb;

    return numb;
}



int main()
{
	string input;

	getline(cin,input);

	int len = input.length();
	vector<string> elements;
	string moves;

	//separate input
	int k = 0;
	string s_input ="";
	for (int i = 0 ; i < len; i++)
	{
	
		if (input[i] != ' ')
		{
			s_input += input[i];
		}

		if (input[i] == ' ')
		{
			elements.push_back(s_input);
			//cout << moves[k] << endl;
			s_input = "";
			k++;
		}

		if (i == len-1)
		{
			moves = s_input;
		}
	}
	
	//make tree
	Node* tree = NULL;

	int len1 = elements.size();
	for (int j = 0 ; j < len1; j++)
	{
		string temp = elements[j];
		string numb = "";

		int len2 = temp.length();
		for (int z = 1; z < len2; z++)
		{
			numb += temp[z];
		}

		int num = convertstr(numb);

		if (temp[0] == 'A')
		{
			tree = insert(tree,num);
		}
		else
		{
			tree = deletevalue(tree,num);
		}

	}

	//print
	if (tree == NULL)
	{
		cout << "EMPTY" << endl;
	}
	else if (moves == "IN")
	{
		inorder(tree);
		cout << endl;
	}
	else if (moves == "PRE")
	{
		preorder(tree);
		cout << endl;
	}
	else if (moves == "POST")
	{
		postorder(tree);
		cout << endl;
	}

	return 0;
}
