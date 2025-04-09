
#ifndef MINPRIORITYQUEUE_H
#define MINPRIORITYQUEUE_H
// NOTE: You may not include any other libraries!
#include <unordered_map>
#include <vector>
#include <utility> 
using namespace std;
template <typename T>
class MinPriorityQueue
{
 
// For the mandatory running times below:
 
//
 
// n is the number of elements in the MinPriorityQueue.
 
//
 
// Assume that the operations of unordered_map are O(1) time 
 
// (they are average case, but not worst-case).
 
// You don't need any other instance variables,
 
// but you can add some if you want to.
 
vector< pair<T, int> > H; // The heap.
 
unordered_map<T, int> I; // Maps values to their indices in H.
 
public:
 
// Creates an empty MinPriorityQueue
 
MinPriorityQueue()
 
{
 
// TODO
//so what goes here? return 0 if nothing
//delcare variables here but will they be global accessed in size and insert?
 
H.clear();
 
I.clear();
 
}
 
// Returns the number of elements in the MinPriorityQueue.
 
//
 
// Must run in O(1) time.
 
int size()
 
{
 
// TODO 
return H.size();
 
} 
 
// Pushes a new value x with priority p
 
// into the MinPriorityQueue.
 
//
 
// Must run in O(log(n)) time. 
 
void push(T x, int p)
 
{
 
// TODO
//create the pair
//push into vector(H)
 
H.push_back({x, p});
 
//Map to H location in I
 
I[x] = H.size() - 1;
 
// bubble up that item
 
bubbleUp(H.size() - 1);
 
}
 
//helper function for bubbleup
 
void bubbleUp(int index) {
 
/*
 
if parent.second < child.second
 
swap parent, child
 
update index
 
update child as parent
 
bubbleUp() with Child as new Parent
 
*/
 
//basecase: when root is reached
if (index == 0) return;
 
int parent = (index - 1) / 2;
 
if (H[parent].second > H[index].second){
 
swap(H[parent], H[index]);
 
I[H[parent].first] = parent;
 
I[H[index].first] = index;
 
bubbleUp(parent);
 
}
 
}
 
// Returns the value at the front of the MinPriorityQueue.
 
// Undefined behavior if the MinPriorityQueue is empty.
 
// 
 
// Must run in O(1) time.
 
T front()

 
{
 
// TODO 
//pseudocode
//basecase H is empty
if (!H.size()) return T(); // Returning default value of T
 
//return the root value x of H the front
//which library is .first from?
return H[0].first;
 
}
 
// Removes the value at the front of the MinPriorityQueue.
 
// Undefined behavior if the MinPriorityQueue is empty.
 
//
 
// Must run in O(log(n)) time. 
 
void pop()
 
{
 
// TODO 
//basecase empty H
if (!H.size()) return;
 
//swap root 0 and last time H.size() -1
 
swap(H[0], H[H.size() - 1]);
 
//pop_back() that item
 
H.pop_back();
 
// bubbledown() at the root
 
bubbleDown(0);
 
}
 
void bubbleDown(int index){
 
//pseudocode:
//swap the root with the last element in Heap
int leftChild = 2 * index + 1;
 
int rightChild = 2 * index + 2;
 
int smallest = index;
//bubbledown algo: swap parent if children smaller,
//swap with smallest child and repeat till Heap is fixed.
//if current index < than children, check to swap which one
//if Left child < Right swap left with parent, else vice versa.
if (leftChild < H.size() && H[leftChild].second < H[smallest].second) {
 
smallest = leftChild;
 
}
 
 
if (rightChild < H.size() && H[rightChild].second < H[smallest].second) {
 
smallest = rightChild;
 
}
 
if (smallest != index) {
 
swap(H[index], H[smallest]);
 
I[H[index].first] = index;
 
I[H[smallest].first] = smallest;
 
bubbleDown(smallest); 
 
}
 
}
 
// If x is in the MinPriorityQueue 
 
// with current priority at least new_p, 
 
// then changes the priority of x to new_p.
 
// Undefined behavior otherwise.
 
//
 
// Must run in O(log(n)) time. 
 
void decrease_key(T x, int new_p)
 
{
 
// TODO
// H[I[x]].second > new_p then decrease it
// bubbledown() that pair
if (H[I[x]].second > new_p) {
 
H[I[x]].second = new_p; 
 
bubbleUp(I[x]);
 
}
 
}
};
#endif