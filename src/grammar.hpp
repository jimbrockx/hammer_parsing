

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

/*
typedef struct __attribute__((__packed__)) rule_ref_s
{
  uint64_t    token;
  bool        optional;
  bool        many;
  int8_t      min;
} rule_ref_t;

#define MAX_CHARSET_LEN     8
typedef struct __attribute__((__packed__)) rule_charset_s
{
  int8_t len;
  bool   in_or_not;
  unsigned char charset[MAX_CHARSET_LEN];
} rule_charset_t;

#define MAX_CHOICE_LEN      8
typedef struct __attribute__((__packed__)) rule_choice_s
{
  int8_t len;
  rule_ref_t choices[MAX_CHOICE_LEN];
} rule_choice_t;
*/
/*
typedef struct __attribute__((__packed__)) rule_ignore_s
{
  int8_t len;
  // TODO: What goes here?
} rule_ignore_t;

typedef struct __attribute__((__packed__)) rule_indirect_s
{
  int8_t len;
  // TODO: What goes here?
} rule_indirect_t;

typedef struct __attribute__((__packed__)) rule_many_s
{
  int8_t len;
  rule_type_t sep;
  int8_t count;
  int8_t min;
} rule_many_t;

typedef struct __attribute__((__packed__)) rule_length_s
{
  int8_t len;
  // TODO: What goes here?
} rule_length_t;
*/

/*
#define MAX_SEQUENCE_LEN    8
typedef struct __attribute__((__packed__)) rule_sequence_s
{
  int8_t len;
  rule_ref_t items[MAX_SEQUENCE_LEN];
} rule_sequence_t;

#define MAX_TOKEN_LEN       8
typedef struct __attribute__((__packed__)) rule_token_s
{
  int8_t len;
  unsigned char token[MAX_TOKEN_LEN];
} rule_token_t;


typedef union __attribute__((__packed__)) ruleset_u
{
  rule_charset_t   charset;
  rule_choice_t    choice;
  //rule_ignore_t    ignore;
  //rule_indirect_t  indirect;
  //rule_many_t      many;
  //rule_length_t    length;
  rule_sequence_t  sequence;
  rule_token_t     token;
} ruleset_t;

typedef struct __attribute__((__packed__)) rule_s
{
  rule_type_t   type;
  stack_datum_t value;
  ruleset_t     ruleset;
} rule_t;
*/

#define MAX_RULE_BUFFER_LEN     8

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
