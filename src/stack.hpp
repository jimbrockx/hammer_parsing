

#pragma once

#ifndef STACK_HPP
#define STACK_HPP

#include <stdint.h>
//#include <stdio.h>
//#include <stdlib.h>

#define GLOBAL_STACK_DEPTH_M  1
#define GLOBAL_STACK_DEPTH_K  1024*GLOBAL_STACK_DEPTH_M
#define GLOBAL_STACK_DEPTH    1024*GLOBAL_STACK_DEPTH_M // K

#define GLOBAL_STACK_INVALID  (GLOBAL_STACK_DEPTH+1)

#define STACK_INVALID_DATA    0xFF

typedef struct __attribute__((__packed__)) stack_datum_s
{
  uint64_t tokens;
  uint8_t  data;
} stack_datum_t;

static uint16_t stack_ptr = 0x0;

static stack_datum_t global_stack[GLOBAL_STACK_DEPTH];

#define STACK_PUSH( flags, val ) \
  stack_ptr++; \
  global_stack[stack_ptr] = (stack_datum_t){flags, val};

#define STACK_POP( count ) \
  stack_ptr -= count;

#define STACK_RESET() stack_ptr = 0x0;


#define GET_STACK_PTR()        (stack_ptr)

#define GET_STACK_TOP()        (global_stack[stack_ptr])

#define GET_STACK_VALUE(off)   (global_stack[stack_ptr-off])


#define SET_TOKEN(tok)         (global_stack[stack_ptr].tokens |= tok)

#define CHECK_TOKEN(tok)       (global_stack[stack_ptr].tokens & tok)


#define STACK_TRANSFORM(len, tok)  \
  stack_ptr -= (len-1); \
  global_stack[stack_ptr].tokens = tok; \
  global_stack[stack_ptr].data   = STACK_INVALID_DATA; \

#endif
