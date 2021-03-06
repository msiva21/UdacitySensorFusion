/* \author Aaron Brown */
// Quiz on implementing kd tree

#include "../../render/render.h"

// Structure to represent node of KD-tree
struct Node
{
	std::vector<float> point; // first one is x, second one is y
	int id;
	Node* left;
	Node* right;

	Node(std::vector<float> arr, int setId)
	:	point(arr), id(setId), left(nullptr), right(nullptr)
	{}
};

struct KdTree
{
	Node* root;

	KdTree()
	: root(nullptr)
	{}

	void insert(std::vector<float> point, int id)
	{
		// TODO (done): Fill in this function to insert a new point into the tree
		// the function should create a new node and place correctly with in the root
		Node* curr = root;
		Node* prev = nullptr;
		bool is_left_child = true;
		int depth = 0;

		while (curr)
		{
			prev = curr;
			if (depth % 2 == 0) // check x split
			{
				if (point[0] > curr->point[0])
				{
					curr = curr->right;
					is_left_child = false;
				}
				else
				{
					curr = curr->left;
					is_left_child = true;
				}
			}
			else // check y split
			{
				if (point[1] > curr->point[1])
				{
					curr = curr->right;
					is_left_child = false;
				}
				else
				{
					curr = curr->left;
					is_left_child = true;
				}
			}
			depth++;
		}

		// connect
		curr = new Node(point, id);
		if (prev)
		{
			if (is_left_child)
				prev->left = curr;
			else
				prev->right = curr;
		}
		else
			root = curr;
	}


	void searchSubtree(Node* node,
					   std::vector<float>& target, 
					   float distanceTol, int depth, 
					   std::vector<int>& ids)
	{
		if (node == nullptr)
			return;

		// check if node belongs to the target BBOX
		if ( fabs(node->point[0] - target[0]) < distanceTol &&
			 fabs(node->point[1] - target[1]) < distanceTol )
			ids.push_back(node->id);

		// check next steps
		if (depth % 2 == 0) // compare x
		{
			if (target[0] - distanceTol <= node->point[0]) // o----x----o , x is target, os are distanceTol ends (same below)
				searchSubtree(node->left, target, distanceTol, depth + 1, ids);
			if (target[0] + distanceTol >= node->point[0])
				searchSubtree(node->right, target, distanceTol, depth + 1, ids);
		}
		else // compare y
		{
			if (target[1] - distanceTol <= node->point[1])
				searchSubtree(node->left, target, distanceTol, depth + 1, ids);
			if (target[1] + distanceTol >= node->point[1])
				searchSubtree(node->right, target, distanceTol, depth + 1, ids);
		}
	}

	// return a list of point ids in the tree that are within distance of target
	std::vector<int> search(std::vector<float> target, float distanceTol)
	{
		std::vector<int> ids;
		// call the recursive helper function above
		searchSubtree(root, target, distanceTol, 0, ids);
		return ids;
	}
};