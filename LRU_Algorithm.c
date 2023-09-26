/*
    LRU Implementation using linkedlist and Hash Map
    
    Author: Kumar Ananda Chaya (Chaya Kumar Gowda A)
    Date: 06/15/2023
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define INVALID_ADDR  0xFFFFFFFF

//Linkedlist for cache
typedef struct _Cache_Node{
    int val;
    struct _Cache_Node* Next; 
}Cache_Node;

//LinkedList for HashMap
typedef struct _Hash_Node{
    int key;
    uint32_t value;
    struct _Hash_Node* Next;
}Hash_Node;

//use this as hash map
typedef struct _DataItem {
    int Hash_Size;
    Hash_Node** Hash_Table;
}DataItem;

//Initializes the hash Map
DataItem* Init_HasH_Map(int Size){
    DataItem* New_Hash_Map = (DataItem*)malloc(sizeof(DataItem));
    New_Hash_Map->Hash_Size = Size;
    New_Hash_Map->Hash_Table = (Hash_Node**)calloc(Size, sizeof(Hash_Node*));
    return New_Hash_Map;
}

int Hash_Code(int Size, int key){
    return key % Size;
}

void Hash_Insert(DataItem* Hashmap_Handle, int key, uint32_t addr){
    int HashNum = Hash_Code(Hashmap_Handle->Hash_Size, key);
    Hash_Node* Curr_HashNode = Hashmap_Handle->Hash_Table[HashNum];
    
    if(Curr_HashNode != NULL)
        Curr_HashNode->value = addr;
}

Hash_Node* Hash_Search(DataItem* Hashmap_Handle, int key){
    int HashNum = Hash_Code(Hashmap_Handle->Hash_Size, key);
    Hash_Node* Curr_HashNode = Hashmap_Handle->Hash_Table[HashNum];
    
    return Curr_HashNode;
}

//Sets all key's value to invalid address..
void Setup_HashMap(DataItem* Hashmap_Handle, int key){
    
    int HashNum = Hash_Code(Hashmap_Handle->Hash_Size, key);
    
    Hash_Node* New_Node = (Hash_Node*)malloc(sizeof(Hash_Node));
    New_Node->key = key;
    New_Node->value = INVALID_ADDR;
    New_Node->Next = NULL;
    
    Hashmap_Handle->Hash_Table[HashNum] = New_Node;
}

Cache_Node* Insert2Cache(Cache_Node** Front_Ptr, Cache_Node** Rear_Ptr, int num){
    Cache_Node* New_Node = (Cache_Node*)malloc(sizeof(Cache_Node));
    New_Node->val = num;
    
    if(*Front_Ptr == NULL && *Rear_Ptr == NULL){
        *Front_Ptr = New_Node;
        *Rear_Ptr = New_Node;
        New_Node->Next = NULL;
    }else{
        Cache_Node* Curr_Node = *Rear_Ptr;
        Curr_Node->Next = New_Node;
        New_Node->Next = NULL;
        *Rear_Ptr = New_Node;
    }
    
    return New_Node;
}

void Remove_LRU_Data(DataItem* Hashmap_Handle, Cache_Node** Front_Ptr, Cache_Node** Rear_Ptr){
    
    //Remove in cache
    if(*Front_Ptr == NULL || *Rear_Ptr == NULL){
        printf("Invalid data...\n");
        return;
    }
    
    Cache_Node* LRU_Node = *Front_Ptr;
    int LRU_Val = LRU_Node->val;
    *Front_Ptr = LRU_Node->Next;
    free(LRU_Node);
    
    //Remove in HashTable
    int HashNum = Hash_Code(Hashmap_Handle->Hash_Size, LRU_Val);
    Hash_Node* Curr_HashNode = Hashmap_Handle->Hash_Table[HashNum];
    
    if(Curr_HashNode == NULL){
        printf("Invalid Hash...\n");
        return;
    }
    
    free(Curr_HashNode);
}

//Reorder when there is a cache hit. Move the hit node to the rear end
void Rearrange_Cache_data(uint32_t addr, Cache_Node** Front_Ptr, Cache_Node** Rear_Ptr){

    Cache_Node* Hit_Node =  (Cache_Node*)(void*)addr;
    Cache_Node* Last_Node = *Rear_Ptr;
    
    if (Hit_Node == *Front_Ptr){
        *Front_Ptr = Hit_Node->Next;
        Last_Node->Next = Hit_Node;
        Hit_Node->Next = NULL;
        *Rear_Ptr = Hit_Node;
    }else if (Hit_Node == *Rear_Ptr){
        //do nothing..
    }else{
        Cache_Node* Prev_Node = *Front_Ptr;
        while(Prev_Node->Next != Hit_Node){
            Prev_Node = Prev_Node->Next;
        }
        Prev_Node->Next = Hit_Node->Next;
        Hit_Node->Next = NULL;
        Last_Node->Next = Hit_Node;
        *Rear_Ptr = Hit_Node;
    }
    
}

void Print_CacheData(Cache_Node** Head)
{
    Cache_Node* Curr_node = *Head;
    while(Curr_node != NULL)
    {
        printf("%d\t",Curr_node->val);
        printf("addr: %p\t\n", Curr_node);
        Curr_node = Curr_node->Next;
    }
}

void Print_hashData(DataItem* Hashmap_Handle, int num){
    if(Hashmap_Handle != NULL){
        printf("Hashdata addr: %p\n", Hashmap_Handle->Hash_Table[num]->value);
    }
} 

//LRU implementation
void LRU_Alogrithm(DataItem* Hashmap_Handle, int num, int cache_capacity){
    static int cache_count = 0;
    
    static Cache_Node* Front = NULL;
    static Cache_Node* Rear = NULL;
    Cache_Node* Node = NULL;
    Hash_Node* hNode = NULL;
    
    hNode = Hash_Search(Hashmap_Handle, num);
        
    if(hNode == NULL || hNode->value == INVALID_ADDR){
        printf("cache Miss: %d\n",num);
            
        if(cache_count < cache_capacity){
            cache_count++;
        }else{
            Remove_LRU_Data(Hashmap_Handle, &Front, &Rear);
        }
        
        Setup_HashMap(Hashmap_Handle, num);  
        Node = Insert2Cache(&Front, &Rear, num);
        Hash_Insert(Hashmap_Handle, num, (uint32_t)(void*)Node);
        
    }else{
        
        printf("cache hit: %d\n",num);
        hNode = Hash_Search(Hashmap_Handle, num);
        Rearrange_Cache_data(hNode->value, &Front, &Rear);
        
    }

    Print_CacheData(&Front);
    Print_hashData(Hashmap_Handle, num);
    printf("\n");
}

int main() {
    
    int cache_capacity = 4; //initializing  the cache size to 4
    int i;
    
    //Initialize the  hashmap thrice the capacity inorder to avoid collision
    DataItem* Hashmap_Handle = Init_HasH_Map(3*cache_capacity);
    if(Hashmap_Handle == NULL){
        printf("HashMap Init failed\n");
        return 0;
    }
    
    
    int input[] = {3,2,4,3,5,6,7,1,8,1,1};
    int inpt_nums = sizeof(input)/sizeof(input[0]);
    for (i = 0; i < inpt_nums; i++){
        LRU_Alogrithm(Hashmap_Handle, input[i],cache_capacity);
    }
    
    return 0;
}
