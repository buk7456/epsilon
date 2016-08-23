#ifndef ION_EVENTS_H
#define ION_EVENTS_H

namespace Ion {
namespace Events {

enum class Event {
  LEFT_PARENTHESIS = '(', // 0x28
  RIGHT_PARENTHESIS = ')', // 0x29
  COMMA = ',',
  PRODUCT = '*', // 0x2a
  PLUS = '+', // 0x2b
  MINUS = '-', // 0x2d
  DOT = '.', // 0x2e
  DIVISION = '/', // 0x2f
  ZERO = '0', // 0x30
  ONE = '1',
  TWO = '2',
  THREE = '3',
  FOUR = '4',
  FIVE = '5',
  SIX = '6',
  SEVEN = '7',
  EIGHT = '8',
  NINE = '9',
  EQUAL = '=', // 0x3d
  UPPER_CASE_A = 'A', // 0x41
  UPPER_CASE_B,
  UPPER_CASE_C,
  UPPER_CASE_D,
  UPPER_CASE_E,
  UPPER_CASE_F,
  UPPER_CASE_G,
  UPPER_CASE_H,
  UPPER_CASE_I,
  UPPER_CASE_J,
  UPPER_CASE_K,
  UPPER_CASE_L,
  UPPER_CASE_M,
  UPPER_CASE_N,
  UPPER_CASE_O,
  UPPER_CASE_P,
  UPPER_CASE_Q,
  UPPER_CASE_R,
  UPPER_CASE_S,
  UPPER_CASE_T,
  UPPER_CASE_U,
  UPPER_CASE_V,
  UPPER_CASE_W,
  UPPER_CASE_X,
  UPPER_CASE_Y,
  UPPER_CASE_Z,
  POWER = '^', // 0x5e
  LOWER_CASE_A = 'a', // 0X61
  LOWER_CASE_B,
  LOWER_CASE_C,
  LOWER_CASE_D,
  LOWER_CASE_E,
  LOWER_CASE_F,
  LOWER_CASE_G,
  LOWER_CASE_H,
  LOWER_CASE_I,
  LOWER_CASE_J,
  LOWER_CASE_K,
  LOWER_CASE_L,
  LOWER_CASE_M,
  LOWER_CASE_N,
  LOWER_CASE_O,
  LOWER_CASE_P,
  LOWER_CASE_Q,
  LOWER_CASE_R,
  LOWER_CASE_S,
  LOWER_CASE_T,
  LOWER_CASE_U,
  LOWER_CASE_V,
  LOWER_CASE_W,
  LOWER_CASE_X,
  LOWER_CASE_Y,
  LOWER_CASE_Z,
  LEFT_ARROW = 0x100, // events for things outside of ASCII.
  RIGHT_ARROW,
  UP_ARROW,
  DOWN_ARROW,
  TRIG_MENU,
  DELETE,
  PLOT,
  F1,
  F2,
  F3,
  F4,
  F5,
  SECOND,
  SHIFT,
  ESC,
  ENTER,
  DIAMOND,
  ALPHA,
  APPS,
  HOME,
  MODE,
  CATALOG,
  CLEAR,
  //ERROR = 0xffffffff,
};

Event getEvent();

}
}

#endif
