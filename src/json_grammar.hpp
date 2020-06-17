
#include "grammar.hpp"

// Simple rules
#define GRAMMAR_RULE_WS               (0x1ULL << 0)
#define GRAMMAR_RULE_LEFTSQUARE       (0x1ULL << 1)
#define GRAMMAR_RULE_LEFTSQUARE_SEQ   (0x1ULL << 2)
#define GRAMMAR_RULE_RIGHTSQUARE      (0x1ULL << 3)
#define GRAMMAR_RULE_RIGHTSQUARE_SEQ  (0x1ULL << 4)
#define GRAMMAR_RULE_LEFTCURLY        (0x1ULL << 5)
#define GRAMMAR_RULE_LEFTCURLY_SEQ    (0x1ULL << 6)
#define GRAMMAR_RULE_RIGHTCURLY       (0x1ULL << 7)
#define GRAMMAR_RULE_RIGHTCURLY_SEQ   (0x1ULL << 8)
#define GRAMMAR_RULE_COLON            (0x1ULL << 9)
#define GRAMMAR_RULE_COLON_SEQ        (0x1ULL << 10)
#define GRAMMAR_RULE_COMMA            (0x1ULL << 11)
#define GRAMMAR_RULE_COMMA_SEQ        (0x1ULL << 12)
#define GRAMMAR_RULE_LIT_TRUE         (0x1ULL << 13)
#define GRAMMAR_RULE_LIT_FALSE        (0x1ULL << 14)
#define GRAMMAR_RULE_LIT_NULL         (0x1ULL << 15)
#define GRAMMAR_RULE_MINUS            (0x1ULL << 16)
#define GRAMMAR_RULE_DOT              (0x1ULL << 17)
#define GRAMMAR_RULE_DIGIT            (0x1ULL << 18)
#define GRAMMAR_RULE_NON_ZERO_DIGIT   (0x1ULL << 19)
#define GRAMMAR_RULE_ZERO             (0x1ULL << 20)
#define GRAMMAR_RULE_LOWERE           (0x1ULL << 21)
#define GRAMMAR_RULE_UPPERE           (0x1ULL << 22)
#define GRAMMAR_RULE_EXP              (0x1ULL << 23)
#define GRAMMAR_RULE_PLUS             (0x1ULL << 24)
#define GRAMMAR_RULE_WHOLENUM_SEQ     (0x1ULL << 25)
#define GRAMMAR_RULE_WHOLENUM         (0x1ULL << 26)
#define GRAMMAR_RULE_DECIMAL_SEQ      (0x1ULL << 27)
#define GRAMMAR_RULE_EXP_SIGN         (0x1ULL << 28)
#define GRAMMAR_RULE_EXP_SEQ          (0x1ULL << 29)
#define GRAMMAR_RULE_JSON_NUMBER      (0x1ULL << 30)
#define GRAMMAR_RULE_QUOTE            (0x1ULL << 31)
#define GRAMMAR_RULE_BACKSLASH        (0x1ULL << 32)
#define GRAMMAR_RULE_SLASH            (0x1ULL << 33)
#define GRAMMAR_RULE_BACKSPACE        (0x1ULL << 34)
#define GRAMMAR_RULE_FF               (0x1ULL << 35)
#define GRAMMAR_RULE_LF               (0x1ULL << 36)
#define GRAMMAR_RULE_CR               (0x1ULL << 37)
#define GRAMMAR_RULE_TAB              (0x1ULL << 38)
#define GRAMMAR_RULE_ESC_QUOTE        (0x1ULL << 39)
#define GRAMMAR_RULE_ESC_BACKSLASH    (0x1ULL << 40)
#define GRAMMAR_RULE_ESC_SLASH        (0x1ULL << 41)
#define GRAMMAR_RULE_ESC_BACKSPACE    (0x1ULL << 42)
#define GRAMMAR_RULE_ESC_FF           (0x1ULL << 43)
#define GRAMMAR_RULE_ESC_LF           (0x1ULL << 44)
#define GRAMMAR_RULE_ESC_CR           (0x1ULL << 45)
#define GRAMMAR_RULE_ESC_TAB          (0x1ULL << 46)
#define GRAMMAR_RULE_ESCAPED          (0x1ULL << 47)
#define GRAMMAR_RULE_UNESCAPED        (0x1ULL << 48)
#define GRAMMAR_RULE_JSON_CHAR        (0x1ULL << 49)
#define GRAMMAR_RULE_JSON_STRING      (0x1ULL << 50)

// Complex Rules 
//#define GRAMMAR_RULE_JSON_ARRAY       (0x1 << 32)
//#define GRAMMAR_RULE_JSON_NV_PAIR     (0x1 << 33)
//#define GRAMMAR_RULE_JSON_OBJECT      (0x1 << 34)
//#define GRAMMAR_RULE_JSON_VALUE       (0x1 << 35)
// Total number of rules
#define GRAMMAR_RULE_NUM_RULES        (30)


/** \brief */
static rule_t global_grammar[MAX_GRAMMAR_DEPTH] = 
{
  //0, H_RULE(ws, h_in((uint8_t*)" \r\n\t", 4));
  { 
    .type = RULE_TYPE_CHARSET,
    .value = {GRAMMAR_RULE_WS, 0x0},
    .len = 4,
    .in_or_not = true,
    .data = { ' ', '\r', '\n', '\t' }
  },
  //1, H_RULE(right_square_bracket, h_in((uint8_t*)"]", 1));
  {
    .type = RULE_TYPE_CHARSET,
    .value = {GRAMMAR_RULE_RIGHTSQUARE, 0x0},
    .len = 1,
    .in_or_not = true,
    .data = { ']' }
  },
  //2, H_RULE(left_square_bracket, h_in((uint8_t*)"[", 1));
  {
    .type = RULE_TYPE_CHARSET,
    .value = {GRAMMAR_RULE_LEFTSQUARE, 0x0},
    .len = 1,
    .in_or_not = true,
    .data = { '[' }
  },
  //3, H_RULE(right_square_bracket_seq, h_middle(h_many(ws), h_ch(']'), h_many(ws)));
  { 
    .type = RULE_TYPE_SEQUENCE,
    .value = {GRAMMAR_RULE_RIGHTSQUARE_SEQ, 0x0},
    .len = 3,
    .in_or_not = true,
    .data = {},
    .refs = {
      (rule_ref_t){
        .token = GRAMMAR_RULE_WS,
        .optional = true,
        .many = true,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_RIGHTSQUARE,
        .optional = false,
        .many = false,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_WS,
        .optional = true,
        .many = true,
        .min = 0
      }
    }
  },
  //4, H_RULE(left_square_bracket_seq, h_middle(h_many(ws), h_ch('['), h_many(ws)));
  { 
    .type = RULE_TYPE_SEQUENCE,
    .value = {GRAMMAR_RULE_LEFTSQUARE_SEQ, 0x0},
    .len = 3,
    .in_or_not = true,
    .data = {},
    .refs = {
      (rule_ref_t){
        .token = GRAMMAR_RULE_WS,
        .optional = true,
        .many = true,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_LEFTSQUARE,
        .optional = false,
        .many = false,
        .min = 0

      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_WS,
        .optional = true,
        .many = true,
        .min = 0
      }
    }
  },
  //5, H_RULE(right_curly_bracket, h_in((uint8_t*)"}", 1));
  {
    .type = RULE_TYPE_CHARSET,
    .value = {GRAMMAR_RULE_RIGHTCURLY, 0x0},
    .len = 1,
    .in_or_not = true,
    .data = { '}' }
  },
  //6, H_RULE(left_curly_bracket, h_in((uint8_t*)"{", 1));
  {
    .type = RULE_TYPE_CHARSET,
    .value = {GRAMMAR_RULE_LEFTCURLY, 0x0},
    .len = 1,
    .in_or_not = true,
    .data = { '{' }
  },
  //7, H_RULE(right_curly_bracket_seq, h_middle(h_many(ws), h_ch('}'), h_many(ws)));
  { 
    .type = RULE_TYPE_SEQUENCE,
    .value = {GRAMMAR_RULE_RIGHTCURLY_SEQ, 0x0},
    .len = 3,
    .in_or_not = true,
    .data = {},
    .refs = {
      (rule_ref_t){
        .token = GRAMMAR_RULE_WS,
        .optional = true,
        .many = true,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_RIGHTCURLY,
        .optional = false,
        .many = false,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_WS,
        .optional = true,
        .many = true,
        .min = 0
      }
    }
  },
  //8, H_RULE(left_curly_bracket_seq, h_middle(h_many(ws), h_ch('{'), h_many(ws)));
  { 
    .type = RULE_TYPE_SEQUENCE,
    .value = {GRAMMAR_RULE_LEFTCURLY_SEQ, 0x0},
    .len = 3,
    .in_or_not = true,
    .data = {},
    .refs = {
      (rule_ref_t){
        .token = GRAMMAR_RULE_WS,
        .optional = true,
        .many = true,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_LEFTCURLY,
        .optional = false,
        .many = false,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_WS,
        .optional = true,
        .many = true,
        .min = 0
      }
    }
  },
  //9, H_RULE(colon, h_in((uint8_t*)"=", 1));
  {
    .type = RULE_TYPE_CHARSET,
    .value = {GRAMMAR_RULE_COLON, 0x0},
    .len = 1,
    .in_or_not = true,
    .data = { ':' }
  },
  //10, H_RULE(colon_seq, h_middle(h_many(ws), h_ch('='), h_many(ws)));
  { 
    .type = RULE_TYPE_SEQUENCE,
    .value = {GRAMMAR_RULE_COLON_SEQ, 0x0},
    .len = 3,
    .in_or_not = true,
    .data = {},
    .refs = {
      (rule_ref_t){
        .token = GRAMMAR_RULE_WS,
        .optional = true,
        .many = true,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_COLON,
        .optional = false,
        .many = false,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_WS,
        .optional = true,
        .many = true,
        .min = 0
      }
    }
  },
  //11, H_RULE(comma, h_in((uint8_t*)",", 1));
  {
    .type = RULE_TYPE_CHARSET,
    .value = {GRAMMAR_RULE_COMMA, 0x0},
    .len = 1,
    .in_or_not = true,
    .data = { ',' }
  },
  //12, H_RULE(comma_seq, h_middle(h_many(ws), h_ch(','), h_many(ws)));
  { 
    .type = RULE_TYPE_SEQUENCE,
    .value = {GRAMMAR_RULE_COMMA_SEQ, 0x0},
    .len = 3,
    .in_or_not = true,
    .data = {},
    .refs = {
      (rule_ref_t){
        .token = GRAMMAR_RULE_WS,
        .optional = true,
        .many = true,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_COMMA,
        .optional = false,
        .many = false,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_WS,
        .optional = true,
        .many = true,
        .min = 0
      }
    }
  },
  //13, H_RULE(lit_true, h_literal("true"));
  {
    .type = RULE_TYPE_TOKEN,
    .value = {GRAMMAR_RULE_LIT_TRUE, 0x0},
    .len = 4,
    .in_or_not = true,
    .data = { 't','r','u','e' }
  },
  //14, H_RULE(lit_false, h_literal("false"));
  {
    .type = RULE_TYPE_TOKEN,
    .value = {GRAMMAR_RULE_LIT_FALSE, 0x0},
    .len = 5,
    .in_or_not = true,
    .data = { 'f','a','l','s','e' }
  },
  //15, H_RULE(lit_null, h_literal("null"));
  {
    .type = RULE_TYPE_TOKEN,
    .value = {GRAMMAR_RULE_LIT_NULL, 0x0},
    .len = 4,
    .in_or_not = true,
    .data = { 'n','u','l','l' }
  },
  //16, H_RULE(minus, h_ch('-'));
  {
    .type = RULE_TYPE_CHARSET,
    .value = {GRAMMAR_RULE_MINUS, 0x0},
    .len = 1,
    .in_or_not = true,
    .data = { '-' }
  },
  //17, H_RULE(dot, h_ch('.'));
  {
    .type = RULE_TYPE_CHARSET,
    .value = {GRAMMAR_RULE_DOT, 0x0},
    .len = 1,
    .in_or_not = true,
    .data = { '.' }
  },
  //18, H_RULE(digit, h_ch_range(0x30, 0x39));
  {
    .type = RULE_TYPE_CHARSET,
    .value = {GRAMMAR_RULE_DIGIT, 0x0},
    .len = 10,
    .in_or_not = true,
    .data = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' }
  },
  //19, H_RULE(non_zero_digit, h_ch_range(0x31, 0x39));
  {
    .type = RULE_TYPE_CHARSET,
    .value = {GRAMMAR_RULE_NON_ZERO_DIGIT, 0x0},
    .len = 9,
    .in_or_not = true,
    .data = { '1', '2', '3', '4', '5', '6', '7', '8', '9' }
  },
  //20, H_RULE(zero, h_ch('0'));
  {
    .type = RULE_TYPE_CHARSET,
    .value = {GRAMMAR_RULE_ZERO, 0x0},
    .len = 1,
    .in_or_not = true,
    .data = { '0' }
  },
  //21, H_RULE(exp, h_choice_2(h_ch('E'), h_ch('e')));
  {
    .type = RULE_TYPE_CHARSET,
    .value = {GRAMMAR_RULE_LOWERE, 0x0},
    .len = 1,
    .in_or_not = true,
    .data = { 'e' }
  },
  //22
  {
    .type = RULE_TYPE_CHARSET,
    .value = {GRAMMAR_RULE_UPPERE, 0x0},
    .len = 1,
    .in_or_not = true,
    .data = { 'E' }
  },
  //23
  {
    .type = RULE_TYPE_CHOICE,
    .value = {GRAMMAR_RULE_EXP, 0x0},
    .len = 2,
    .in_or_not = true,
    .data = {},
    .refs = {
      (rule_ref_t){
        .token = GRAMMAR_RULE_LOWERE,
        .optional = false,
        .many = false,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_UPPERE,
        .optional = false,
        .many = false,
        .min = 0
      }
    }
  },
  //24, H_RULE(plus, h_ch('+'));
  {
    .type = RULE_TYPE_CHARSET,
    .value = {GRAMMAR_RULE_PLUS, 0x0},
    .len = 1,
    .in_or_not = true,
    .data = { '+' }
  },
  //25, wholenum seq - h_sequence_1(non_zero_digit, h_many(digit)))
  { 
    .type = RULE_TYPE_SEQUENCE,
    .value = {GRAMMAR_RULE_WHOLENUM_SEQ, 0x0},
    .len = 2, 
    .in_or_not = true,
    .data = {},
    .refs = {
      (rule_ref_t){
        .token = GRAMMAR_RULE_NON_ZERO_DIGIT,
        .optional = false,
        .many = true,
        .min = 1
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_DIGIT,
        .optional = true,
        .many = true,
        .min = 0
      }
    }
  },
  //26, wholenum - h_choice_2(zero, h_sequence_1(non_zero_digit, h_many(digit)))
  {
    .type = RULE_TYPE_CHOICE,
    .value = {GRAMMAR_RULE_WHOLENUM, 0x0},
    .len = 2,
    .in_or_not = true,
    .data = {},
    .refs = {
      (rule_ref_t){
        .token = GRAMMAR_RULE_ZERO,
        .optional = false,
        .many = false,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_WHOLENUM_SEQ,
        .optional = false,
        .many = false,
        .min = 0
      }
    }
  },
  //27, decimal number - h_sequence_1(dot, h_many1(digit))),
  { 
    .type = RULE_TYPE_SEQUENCE,
    .value = {GRAMMAR_RULE_DECIMAL_SEQ, 0x0},
    .len = 2, 
    .in_or_not = true,
    .data = {},
    .refs = {
      (rule_ref_t){
        .token = GRAMMAR_RULE_DOT,
        .optional = false,
        .many = false,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_DIGIT,
        .optional = false,
        .many = true,
        .min = 1
      }
    }
  },
  //28, exponent sign - h_choice_2(plus, minus)
  {
    .type = RULE_TYPE_CHOICE,
    .value = {GRAMMAR_RULE_EXP_SIGN, 0x0},
    .len = 2,
    .in_or_not = true,
    .data = {},
    .refs = {
      (rule_ref_t){
        .token = GRAMMAR_RULE_PLUS,
        .optional = false,
        .many = false,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_MINUS,
        .optional = false,
        .many = false,
        .min = 0
      }
    }
  },
  //29, exponent - h_optional(h_sequence_2(exp, h_optional(h_choice_2(plus, minus)), h_many1(digit)))
  { 
    .type = RULE_TYPE_SEQUENCE,
    .value = {GRAMMAR_RULE_EXP_SEQ, 0x0},
    .len = 3, 
    .in_or_not = true,
    .data = {},
    .refs = {
      (rule_ref_t){
        .token = GRAMMAR_RULE_EXP,
        .optional = false,
        .many = false,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_EXP_SIGN,
        .optional = true,
        .many = false,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_DIGIT,
        .optional = false,
        .many = true,
        .min = 1
      }
    }
  },

  //30, H_RULE(json_number,
  //        h_sequence_3(
  //     sign            h_optional(minus),
  //     whole number    h_choice_2(zero, h_sequence_1(non_zero_digit, h_many(digit))),
  //     decimal number  h_optional(h_sequence_1(dot, h_many1(digit))),
  //     exponent        h_optional(h_sequence_2(exp, h_optional(h_choice_2(plus, minus)), h_many1(digit)))
  //                    )
  //        );
  {
    .type = RULE_TYPE_SEQUENCE,
    .value = {GRAMMAR_RULE_JSON_NUMBER, 0x0},
    .len = 4, 
    .in_or_not = true,
    .data = {},
    .refs = {
      (rule_ref_t){
        .token = GRAMMAR_RULE_MINUS,
        .optional = true,
        .many = false,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_WHOLENUM,
        .optional = false,
        .many = false,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_DECIMAL_SEQ,
        .optional = true,
        .many = false,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_EXP_SEQ,
        .optional = true,
        .many = false,
        .min = 0
      }
    }
  },
  //31, H_RULE(quote, h_ch('"'));
  {
    .type = RULE_TYPE_CHARSET,
    .value = {GRAMMAR_RULE_QUOTE, 0x0},
    .len = 1,
    .in_or_not = true,
    .data = { '\"' }
  },
  //32, H_RULE(backslash, h_ch('\\'));
  {
    .type = RULE_TYPE_CHARSET,
    .value = {GRAMMAR_RULE_BACKSLASH, 0x0},
    .len = 1,
    .in_or_not = true,
    .data = { '\\' }
  },
  //33, H_RULE(slash, h_ch('/'));
  {
    .type = RULE_TYPE_CHARSET,
    .value = {GRAMMAR_RULE_SLASH, 0x0},
    .len = 1,
    .in_or_not = true,
    .data = { '/' }
  },
  //34, H_RULE(backspace, h_ch('b'));
  {
    .type = RULE_TYPE_CHARSET,
    .value = {GRAMMAR_RULE_BACKSPACE, 0x0},
    .len = 1,
    .in_or_not = true,
    .data = { 'b' }
  },
  //35, H_RULE(backspace, h_ch('f'));
  {
    .type = RULE_TYPE_CHARSET,
    .value = {GRAMMAR_RULE_FF, 0x0},
    .len = 1,
    .in_or_not = true,
    .data = { 'f' }
  },
  //36, H_RULE(backspace, h_ch('n'));
  {
    .type = RULE_TYPE_CHARSET,
    .value = {GRAMMAR_RULE_LF, 0x0},
    .len = 1,
    .in_or_not = true,
    .data = { 'n' }
  },
  //37, H_RULE(backspace, h_ch('r'));
  {
    .type = RULE_TYPE_CHARSET,
    .value = {GRAMMAR_RULE_CR, 0x0},
    .len = 1,
    .in_or_not = true,
    .data = { 'r' }
  },
  //38, H_RULE(backspace, h_ch(' t'));
  {
    .type = RULE_TYPE_CHARSET,
    .value = {GRAMMAR_RULE_TAB, 0x0},
    .len = 1,
    .in_or_not = true,
    .data = { 't' }
  },
  //39, H_RULE(esc_quote, h_sequence_1(backslash, quote)); //, NULL));
  {
    .type = RULE_TYPE_SEQUENCE,
    .value = {GRAMMAR_RULE_ESC_QUOTE, 0x0},
    .len = 2,
    .in_or_not = true,
    .data = {},
    .refs = {
      (rule_ref_t){
        .token = GRAMMAR_RULE_BACKSLASH,
        .optional = false,
        .many = false,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_QUOTE,
        .optional = false,
        .many = false,
        .min = 0
      }
    }
  },
  //40, H_RULE(esc_backslash, h_sequence_1(backslash, backslash)); //, NULL));
  {
    .type = RULE_TYPE_SEQUENCE,
    .value = {GRAMMAR_RULE_ESC_BACKSLASH, 0x0},
    .len = 2,
    .in_or_not = true,
    .data = {},
    .refs = {
      (rule_ref_t){
        .token = GRAMMAR_RULE_BACKSLASH,
        .optional = false,
        .many = false,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_BACKSLASH,
        .optional = false,
        .many = false,
        .min = 0
      }
    }
  },
  //41, H_RULE(esc_slash, h_sequence_1(backslash, h_ch('/'))); //, NULL));
  {
    .type = RULE_TYPE_SEQUENCE,
    .value = {GRAMMAR_RULE_ESC_SLASH, 0x0},
    .len = 2,
    .in_or_not = true,
    .data = {},
    .refs = {
      (rule_ref_t){
        .token = GRAMMAR_RULE_BACKSLASH,
        .optional = false,
        .many = false,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_SLASH,
        .optional = false,
        .many = false,
        .min = 0
      }
    }
  },
  //42, H_RULE(esc_backspace, h_sequence_1(backslash, h_ch('b'))); //, NULL));
  {
    .type = RULE_TYPE_SEQUENCE,
    .value = {GRAMMAR_RULE_ESC_BACKSPACE, 0x0},
    .len = 2,
    .in_or_not = true,
    .data = {},
    .refs = {
      (rule_ref_t){
        .token = GRAMMAR_RULE_BACKSLASH,
        .optional = false,
        .many = false,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_BACKSPACE,
        .optional = false,
        .many = false,
        .min = 0
      }
    }
  },
  //43, H_RULE(esc_ff, h_sequence_1(backslash, h_ch('f'))); //, NULL));
  {
    .type = RULE_TYPE_SEQUENCE,
    .value = {GRAMMAR_RULE_ESC_FF, 0x0},
    .len = 2,
    .in_or_not = true,
    .data = {},
    .refs = {
      (rule_ref_t){
        .token = GRAMMAR_RULE_BACKSLASH,
        .optional = false,
        .many = false,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_FF,
        .optional = false,
        .many = false,
        .min = 0
      }
    }
  },
  //44, H_RULE(esc_lf, h_sequence_1(backslash, h_ch('n'))); //, NULL));
  {
    .type = RULE_TYPE_SEQUENCE,
    .value = {GRAMMAR_RULE_ESC_LF, 0x0},
    .len = 2,
    .in_or_not = true,
    .data = {},
    .refs = {
      (rule_ref_t){
        .token = GRAMMAR_RULE_BACKSLASH,
        .optional = false,
        .many = false,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_LF,
        .optional = false,
        .many = false,
        .min = 0
      }
    }
  },
  //45, H_RULE(esc_cr, h_sequence_1(backslash, h_ch('r'))); //, NULL));
  {
    .type = RULE_TYPE_SEQUENCE,
    .value = {GRAMMAR_RULE_ESC_CR, 0x0},
    .len = 2,
    .in_or_not = true,
    .data = {},
    .refs = {
      (rule_ref_t){
        .token = GRAMMAR_RULE_BACKSLASH,
        .optional = false,
        .many = false,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_CR,
        .optional = false,
        .many = false,
        .min = 0
      }
    }
  },
  //46, H_RULE(esc_tab, h_sequence_1(backslash, h_ch('t'))); //, NULL));
  {
    .type = RULE_TYPE_SEQUENCE,
    .value = {GRAMMAR_RULE_ESC_TAB, 0x0},
    .len = 2,
    .in_or_not = true,
    .data = {},
    .refs = {
      (rule_ref_t){
        .token = GRAMMAR_RULE_BACKSLASH,
        .optional = false,
        .many = false,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_TAB,
        .optional = false,
        .many = false,
        .min = 0
      }
    }
  },
  //47, H_RULE(escaped, h_choice_8(esc_quote, esc_backslash, esc_slash, esc_backspace,
  //                           esc_ff, esc_lf, esc_cr, esc_tab)); //, NULL));
  {
    .type = RULE_TYPE_CHOICE,
    .value = {GRAMMAR_RULE_ESCAPED, 0x0},
    .len = 8,
    .in_or_not = true,
    .data = {},
    .refs = {
      (rule_ref_t){
        .token = GRAMMAR_RULE_ESC_QUOTE,
        .optional = false,
        .many = false,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_BACKSLASH,
        .optional = false,
        .many = false,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_SLASH,
        .optional = false,
        .many = false,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_BACKSPACE,
        .optional = false,
        .many = false,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_FF,
        .optional = false,
        .many = false,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_CR,
        .optional = false,
        .many = false,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_LF,
        .optional = false,
        .many = false,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_TAB,
        .optional = false,
        .many = false,
        .min = 0
      }
    }
  },
  //48, H_RULE(unescaped, h_not_in((uint8_t*)"\"\\\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F", 34));
  {
    .type = RULE_TYPE_CHARSET,
    .value = {GRAMMAR_RULE_UNESCAPED, 0x0},
    .len = 34,
    .in_or_not = false,
    .data = { '\"', '\\', '\x00', '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07', '\x08', '\x09', '\x0A', '\x0B', '\x0C', '\x0D', '\x0E', '\x0F', '\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17', '\x18', '\x19', '\x1A', '\x1B', '\x1C', '\x1D', '\x1E', '\x1F' }
  },
  //49, H_RULE(json_char, h_choice_2(escaped, unescaped)); //, NULL));
  {
    .type = RULE_TYPE_CHOICE,
    .value = {GRAMMAR_RULE_JSON_CHAR, 0x0},
    .len = 2,
    .in_or_not = true,
    .data = {},
    .refs = {
      (rule_ref_t){
        .token = GRAMMAR_RULE_ESCAPED,
        .optional = false,
        .many = false,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_UNESCAPED,
        .optional = false,
        .many = false,
        .min = 0
      }
    }
  },
  //50, H_ARULE(json_string, h_middle(quote, h_many(json_char), quote));
  { 
    .type = RULE_TYPE_SEQUENCE,
    .value = {GRAMMAR_RULE_JSON_STRING, 0x0},
    .len = 3, 
    .in_or_not = true,
    .data = {},
    .refs = {
      (rule_ref_t){
        .token = GRAMMAR_RULE_QUOTE,
        .optional = false,
        .many = false,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_JSON_CHAR,
        .optional = true,
        .many = true,
        .min = 0
      },
      (rule_ref_t){
        .token = GRAMMAR_RULE_QUOTE,
        .optional = false,
        .many = false,
        .min = 0
      }
    }
  }

/*
    // Arrays
    H_ARULE(json_array, h_middle(left_square_bracket,
                                 h_sepBy(value, comma),
                                 right_square_bracket));
    // Objects
    H_RULE(name_value_pair, h_sequence_2(json_string, colon, value)); //, NULL));
    H_ARULE(json_object, h_middle(left_curly_bracket,
                                 h_sepBy(name_value_pair, comma),
                                 right_curly_bracket));
    h_bind_indirect(value, h_choice_7(json_object, json_array, json_number, json_string, lit_true, lit_false, lit_null)); //, NULL));
    json = value;
*/

};

