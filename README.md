# SensorQueue

## Introduction
  Buffers a sequential input and produces a sliding-window snapshot of the buffered data. This class uses `<list>` and [EventQueue](https://docs.mbed.com/docs/mbed-os-api-ref/en/stable/APIs/tasks/events_lib/) to achieve memory pooling and thread safety.
## APIs

- **SensorQueue**(uint32_t length, uint16_t blk_length, uint16_t max_pool_blks = 1);
  Constructor
  `length`: sliding window's length
  `blk_length`: the over-lapse between the sliding windows
  `max_pool_blks`: how many sliding windows to buffer

- void **append**(T elem)
  appends element `elem` to the buffer

- void **setCallBack**(void (*_fun_ptr))

  A function pointer to call when a new sliding window is ready

- void **copyTo**(void *ptr, bool adv_frame = true);

  copies a window length of data into `ptr`
  `adv_frame = true` will free the data that has been copied and will no longer be used

- void **printStates**(void);

  prints the internal variables for debugging purpose

## Example

The following code snippet creats a sliding window of length 12 and advances 4 elements everytime `copyTo` is called.

```
  //window length of 12, increment of 4 elements, buffers 2 extra windows
  SensorQueue<int> buff(12, 4, 2);
  list<int> ref;
  int* result = (int*) malloc(sizeof(int) * 12);

  for(int i = 0; i < 12; i++) {
    ref.push_back(i);
    buff.append(i);
  }

  buff.copyTo(result);
  //result now has the same content as ref

  ```

  The memory of any over-lapping elements is reused.