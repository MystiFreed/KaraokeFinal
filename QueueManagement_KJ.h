//This class houses the doubly linked list and kj functionality. This class is used to add a singer to the queue (in any order desired), remove
//a singer from the queue, or move people around in the queue.
//Mysti Freed
//mrfreed@dmacc.edu
//16-NOV-2019

//double check questions in modes below

#pragma once
#ifndef QUEUEMANAGEMENT_KJ_H
#define QUEUEMANAGEMENT_KJ_H
#include <iostream>

using namespace std;

//This is the QueueNode class, which creates a type used to store the node of the linked list (GADDIS pg 1148) with modifications for a doubly
//linked list (GADDIS pg 1153)


template <class T>

class QueueNode
{
public:
	T value; //this is the value housed in the node
	QueueNode<T> *next; //pointer to the next node
	QueueNode<T> *previous; //pointer to the previous node

	//constructor for the node
	QueueNode (T nodeValue)
	{
		value = nodeValue;
		next = nullptr;
		previous = nullptr;
	}
};

//This is the QueueManagement class, which will allow a KJ to manipulate the queue as singers add their names to the list or leave

template <class T>
class QueueManagement_KJ
{
private:
	QueueNode<T> *head; //this lists the head pointer
	QueueNode<T> *tail; //this lists the tail pointer
public:
	//constructor
	QueueManagement_KJ()
	{
		head = nullptr;
		tail = nullptr;
	}

	//destructor
	~QueueManagement_KJ();

	//linked list operations
	void appendNode(T);
	void appendNode(T, T);
	void insertNode(T, T);
	void deleteNode(T);
	void displayList() const;
	void displayFullList() const;
	bool verifyNameExists(string);
	void deleteList();
	T findNode(T&);
	//still need to add in doubly linked elements, such as insert between, etc
};

//This is the append operation, which adds a new singer to the very end of the list

template <class T>
void QueueManagement_KJ<T>::appendNode(T newS)
{
	QueueNode<T> *newNode; //this points to the new singer added to the queue
	QueueNode<T> *nodePtr; //this iterates through the list

	//create a new singer node and store the new singer in that node
	newNode = new QueueNode<T>(newS);

	//if this is the first singer of the night, add them to the first in queue and make newSinger the first node
	if (!head)
		head = newNode;
	else //if they're not the first one of the night, add them to the end
	{
		nodePtr = head;
		while (nodePtr->next)
			nodePtr = nodePtr->next; //get to the end of the list
		nodePtr->next = newNode; //set the singer to the last node (after the end is found
	}

	//do I need to add more here for the double part of doubly linked list? make sure to look this up
}

template <class T>
void QueueManagement_KJ<T>::appendNode(T newS, T newDN)
{
	QueueNode<T>* newNode; //this points to the new singer added to the queue
	QueueNode<T>* nodePtr; //this iterates through the list

	//create a new singer node and store the new singer in that node
	newNode = new QueueNode<T>(newS + "/" + newDN);

	//if this is the first singer of the night, add them to the first in queue and make newSinger the first node
	if (!head)
		head = newNode;
	else //if they're not the first one of the night, add them to the end
	{
		nodePtr = head;
		while (nodePtr->next)
			nodePtr = nodePtr->next; //get to the end of the list
		nodePtr->next = newNode; //set the singer to the last node (after the end is found
	}

	//do I need to add more here for the double part of doubly linked list? make sure to look this up
}

//This is the display operation, which will display the nodes in their order, for use in popping up the next 10 to the screen

template <class T>
void QueueManagement_KJ<T>::displayList() const
{
	QueueNode<T> *nodePtr; //used to iterate through the list

	//position the pointer at the beginning of the list
	nodePtr = head;
	const int COUNT_MAX = 10;
	int counter = 0;
	string displayName;

	while (counter < COUNT_MAX && nodePtr) //while the pointer points to a valid singer, generate the information in that singer
	{
		cout << nodePtr->value << endl; //generate the name of the singer
		nodePtr = nodePtr->next; //move to the next singer node
		counter++;
	}
}

//This is the insert operation, which will insert a new singer into a selected place in the queue
//placeholder only, as I need to look into this for the doubly linked list vs single

template <class T>
//credit to geeksforgeeks for the basic ideas: https://www.geeksforgeeks.org/insert-a-node-at-a-specific-position-in-a-linked-list/
//second: https://www.geeksforgeeks.org/rearrange-a-given-linked-list-in-place/ and https://ide.geeksforgeeks.org/1eGSEy

//1) Initialize current node as head.
//2) While next of current node is not null, do following
//a) Find the last node, remove it from the endand insert it as next of the current node.
//b) Move current to next to next of current
void QueueManagement_KJ<T>::insertNode(T movedNode, T beforenode)
{
	QueueNode<T> *newNode;
	QueueNode<T> *nodePtr; //used to iterate through the list
	QueueNode<T> *previousNode = nullptr; //points to the previous singer

	//allocate a new node and store the new value in that node
	newNode = new QueueNode<T>(movedNode);

	//string newData = movedNode;
	//nodePtr = head;
	int queueSize = 0; //this holds the size of the existing queue

	if (!head)
	{
		head = newNode;
		newNode->next = nullptr;
	}
	else
	{
		nodePtr = head;
		previousNode = nullptr;
		while (nodePtr != nullptr && nodePtr->value != beforenode)
		{
			previousNode = nodePtr;
			nodePtr = nodePtr->next;
		}
		if (previousNode == nullptr)
		{
			head = newNode;
			newNode->next = nodePtr;
		}
		else
		{
			previousNode->next = newNode;
			newNode->next = nodePtr;
		}
	}

}

//This is the delete operation, which will remove a singer who has left the venue
//placeholder only, as I need to look into this for the doubly linked list vs single

template <class T>
void QueueManagement_KJ<T>::deleteNode(T singerToRemove)
{
	QueueNode<T> *nodePtr; //used to iterate through the list
	QueueNode<T> *previousNode = nullptr; //points to the previous singer

	//if the head is empty, no need to do anything
	if (!head)
		return;

	if (head->value == singerToRemove) //if the head is the right singer to remove
	{
		nodePtr = head->next; 
		delete head;
		head = nodePtr;
	}
	else //if the first isn't the singer to remove, search the list
	{
		nodePtr = head;
		while (nodePtr != nullptr && nodePtr->value != singerToRemove)
		{
			previousNode = nodePtr;
			nodePtr = nodePtr->next;
		}
		//if the value isn't found, reset the ptr and delete it
		if (nodePtr)
		{
			previousNode->next = nodePtr->next;
			delete nodePtr;
		}
	}
}

//This cancels out the rest of the queue at the end of the night 
 
template <class T>
QueueManagement_KJ<T>::~QueueManagement_KJ()
{
	QueueNode<T> *nodePtr; //used to iterate through the list
	QueueNode<T> *nextNode; //points to the next singer in the list

	//start at the beginning of the queue
	nodePtr = head;
	while (nodePtr != nullptr) //while there are still singers in the queue
	{
		nextNode = nodePtr->next; //save the pointer for the next in line
		delete nodePtr; //delete the current node
		nodePtr = nextNode; //position the pointer at the next singer
	}
}

//template <class T>
//bool QueueManagement_KJ<T>::verifyNameExists(string dispname)
//{
//	QueueNode<T>* nodePtr; //used to iterate through the list
//
//	//position the pointer at the beginning of the list
//	nodePtr = head;
//
//	bool exists = false;
//	string tempCheck;
//	string nameToCheck = dispname;
//
//	while (nodePtr) //while the pointer points to a valid singer, generate the information in that singer
//	{
//		tempCheck = nodePtr->value; //generate the name of the singer
//		if (tempCheck == nameToCheck)
//		{
//			exists = true;
//			break;
//		}
//		nodePtr = nodePtr->next; //move to the next singer node
//	}
//	return exists;
//}

template <class T>
void QueueManagement_KJ<T>::displayFullList() const
{
	QueueNode<T>* nodePtr; //used to iterate through the list

	//position the pointer at the beginning of the list
	nodePtr = head;

	while (nodePtr) //while the pointer points to a valid singer, generate the information in that singer
	{
		cout<< nodePtr->value.display() << endl; //generate the name of the singer
		nodePtr = nodePtr->next; //move to the next singer node
	}
}
//returns copy of object
template <class T>
T QueueManagement_KJ<T>::findNode(T& searchObject)
{
	QueueNode<T>* nodePtr; //used to iterate through the list

	//position the pointer at the beginning of the list
	nodePtr = head;

	bool exists = false;
	T tempCheck;
	//string nameToCheck = dispname;

	while (nodePtr) //while the pointer points to a valid singer, generate the information in that singer
	{
		tempCheck = nodePtr->value; //generate the name of the singer
		if (tempCheck == searchObject)
		{
			return searchObject;
			break;
		}
		nodePtr = nodePtr->next; //move to the next singer node
	}

	return T();
}

#endif // !QUEUEMANAGEMENT_KJ_H

