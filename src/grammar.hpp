

#pragma once

#ifndef GRAMMAR_HPP
#define GRAMMAR_HPP

#include "stack.hpp"

#define MAX_GRAMMAR_DEPTH_M  1
#define MAX_GRAMMAR_DEPTH_K  1024*MAX_GRAMMAR_DEPTH_M
#define MAX_GRAMMAR_DEPTH    128 //1024*MAX_GRAMMAR_DEPTH_K

#define GRAMMAR_INVALID      (MAX_GRAMMAR_DEPTH+1)

typedef enum rule_type_e
{
  RULE_TYPE_INVALID = 0,
  RULE_TYPE_CHARSET,
  RULE_TYPE_CHOICE,
  //RULE_TYPE_IGNORE,
  //RULE_TYPE_INDIRECT,
  //RULE_TYPE_MANY,
  //RULE_TYPE_LENGTH,
  //RULE_TYPE_OPTIONAL,
  RULE_TYPE_SEQUENCE,
  RULE_TYPE_TOKEN,
  RULE_TYPE_NUM_TYPES
} rule_type_t;


#define MAX_RULE_BUFFER_LEN     16

typedef struct __attribute__((__packed__)) rule_ref_s
{
  uint64_t token;
  bool     optional;
  bool     many;
  int8_t   min;
} rule_ref_t;

typedef struct __attribute__((__packed__)) rule_s
{
  rule_type_t   type;
  stack_datum_t value;
  int8_t len;
  bool in_or_not;
  unsigned char data[MAX_RULE_BUFFER_LEN];
  rule_ref_t refs[MAX_RULE_BUFFER_LEN];
} rule_t;


#endif
