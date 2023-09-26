# LRU-Implementation

LRU stands for Least Recursive Used. It is a cache replacement technique. When new data is encountered or in other words if there is a cache hit, the least used or old data will be replaced by new data.
The idea is demonstrate how LRU algorithm works. In this code, cache is illustrated in the form of queue implemented linked List. Old data is always a head of the linked list and new data is always a tail of linked list.

Whenever there is a cache hit, head node is deleted and new node is inserted at a tail of the linked list. For complete details, refer the following youTube link:
https://www.youtube.com/watch?v=akFRa58Svug&t=398s

All of the corner cases discussed in the video is implemented in my code.
