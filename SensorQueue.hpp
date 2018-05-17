#ifndef _SENSORQUEUE_H_
#define _SENSORQUEUE_H_

#include "mbed.h"
#include "mbed_events.h"
#include <list>
#include <cstdlib>
#include <cstring>

template <class T>
class SensorQueue {
  private:
    uint16_t blk_length;
    uint16_t max_pool_blks;
    uint16_t total_blks;
    uint16_t blk_offset; //The index within an individual block
    EventQueue queue;

    void (*callback_func)(void);
    std::list<T*> block_list;
    T* newBlock(void);
    void append_helper(T elem);
    void copyTo_helper(void* ptr, bool adv_frame = true);

  public:
    SensorQueue(uint32_t length, uint16_t blk_length, uint16_t max_pool_blks = 1);
    void append(T elem);  //called by interrupt
    void setCallBack(void (*_fun_ptr));  //notify when a segment is full
    void copyTo(void *ptr, bool adv_frame = true);  //fill the ptr with the newest elements
    void printStates(void);
};

template <class T>
void SensorQueue<T>::printStates(void) {
  printf("blen: %d, bmax: %d, bttl: %d,\r\n", blk_length, max_pool_blks, total_blks);
  printf("len: %d, nblocks: %d, boffet: %d\r\n", block_list.size() * blk_length, block_list.size(), blk_offset);
}

template <class T>
SensorQueue<T>::SensorQueue(uint32_t length, uint16_t _blk_length, uint16_t _max_pool_blks) {

  blk_length = _blk_length;
  max_pool_blks = _max_pool_blks;
  
  total_blks = length / blk_length;
  if(length > (total_blks * blk_length)) total_blks += 1;
  blk_offset = 0;

  //Init Block
  newBlock();

  callback_func = NULL;  
}

template <class T>
T* SensorQueue<T>::newBlock(void) {
  blk_offset = 0;
  T* new_blk = (int*) malloc(sizeof(T) * blk_length);
  block_list.push_back(new_blk);

  return new_blk;
}

template <class T>
void SensorQueue<T>::setCallBack(void (*_fun_ptr)) {
  callback_func = _fun_ptr;
}

template <class T>
void SensorQueue<T>::append_helper(T elem) {
  //printf("inside append helper\r\n");
  //printf("blocks: %d / %d\r\n", block_list.size(), total_blks);
  if(blk_offset >= blk_length) {
    if(block_list.size() < max_pool_blks + total_blks) {
      //(*callback_func)();
      if(callback_func != NULL) {
        queue.call(*callback_func);
        queue.dispatch(0);
      }
      newBlock();  //new block and resets blk_offset
    } else {
      printf("error: max_pool_blks exceeded\r\n");
      exit(-1);
    }
  }

  T* current_blk = block_list.back();
  current_blk[blk_offset] = elem;
  blk_offset += 1;
}

template <class T>
void SensorQueue<T>::append(T elem) {
  //printf("inside append\r\n");
  queue.call(this, &SensorQueue<T>::append_helper, elem);
  queue.dispatch(0); //returns immediately
}

template <class T>
void SensorQueue<T>::copyTo_helper(void *ptr, bool adv_frame) {
  printf("inside copyTo_helper\r\n");
  printf("blk_length is: %d\r\n", blk_length);
  auto it = block_list.begin();
  for(int i = 0; i < total_blks; i++) {
    //printf("offset is: %d\r\n", i * blk_length);
    memcpy((void*)(ptr + i * blk_length * sizeof(T)), (void*) *it, sizeof(T) * blk_length);
    it++;
  }

  if(adv_frame && block_list.size() > total_blks) {
    printf("freeing memory, total blocks: %d\r\n", block_list.size());
    free(block_list.front());
    block_list.pop_front();
    printf("memory freed, total blocks: %d\r\n", block_list.size());
  }
}

template <class T>
void SensorQueue<T>::copyTo(void *ptr, bool adv_frame) {
  queue.call(this, &SensorQueue<T>::copyTo_helper, ptr, adv_frame);
  queue.dispatch(1000);
  //NT: why doesn't queue.dispatch() work?
}


template <class T>
bool compare_test(T* input, list<T> ref, int n) {
  int i = 0;
  bool result = true;
  auto it = ref.begin();
  for(int i = 0; i < n; i++) {
    if(*it != input[i]) {
      result = false;
      break;
    }
    it++;
  }

  printf("internal compare finished\r\n");

  if(!result) {
    printf("=============================\n");
    it = ref.begin();
    for(int i = 0; i < n; i++) {
      printf("reference: ");
      printf("%d ", *it);
      it++;
    }
    printf("\n");
    for(i = 0; i < n; i++) {
      printf("result   : ");
      printf("%d ", input[i]);
    }
    printf("\n");
  } else {
    printf("ok\r\n");
  }

  return result;
}

void SensorQueue_Test(void) {
  SensorQueue<int> buff(12, 4, 2);
  list<int> ref;
  int* result = (int*) malloc(sizeof(int) * 12);

  printf("Constructing initial test data\r\n");
  for(int i = 0; i < 12; i++) {
    ref.push_back(i);
    buff.append(i);
  }
  buff.printStates();
  //l:12 b:3

  printf("1st copy to:\r\n");
  buff.copyTo(result);
  printf("done copying\r\n");
  if(!compare_test(result, ref, 12)) { printf("test failed"); exit(-1); }
  //for(int i = 0; i < 4; i++) { ref.pop_front(); }
  buff.printStates();
  //l:12 b:3

  printf("Constructing 2nd test data\r\n");
  for(int i = 0; i < 8; i++) {
    ref.push_back(i * -1);
    buff.append(i * -1);
  }
  buff.printStates();
  //l:16 b:4
  printf("2nd copy to:\r\n");
  buff.copyTo(result);
  if(!compare_test(result, ref, 12)) { printf("test failed"); exit(-1); }
  for(int i = 0; i < 4; i++) { ref.pop_front(); }
  buff.printStates();
  //l:12 b:3
  printf("3rd copy to:\r\n");
  buff.copyTo(result);
  if(!compare_test(result, ref, 12)) { printf("test failed"); exit(-1); }
  buff.printStates();
  //l:12 b:3
}
#endif