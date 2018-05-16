#include "mbed.h"
#include "mbed_events.h"
#include <list>
#include <cstdlib>
#include <cstring>

template <class T>
class SensorQueue {
  private:
    uint16_t blk_length;
    uint26_t max_pool_blks;
    uint26_t total_blks;
    uint16_t buffered_blocks;
    uint16_t blk_offset; //The index within an individual block
    EventQueue queue;

    void (*callback_func)(void);
    std::list<*T> block_list;
    T* newBlock(void);
    void append_helper(T elem);
    void copyTo_helper(*void ptr, bool adv_frame = true);

  public:
    SensorQueue(uint32_t length, uint16_t blk_length, uint16_t max_pool_blks = 1);
    void append(T &elem);  //called by interrupt
    void setCallBack(void (*_fun_ptr));  //notify when a segment is full
    void copyTo(*void ptr, bool adv_frame = true);  //fill the ptr with the newest elements
}