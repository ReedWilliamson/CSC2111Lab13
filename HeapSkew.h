#if !defined (HEAPSKEW_H)
#define HEAPSKEW_H

#include "BinaryTree.h"
#include <iostream>
using namespace std;

template < class T >
class HeapSkew : public Drawable
{

   private:
      BinaryTree<T>* bt;
      int sze;
      BinaryTree<T>* merge(BinaryTree<T>* left, BinaryTree<T>* right);

      int (*compare_items) (T* item_1, T* item_2);

   public:
      HeapSkew(int (*comp_items) (T* item_1, T* item_2));
      virtual ~HeapSkew();

      bool heapIsEmpty();
      void heapInsert(T* item);
      T* heapRemove();

      void draw(Cairo::RefPtr<Cairo::Context> cr, int width, int height);
      void mouseClicked(int x, int y);

};

// this is a MAXHEAP (largest items to top alphabetical order)
template < class T >
HeapSkew<T>::HeapSkew(int (*comp_items) (T* item_1, T* item_2)) : Drawable()
{ 
   bt = new BinaryTree<T>();
   sze = 0;

   compare_items = comp_items;
}

template < class T >
HeapSkew<T>::~HeapSkew() 
{ 
   delete bt;
}  

template < class T >
bool HeapSkew<T>::heapIsEmpty()
{
   return sze == 0;
}

template < class T >
BinaryTree<T>* HeapSkew<T>::merge(BinaryTree<T>* left, BinaryTree<T>* right)
{
	if(left->isEmpty())
	{
		delete left;
		return right;
	}
	else if(right->isEmpty())
	{
		delete right;
		return left;
	}
	
	int comp = (compare_items)(left->getRootItem(),right->getRootItem());
	 
	if (comp < 0)
	{
		return merge(right,left);
	}
	
	BinaryTree<T>* LL = left->detachLeftSubtree();
	BinaryTree<T>* LR = left->detachRightSubtree();
	
	left->attachRightSubtree(LL);
	delete LL;
	
	if(LR->isEmpty())
	{
		left->attachLeftSubtree(right);
		delete LR;
		delete right;
		return left;
	}
	else
	{
		BinaryTree<T>* temp = merge(LR, right);
		left->attachLeftSubtree(temp);
		delete temp;
		return left;
	}
}

template < class T >
void HeapSkew<T>::heapInsert(T* item)
{
	BinaryTree<T>* right = new BinaryTree<T>(item);
	bt = merge(bt, right);
    sze++;
}

template < class T >
T* HeapSkew<T>::heapRemove()
{
	T* result = bt->getRootItem();
	BinaryTree<T>* left = bt->detachLeftSubtree();
	BinaryTree<T>* right = bt->detachRightSubtree();
	
	delete bt;
	
	bt = merge(left,right);

    sze--;
	
    return result;
}

template < class T >
void HeapSkew<T>::draw(Cairo::RefPtr<Cairo::Context> cr, int width, int height)
{
   bt->draw(cr, width, height);
}

template < class T >
void HeapSkew<T>::mouseClicked(int x, int y) {bt->mouseClicked(x, y);}

#endif
