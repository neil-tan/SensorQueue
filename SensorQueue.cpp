//#include "SensorQueue.hpp"

// template <class T>
// SensorQueue<T>::SensorQueue(uint32_t length, uint16_t _blk_length, uint16_t _max_pool_blks) {

//   blk_length = _blk_length;
//   max_pool_blks = _max_pool_blks;
  
//   total_blks = length / blk_length;
//   if(length > (total_blks * blk_length)) total_blks += 1;
//   blk_offset = 0;
//   buffered_blocks = 0;

//   //Init Blocks
//   for(int i = 0; i < total_blks; i++) {
//     newBlock();
//   }

//   callback_func = NULL;  
// }

// template <class T>
// T* SensorQueue<T>::newBlock(void) {
//   blk_offset = 0;
//   T* new_blk = malloc(sizeof(T) * blk_length);
//   block_list.push_back(new_blk);

//   return new_blk;
// }

// template <class T>
// void SensorQueue<T>::setCallBack(void (*_fun_ptr)) {
//   callback_func = _fun_ptr;
// }

// template <class T>
// void SensorQueue<T>::append_helper(T elem) {
//   if(blk_offset >= blk_length) {
//     if(buffered_blocks < max_pool_blks) {
//       //(*callback_func)();
//       queue.call(*callback_func);
//       queue.dispatch(0);
//       newBlock();  //new block and resets blk_offset
//     } else {
//       printf("error: max_pool_blks exceeded\r\n");
//       exit(-1);
//     }
//   }

//   T* current_blk = block_list.back();
//   current_blk[blk_offset] = elem;
//   blk_offset += 1;
// }

// template <class T>
// void SensorQueue<T>::append(T elem) {
//   queue.call(&append_helper, elem);
//   queue.dispatch(0); //returns immediately
// }
// template <class T>
// void SensorQueue<T>::copyTo_helper(void *ptr, bool adv_frame) {
//   //TODO:  move the following to a blocking event queue call
//   auto it = block_list.begin();
//   for(int i = 0; i < total_blks; i++) {
//     memcpy((void*)(ptr + i * blk_length * sizeof(T)), (void*) *it, sizeof(T) * blk_length);
//     it++;
//   }

//   if(adv_frame && block_list.size() >= blk_length) {
//     free(block_list.front());
//     block_list.pop_front();
//   }
// }

// template <class T>
// void SensorQueue<T>::copyTo(void *ptr, bool adv_frame) {
//   queue.call(&copyTo_helper, ptr, adv_frame);
//   queue.dispatch();
// }

// void SensorQueue_Test(void) {
//   SensorQueue<int> buff(12, 4);
//   list<int> ref;
//   int result[12];

//   for(int i = 0; i < 12; i++) {
//     ref.push_back(i);
//     buff.append(i);
//   }

//   buff.copyTo(result);
//   if(!compare_test(result, ref)) { exit(-1); }
//   for(int i = 0; i < 4; i++) { ref.pop_front(); }

//   for(int i = 0; i < 8; i++) {
//     ref.push_back(i * -1);
//     buff.append(i * -1);
//   }

//   buff.copyTo(result);
//   if(!compare_test(result, ref)) { exit(-1); }
//   for(int i = 0; i < 4; i++) { ref.pop_front(); }

//   buff.copyTo(result);
//   if(!compare_test(result, ref)) { exit(-1); }
  
// }
