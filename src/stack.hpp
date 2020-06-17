

#pragma once

#ifndef STACK_HPP
#define STACK_HPP

#include <stdint.h>

#define GLOBAL_STACK_DEPTH_M  1
#define GLOBAL_STACK_DEPTH_K  1024*GLOBAL_STACK_DEPTH_M
#define GLOBAL_STACK_DEPTH    1024*GLOBAL_STACK_DEPTH_M // K

#define GLOBAL_STACK_INVALID  (GLOBAL_STACK_DEPTH+1)

#define STACK_INVALID_DATA    0x0

typedef struct __attribute__((__packed__)) stack_datum_s
{
  uint64_t tokens;
  uint8_t  data;
} stack_datum_t;

static uint16_t stack_ptr = 0x0;

static stack_datum_t global_stack[GLOBAL_STACK_DEPTH];

static uint16_t update_len = 0;
static uint64_t update_tokens = 0x0;

#define STACK_PUSH( flags, val ) \
  stack_ptr++; \
  global_stack[stack_ptr] = (stack_datum_t){flags, val};

#define STACK_POP( count ) \
  stack_ptr -= count;


#define STACK_UPDATE_CLEAR() \
  update_len = 0x0; \
  update_tokens = 0x0;

#define STACK_RESET() \
  STACK_UPDATE_CLEAR() \
  stack_ptr = 0x0; \


#define GET_STACK_PTR()        (stack_ptr)

#define GET_STACK_TOP()        (global_stack[stack_ptr])

#define GET_STACK_VALUE(off)   (global_stack[stack_ptr-off])

#define CLEAR_STACK_TOP()      global_stack[stack_ptr].data = STACK_INVALID_DATA;

#define SET_TOKEN(tok)         (global_stack[stack_ptr].tokens |= tok)

#define CHECK_TOKEN(tok)       (global_stack[stack_ptr].tokens & tok)

/*
#define STACK_TRANSFORM(len, tok)  \
  if (len > update_len) { \
    update_len = len; \
    update_tokens = tok; \
  } else if (len == update_len) { \
    update_tokens |= tok; \
  }
*/

#define STACK_TRANSFORM(len, tok)  \
  stack_ptr -= (len-1); \
  global_stack[stack_ptr].data = global_stack[stack_ptr+(len-1)].data; \
  global_stack[stack_ptr].tokens |= tok;


/*
  stack_ptr -= (len-1); \
  global_stack[stack_ptr].data = global_stack[stack_ptr+(len-1)].data; \
  for (int16_t stkidx = (stack_ptr+(len-1)); stkidx >= stack_ptr ; stkidx--) { \
    global_stack[stack_ptr].tokens &= global_stack[stack_ptr+stkidx].tokens; \
  } \
  global_stack[stack_ptr].tokens |= tok;
*/

#define STACK_INVALIDATE() \
  global_stack[0x1] = (stack_datum_t){0x0, STACK_INVALID_DATA}; \
  update_len = 0; \
  update_tokens = 0x0;

#define STACK_UPDATE(change_var) 

/*
  \
  if (update_len > 0) { \
    stack_ptr -= (update_len-1); \
    global_stack[stack_ptr].data = global_stack[stack_ptr+(update_len-1)].data; \
    global_stack[stack_ptr].tokens = update_tokens; \
    change_var = true; \
    STACK_UPDATE_CLEAR() \
  }
*/

#endif
