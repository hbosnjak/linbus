// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef LIN_DECODER_H
#define LIN_DECODER_H

#include <arduino.h>
#include "avr_util.h"

// Uses 
// * Timer2 - used to generate the bit ticks.
// * OC2B (PD3) - timer output ticks. For debugging. If needed, can be changed
//   to not using this pin.
// * PD2 - LIN RX input.
// * PC0, PC1, PC2, PC3 - debugging outputs. See .cpp file for details.
namespace lin_decoder {
  // A buffer for a single recieved frame.
  typedef struct RxFrameBuffer {
    // ID byte with no slave response. 0x55 sync byte is not included.
    static const uint8 kMinBytes = 1;

    // ID byte + 8 data bytes + checksum byte.
    static const uint8 kMaxBytes = 10;

    // Number of valid bytes in the bytes[] array.
    uint8 num_bytes;

    // Recieved frame bytes. Includes id, data and checksum. Does not 
    // include the 0x55 sync byte.
    uint8 bytes[kMaxBytes];
  } 
  RxFrameBuffer;

  // Call once in program setup. Supported baud range is 1000 to 20000. If
  // out of range, using silently default baud of 9600.
  extern void setup(uint16 baud);

  // Try to read next available rx frame. If available, return true and set
  // given buffer. Otherwise, return false and leave *buffer unmodified. 
  // The sync, id and checksum bytes of the frame as well as the total byte
  // count are not verified. 
  extern boolean readNextFrame(RxFrameBuffer* buffer);

  // Errors byte masks for the individual error bits.
  static const uint8 ERROR_FRAME_TOO_SHORT = (1 << 0);
  static const uint8 ERROR_FRAME_TOO_LONG = (1 << 1);
  static const uint8 ERROR_START_BIT = (1 << 2);
  static const uint8 ERROR_STOP_BIT = (1 << 3);
  static const uint8 ERROR_SYNC_BYTE = (1 << 4);
  static const uint8 ERROR_BUFFER_OVERRUN = (1 << 5);
  static const uint8 ERROR_OTHER = (1 << 6);

  // Get current error flag and clear it. 
  extern uint8 getAndClearErrorFlag();
}

#endif  


