/******************************************************************************/
/*!
\file   Lariat.cpp
\author Cody Morgan
\par    email: cody.morgan\@digipen.edu
\par    DigiPen login: cody.morgan
\par    Course: CS280
\par    Section A
\par    Assignment #2
\date   06 JAN 2019
\brief
  a Lariat array-list snake rope implementation
*/
/******************************************************************************/

#include <iostream>
#include <iomanip>

#define _DEBUG_ false
using std::cout;


                //// Construction / Destructors ////


/******************************************************************************
* brief : default constructor
*
* input : size of little array
*
* output: a new rope with one little array
******************************************************************************/
template<typename T, int Size>
Lariat<T, Size>::Lariat()
{
  addNode();
}

/******************************************************************************
* brief : copy constructor
*
* input : another rope
*
* output: a copy of that rope
******************************************************************************/
template<typename T, int Size>
Lariat<T, Size>::Lariat(Lariat const& rhs)
{
  LNode* current = rhs.head_;
  while (current)
  {
    // new lariat stuff
    LNode* newnode = addNode();
    for (int i = 0; i < current->count; i++)
    {
      write(newnode, i, current->values[i]);
    }

    current = current->next;
  }
}




/******************************************************************************
* Name  : addNode
*
* brief : add a new node to the rope at this index (basically insert before)
*
* input : nodeIndex - the index where to insert the node
*                     defaults to the tail, provide a valid index to insert
*
* output: pointer to the added node
******************************************************************************/
template<typename T, int Size>
typename Lariat<T, Size>::LNode*  Lariat<T, Size>::addNode(int nodeIndex)
{
  // make a new node
  LNode* newNode = nullptr;
  try
  {
    newNode = new Lariat<T, Size>::LNode();
  }
  catch (const std::exception&)
  {
    cout << "oh no you have no memory";
  }

  // walk the nodes looking for this nod, new node gets the index
  if (nodeIndex > 1 && nodeIndex < nodecount_ - 1)
  {
    LNode* temp = head_;
    while (temp && nodeIndex--)
    {
      temp = head_->next;
    }
    if (temp)
    {
      // connect new node in
      newNode->next = temp;
      newNode->prev = temp->prev;

      //update next node and previous
      (*temp).prev = newNode;
      newNode->prev->next = newNode;
    }
    else
    {
      throw (LariatException::E_BAD_INDEX);
    }
  }

  // push front node
  else if (nodeIndex == 0)
  {
    newNode->next = head_;
    (*head_).prev = newNode;
    head_ = newNode;
  }

  // push back node index == -1
  else
  {
    if (tail_)
    {
      newNode->prev = tail_;
      tail_->next = newNode;
      tail_ = newNode;
    }
    else
    {
      tail_ = newNode;
      head_ = newNode;
    }
  }

  nodecount_++;


<<<<<<< .mine
||||||| .r201
  //debug only
  printNodes();
=======
  //debug only
  
>>>>>>> .r203

  return newNode;
}

/******************************************************************************
* Name  : addNode
*
* brief : add a new node to the rope after this node
*
* input : afterMe - insertion will be after this node
*
* output: pointer to the added node
******************************************************************************/
template<typename T, int Size>
<<<<<<< .mine
typename Lariat<T, Size>::LNode*  Lariat<T, Size>::addNode(LNode* afterMe)
{

||||||| .r201
typename Lariat<T, Size>::LNode*  Lariat<T, Size>::addNode(LNode* afterMe)
{
  printNodes();

=======
typename Lariat<T, Size>::LNode*  Lariat<T, Size>::addNode(LNode* afterMe)
{
  

>>>>>>> .r203
  // make a new node
  LNode* newNode = nullptr;
  try
  {
    newNode = new Lariat<T, Size>::LNode();
  }
  catch (const std::exception&)
  {
    cout << "oh no you have no memory";
  }

  LNode* current = head_;

  // better check for push back
  if (afterMe == tail_)
    current = tail_;

  // walk the nodes looking for this nod, new node gets the index
  while (current && current != afterMe)
  {
    current = current->next;
  }
  if (current)
  {
    // connect new node in
    newNode->next = current->next;
    newNode->prev = current;

    //update next node and previous
    (*current).next= newNode;
    if(newNode->next)
      newNode->next->prev = newNode;
  }
  else
  {
    throw (LariatException::E_BAD_INDEX);
  }

  //update tail
  while (current)
  {
    tail_ = current;
    current = current->next;
  }

  nodecount_++;

<<<<<<< .mine
||||||| .r201
  //debug only
  printNodes();
=======
  //debug only
  
>>>>>>> .r203

<<<<<<< .mine

  return newNode;
}
||||||| .r201
  return newNode;
}
=======
  return newNode;
}
>>>>>>> .r203


/******************************************************************************
* Name  : balance
*
* brief : redistribute values for an even rope
*
* input : 
*         
* output: the amount of numbers moved
******************************************************************************/
template<typename T, int Size>
int Lariat<T, Size>::balance(LNode* unbalanced)
{
<<<<<<< .mine
||||||| .r201
  printNodes();
=======
  
>>>>>>> .r203
  int move = unbalanced->count / 2;
  int offset = (unbalanced->count) % 2;

  // this should never happen but if it does...
  if (!unbalanced->next)
  {
    addNode(unbalanced);
  }

  // move the fewest nodes into the new node
  LNode* newNode = unbalanced->next;
  for (int i = move, newI = 0; newI < move; i++, newI++)
  {
    write(newNode, newI, unbalanced->values[i + offset]);

    // erase the old values
    // write is going to update size too oops
    size_--;
    unbalanced->count--;
  }

<<<<<<< .mine
||||||| .r201
  printNodes();
=======
  
>>>>>>> .r203
  return move;
}

/******************************************************************************
* brief : destructor, dont forget to destroy the all little nodes
*
* input : 
*
* output:
******************************************************************************/
template<typename T, int Size>
Lariat<T, Size>::~Lariat()
{
  LNode* delMe = tail_;
  while (delMe)
  {
    LNode* next = delMe->prev;

    delete delMe;

    delMe = next;
    if (delMe)
    {
      delMe->next = nullptr;
<<<<<<< .mine
||||||| .r201
      printNodes();
=======
      
>>>>>>> .r203
    }
  }
}


                      //// Operators ////

template<typename T, int Size>
Lariat<T, Size>& Lariat<T, Size>::operator=(Lariat const& rhs)
{
  LNode* current = rhs.head_;
  LNode* copy = this->head_;
  while (current)
  {
    // copy all the values from the master
    for (int i = 0; i < current->count; i++)
    {
      copy->values[i] = current->values[i];
    }
    copy->count = current->count;
    current = current->next;
    
    //expand if needed
    if (copy->next == nullptr && current != nullptr)
    {
      addNode();
    }

    // update tail as we go 
    this->tail_ = copy;
    copy = copy->next;
  }

  // update stats
  this->size_       = rhs.size_;
  this->tail_->next = nullptr;
  this->nodecount_  = rhs.nodecount_;
  
  //there are extra nodes from the coppied to blank
  while (copy)
  {
    current = copy;
    copy = copy->next;
    delete current;
  }

  return *this;
}

/******************************************************************************
* brief :
*
* input :
*
* output:
******************************************************************************/
template <typename T, int Size>
std::ostream& operator<<(std::ostream &os, Lariat<T, Size> const & list)
{
  typename Lariat<T, Size>::LNode * current = list.head_;
  int index = 0;
  while (current) {
    os << "Node starting (count " << current->count << ")\n";
    for (int local_index = 0; local_index < current->count; ++local_index) {
      os << index << " -> " << current->values[local_index] << std::endl;
      ++index;
    }
    os << "-----------\n";
    current = current->next;
  }
  return os;
}


/******************************************************************************
* brief : return this index
*
* input : index - this index
*
* output: value at that index
******************************************************************************/
template<typename T, int Size>
T&   Lariat<T, Size>::operator[](int index)
{
  LNode* current = head_;
  findIndex(&index, &current);

  if (current)
  {
    return current->values[index];
  }
  else
  {
    throw(LariatException(LariatException::E_BAD_INDEX, "Subscript is out of range"));
  }
}


                    //// Node Manipulation ////


/******************************************************************************
* brief : shifts the values of a node to the right by 1
*
* input : node - shift this node's values by 1
*
* output: true if shift performed
******************************************************************************/
template<typename T, int Size>
bool Lariat<T, Size>::shift(LNode* node, int startIndex)
{
  if (_DEBUG_)
  {
    cout << "shifting from " << startIndex << "\n{ ";
    for (int i = 0; i < asize_; i++)
    {
      // denote the first shift
      if (startIndex == i)
        cout << "*";
      cout << node->values[i] << " ";



      // end the usable section
      if (node->count - 1 == i)
        cout << "} ";
    }
    cout << "| " << node->values[asize_] << " |\n";
  }

  if (startIndex >= asize_)
  {
    if (_DEBUG_) cout << "NO SHIFT PERFORMED\n";
    return false;
  }

  // shift nodes
  for (int i = node->count; i > startIndex; i--)
  {
    node->values[i] = node->values[i - 1];
  }

  if (_DEBUG_)
  {
    cout << "shifting to \n{ ";
    for (int i = 0; i < asize_; i++)
    {
      // denote the first shift
      if (startIndex == i)
        cout << "*";

      cout << node->values[i] << " ";
      if (node->count - 1 == i)
        cout << "} ";
    }
    cout << "| " << node->values[asize_] << " |\n";
  }

  return true;
}

template<typename T, int Size>
bool Lariat<T, Size>::findIndex(int* index, LNode** node)
{
  LNode* current = (*node);
  int min = 0;          // valid indices for this node

  // find the smallest index of this node
  while (current->next)
  {
    min += current->count;
    if (min > *index)
    {
      min -= current->count;
      break;
    }
    current = current->next;
  }
  *index -= min;
  *node = current;

  //is this index valid?
  if (*index > asize_ || current->count == asize_)
  {
    //*index = max + 1;
    // needs expansion
    return true;
  }
  else
  {
     //this index is valid for this node somewhere

    // no expand
    return false;
  }
}

/******************************************************************************
* brief :
*
* input :
*        
*
* output:
******************************************************************************/
template<typename T, int Size>
void Lariat<T, Size>::insertMiddle(int index, LNode* current, const T& value)
{
  shift(current, index);
  write(current, index, value);
}

template<typename T, int Size>
void Lariat<T, Size>::write(LNode * current, int index, const T & value)
{
  current->values[index] = value;
  current->count++;
  size_++;
}


/******************************************************************************
* brief : insert a value at this index
*
* input : index - this index
*         value - this value
*        
* output:
******************************************************************************/
template<typename T, int Size>
void Lariat<T, Size>::insert(int index, const T& value)
{
<<<<<<< .mine
||||||| .r201
  printNodes();
=======
  if (_DEBUG_) cout << "inserting " << value << " at " << index << "\n";
  
  if (!head_)
    addNode();
>>>>>>> .r203

  LNode* current = head_;
  bool expand = findIndex(&index, &current);
  if (index > current->count)
    throw (LariatException(LariatException::E_BAD_INDEX, "Subscript is out of range"));

  bool unbalanced = false;
  if (_DEBUG_) cout << "inserting " << value << " at local " << index << "\n";

  // exand if needed
  if (expand)
  {
    if(_DEBUG_) cout << "expanding\n";
    addNode(current);
    unbalanced = true;
  }

  // not a push back - some shifting will be needed
  if (index != current->count)
  {
    if (_DEBUG_) cout << "middle\n";
    insertMiddle(index, current, value);
  }
  else
  {
    write(current, index, value);
  }

  if (unbalanced)
    balance(current);

<<<<<<< .mine
||||||| .r201
  printNodes();
=======
  
>>>>>>> .r203

}

/******************************************************************************
* brief : push a value to the back
*
* input : value - this value
*
* output:
******************************************************************************/
template<typename T, int Size>
void Lariat<T, Size>::push_back(const T& value)
{
  write(tail_, tail_->count, value);
  if (tail_->count > asize_)
  {
    LNode* unbalanced = tail_;
    addNode(unbalanced);
    balance(unbalanced);
  }
}

/******************************************************************************
* brief : push a value to the front of the rope (not to the front of a node)
*
* input : value - this value
*
* output:
******************************************************************************/
template<typename T, int Size>
void Lariat<T, Size>::push_front(const T& value)
{
  shift(head_);
  write(head_, 0, value);
  if (head_->count > asize_)
  {
    addNode(head_);
    balance(head_);
  }
}

/******************************************************************************
* brief : remove this value
*
* input : index - at this index
*
* output:
******************************************************************************/
template<typename T, int Size>
void Lariat<T, Size>::erase(int index)
{
  LNode* current = head_;

  // there's nothing to erase - shifting will be problematic
  if (current->count == 0)
    return;
  findIndex(&index, &current);

  for (int i = index; i < current->count - 1; i++)
  {
    current->values[i] = current->values[i + 1];
  }

  current->count--;
  size_--;

}

/******************************************************************************
* brief : remove the last value
*
* input :
*
* output:
******************************************************************************/
template<typename T, int Size>
void Lariat<T, Size>::pop_back()
{
  // erase the last value
  if (tail_->count)
  {
    tail_->count--;
    size_--;
  }

  // clean up empty nodes
  if (tail_->count == 0 && tail_ != head_)
  {
    LNode* delMe = tail_;
    tail_ = tail_->prev;
    
    //update head if deleted
    if (delMe == head_)
      head_ = nullptr;

    delete delMe;

    if(tail_)
      tail_->next = nullptr;
  }

}

/******************************************************************************
* brief : removes the first value 
*
* input :
*
* output:
******************************************************************************/
template<typename T, int Size>
void Lariat<T, Size>::pop_front()
{
  if (head_)
  {
    erase(0);
  }
}



/******************************************************************************
* brief : for l values
*
* input :
*
* output:
******************************************************************************/
template<typename T, int Size>
const T& Lariat<T, Size>::operator[](int index) const
{
  LNode* current = head_;
  int min = 0;          // valid indices for this node

  // find the smallest index of this node
  while (current->next)
  {
    min += current->count;
    if (min > index)
    {
      min -= current->count;
      break;
    }
    current = current->next;
  }
  index -= min;

  //is this index valid?
  if (index < current->count && index >= 0)
  {
    return current->values[index];
  }
  else
    return this->size_;
}

/******************************************************************************
* brief : for r values
*
* input :
*
* output:
******************************************************************************/
template<typename T, int Size>
T&   Lariat<T, Size>::first()
{
  if (head_)
    return head_->values[0];
  else
    return nullptr;
}

/******************************************************************************
* brief : return the first value
*
* input :
*
* output: the first value
******************************************************************************/
template<typename T, int Size>
T const& Lariat<T, Size>::first() const
{
  if (head_)
    return head_->values[0];
  else
    return nullptr;
}

/******************************************************************************
* brief : return the last value
*
* input :
*
* output: the last value
******************************************************************************/
template<typename T, int Size>
T&   Lariat<T, Size>::last()
{
    return tail_->values[tail_->count - 1];
}

/******************************************************************************
* brief : return the last value
*
* input :
*
* output: the last value
******************************************************************************/
template<typename T, int Size>
T const& Lariat<T, Size>::last() const
{
    return tail_->values[tail_->count - 1];
}

/******************************************************************************
* brief : find this value
*
* input : value - find me
*
* output: the index of the first value found or the size of rope if not
******************************************************************************/
template<typename T, int Size>
unsigned Lariat<T, Size>::find(const T& value) const
{
  LNode* current = head_;
  unsigned gIndex = 0;
  while (current)
  {
    for (int i = 0; i < current->count; i++)
    {
      if (current->values[i] == value)
        return gIndex;
      gIndex++;
    }
    current = current->next;
  }

  //couldn't find your value
  return size_;
}

/******************************************************************************
* brief : returns index or size+1 if not found
*
* input :
*
* output: index or size+1 if not found
******************************************************************************/
template<typename T, int Size>
size_t   Lariat<T, Size>::size(void) const
{
  return size_;
}

/******************************************************************************
* brief : empty this rope - but dont free nodes
*
* input :
*
* output:
******************************************************************************/
template<typename T, int Size>
void Lariat<T, Size>::clear(void)
{
  // do we really need to fee these nodes?
  LNode* current = head_;
  while (current)
  {
    current->count = 0;
    current = current->next;
  }
  size_ = 0;
}

/******************************************************************************
* brief : push all data to the front as much as possible
*
* input :
*
* output:
******************************************************************************/
template<typename T, int Size>
void Lariat<T, Size>::compact()
{
  // bail if some fool gives you an empty list to compact
  if (!head_)
    return;

  bool done = false;
  int backupSum = 0;

  while (!done)
  {
    int sum = backupSum;
    LNode* front = head_;
    LNode* back = head_->next;
    done = true;

    while (back)
    {
      // copy back values forward
      for (int i = front->count; i < asize_ && back->count > 0; i++)
      {
        write(front, i, back->values[0]);
        erase(sum + front->count);
        done = false;
      }

      // keep track of global index
      sum += front->count;
      front = back;
      back = back->next;
    }
    
    // delete empty nodes
    LNode* current = tail_;
    while (current->count == 0 && tail_ != head_)
    {
      LNode* prev = current->prev;
      delete current;
      current = prev;
    }
    current->next = nullptr;
    tail_ = current;
  }
}

<<<<<<< .mine
||||||| .r201

                ////// Debug //////


template<typename T, int Size>
void Lariat<T, Size>::printNodes()
{

#if _DEBUG_
  typename Lariat<T, Size>::LNode* current = head_;
  cout << "=========begin============\n";
  cout << "nodecount: " << nodecount_ << " containing data: " << size_ << "\n";
  int spaces = 0;
  int count = 0;
  while (current)
  {
    cout << "[" << count++ << "] " << current->prev << " " << current << " " << current->next << "{" << current->count << "}" << "\n";
    spaces += 9;
    printf("%*c", spaces, ' ');
    current = current->next;
  }
  cout << "\n head node: " << head_ << "tail : " << tail_ << "\n";
  cout << *this;
  cout << "\n========end=========\n";

#endif

  if (head_->prev)
    throw(std::exception("none before the head"));

}
=======

                ////// Debug //////

>>>>>>> .r203
