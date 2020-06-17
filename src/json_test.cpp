//#include "hammer.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.hpp"
#include "json_grammar.hpp"


static int json_parser_parse(const uint8_t* str, int32_t strlen, uint64_t flag)
{
  int err = 0x0;
  axi_stream_t          data_in("AXIMASTER_IN");
  rule_t*               grammar_in = &(global_grammar[0]);
  axi_valid_stream_t    valid_out("VALID_OUT");
  axi_bus_t             cur_data;

  for (int32_t idx = 0; idx < strlen; idx++)
  {
     cur_data.keep = 0x1;
     cur_data.data = str[idx];
     cur_data.last = 0x0;
     if (idx == (strlen-1))
     {
       cur_data.last = 1;
     }
     data_in.write(cur_data);
  }

  grammar_parser( data_in, grammar_in, flag, valid_out );

  bool valid = false;
  valid = valid_out.read().data.to_bool();

  if (valid)
  {
    return 0;
  }
  else
  {
    return -1;
  }

}


#define FAIL_ON_FAIL(call) if (call != 0) { return -1; }
#define FAIL_ON_PASS(call) if (call != -1) { return -1; }


/*
static int test_json_object() {
    int retval = json_parser_parse((const uint8_t*)"{}", 2);
    if (retval == 0)
    {
      retval = json_parser_parse((const uint8_t*)"{\"Width\": 800}", 14);
      if (retval == 0)
      {
        uint8_t *file_contents;
        size_t input_file_size;
        FILE *input_file = fopen("object.json", "r");
        fseek(input_file, 0, SEEK_END);
        input_file_size = ftell(input_file);
        rewind(input_file);
        file_contents = (uint8_t*)(malloc(input_file_size * sizeof(uint8_t)));
        fread(file_contents, sizeof(uint8_t), input_file_size, input_file);
        fclose(input_file);
        retval = json_parser_parse((const uint8_t*)file_contents, input_file_size);
      }
    }
    return retval;
}

static int test_json_array() {
    int retval = json_parser_parse((const uint8_t*)"[]", 2);
    if (retval == 0)
    {
      retval = json_parser_parse((const uint8_t*)"[1,2,3]", 7);
      if (retval == 0)
      {
        uint8_t *file_contents;
        size_t input_file_size;
        FILE *input_file = fopen("array.json", "r");
        fseek(input_file, 0, SEEK_END);
        input_file_size = ftell(input_file);
        rewind(input_file);
        file_contents = (uint8_t*)(malloc(input_file_size * sizeof(uint8_t)));
        fread(file_contents, sizeof(uint8_t), input_file_size, input_file);
        fclose(input_file); 
        retval = json_parser_parse((const uint8_t*)file_contents, input_file_size);
      }
    }
    return retval;
}

static int test_json_fail()
{
    uint8_t *file_contents;
    size_t input_file_size;
    FILE *input_file = fopen("object.json", "r");
    fseek(input_file, 0, SEEK_END);
    input_file_size = ftell(input_file);
    rewind(input_file);
    file_contents = (uint8_t*)(malloc(input_file_size * sizeof(uint8_t)));
    fread(file_contents, sizeof(uint8_t), input_file_size, input_file);
    fclose(input_file);

    file_contents[10] = ',';
    return json_parser_parse((const uint8_t*)file_contents, input_file_size);
}
*/


static int test_json_ws()
{
  int retval = 0;

  retval |= json_parser_parse((const uint8_t*)" ", 1, GRAMMAR_RULE_WS);
  retval |= json_parser_parse((const uint8_t*)"\n", 1, GRAMMAR_RULE_WS);
  retval |= json_parser_parse((const uint8_t*)"\r", 1, GRAMMAR_RULE_WS);
  retval |= json_parser_parse((const uint8_t*)"\t", 1, GRAMMAR_RULE_WS);

  return retval;
}

static int test_json_brackets()
{
  int retval = 0;

  // Left bracket
  retval |= json_parser_parse((const uint8_t*)"[", 1, GRAMMAR_RULE_LEFTSQUARE_SEQ);
  retval |= json_parser_parse((const uint8_t*)" [ ", 3, GRAMMAR_RULE_LEFTSQUARE_SEQ);
  retval |= json_parser_parse((const uint8_t*)"\n[\t", 3, GRAMMAR_RULE_LEFTSQUARE_SEQ);
  retval |= json_parser_parse((const uint8_t*)"\r \n [ \t\t\r\n", 10, GRAMMAR_RULE_LEFTSQUARE_SEQ);
  retval |= json_parser_parse((const uint8_t*)"[\t", 2, GRAMMAR_RULE_LEFTSQUARE_SEQ);
  retval |= json_parser_parse((const uint8_t*)" [", 2, GRAMMAR_RULE_LEFTSQUARE_SEQ);

  // Right bracket
  retval |= json_parser_parse((const uint8_t*)"]", 1, GRAMMAR_RULE_RIGHTSQUARE_SEQ);
  retval |= json_parser_parse((const uint8_t*)" ] ", 3, GRAMMAR_RULE_RIGHTSQUARE_SEQ);
  retval |= json_parser_parse((const uint8_t*)"\n]\t", 3, GRAMMAR_RULE_RIGHTSQUARE_SEQ);
  retval |= json_parser_parse((const uint8_t*)"\r \n ] \t\t\r\n", 10, GRAMMAR_RULE_RIGHTSQUARE_SEQ);
  retval |= json_parser_parse((const uint8_t*)"]\t", 2, GRAMMAR_RULE_RIGHTSQUARE_SEQ);
  retval |= json_parser_parse((const uint8_t*)" ]", 2, GRAMMAR_RULE_RIGHTSQUARE_SEQ);

  // Left curly bracket
  retval |= json_parser_parse((const uint8_t*)"{", 1, GRAMMAR_RULE_LEFTCURLY_SEQ);
  retval |= json_parser_parse((const uint8_t*)" { ", 3, GRAMMAR_RULE_LEFTCURLY_SEQ);
  retval |= json_parser_parse((const uint8_t*)"\n{\t", 3, GRAMMAR_RULE_LEFTCURLY_SEQ);
  retval |= json_parser_parse((const uint8_t*)"\r \n { \t\t\r\n", 10, GRAMMAR_RULE_LEFTCURLY_SEQ);
  retval |= json_parser_parse((const uint8_t*)"{\t", 2, GRAMMAR_RULE_LEFTCURLY_SEQ);
  retval |= json_parser_parse((const uint8_t*)" {", 2, GRAMMAR_RULE_LEFTCURLY_SEQ);

  // Right curly bracket
  retval |= json_parser_parse((const uint8_t*)"}", 1, GRAMMAR_RULE_RIGHTCURLY_SEQ);
  retval |= json_parser_parse((const uint8_t*)" } ", 3, GRAMMAR_RULE_RIGHTCURLY_SEQ);
  retval |= json_parser_parse((const uint8_t*)"\n}\t", 3, GRAMMAR_RULE_RIGHTCURLY_SEQ);
  retval |= json_parser_parse((const uint8_t*)"\r \n } \t\t\r\n", 10, GRAMMAR_RULE_RIGHTCURLY_SEQ);
  retval |= json_parser_parse((const uint8_t*)"}\t", 2, GRAMMAR_RULE_RIGHTCURLY_SEQ);
  retval |= json_parser_parse((const uint8_t*)" }", 2, GRAMMAR_RULE_RIGHTCURLY_SEQ);

  return retval;
}

static int test_json_colon()
{
  int retval = 0;

  // Colon
  retval |= json_parser_parse((const uint8_t*)":", 1, GRAMMAR_RULE_COLON_SEQ);
  retval |= json_parser_parse((const uint8_t*)" : ", 3, GRAMMAR_RULE_COLON_SEQ);
  retval |= json_parser_parse((const uint8_t*)"\r:\n", 3, GRAMMAR_RULE_COLON_SEQ);
  retval |= json_parser_parse((const uint8_t*)"\r \n : \t\t\r\n", 10, GRAMMAR_RULE_COLON_SEQ);
  retval |= json_parser_parse((const uint8_t*)":\t\r\n\t\r\t\r\n\n\n\n\n\t", 14, GRAMMAR_RULE_COLON_SEQ);
  retval |= json_parser_parse((const uint8_t*)"           :", 12, GRAMMAR_RULE_COLON_SEQ);

  return retval;
}

static int test_json_comma()
{
  int retval = 0;

  // Colon
  retval |= json_parser_parse((const uint8_t*)",", 1, GRAMMAR_RULE_COMMA_SEQ);
  retval |= json_parser_parse((const uint8_t*)" , ", 3, GRAMMAR_RULE_COMMA_SEQ);
  retval |= json_parser_parse((const uint8_t*)"\r,\n", 3, GRAMMAR_RULE_COMMA_SEQ);
  retval |= json_parser_parse((const uint8_t*)"\r \n , \t\t\r\n", 10, GRAMMAR_RULE_COMMA_SEQ);
  retval |= json_parser_parse((const uint8_t*)",\t\r\n\t\r\t\r\n\n\n\n\n\t", 14, GRAMMAR_RULE_COMMA_SEQ);
  retval |= json_parser_parse((const uint8_t*)"           ,", 12, GRAMMAR_RULE_COMMA_SEQ);

  return retval;
}

static int test_json_literals()
{
  int retval = 0;

  // Literals
  retval |= json_parser_parse((const uint8_t*)"true", 4, GRAMMAR_RULE_LIT_TRUE);
  retval |= json_parser_parse((const uint8_t*)"false", 5, GRAMMAR_RULE_LIT_FALSE);
  retval |= json_parser_parse((const uint8_t*)"null", 4, GRAMMAR_RULE_LIT_NULL);

  return retval;
}

static int test_json_number() {
  int retval = 0;

  retval |= json_parser_parse((const uint8_t*)"1", 1, GRAMMAR_RULE_JSON_NUMBER);
  retval |= json_parser_parse((const uint8_t*)"1234", 4, GRAMMAR_RULE_JSON_NUMBER);
  retval |= json_parser_parse((const uint8_t*)"-0.1234", 7, GRAMMAR_RULE_JSON_NUMBER);
  retval |= json_parser_parse((const uint8_t*)"98.8764e+2", 10, GRAMMAR_RULE_JSON_NUMBER);
  retval |= json_parser_parse((const uint8_t*)"21.04321e-30", 12, GRAMMAR_RULE_JSON_NUMBER);
  retval |= json_parser_parse((const uint8_t*)"-0.2468e+5", 10, GRAMMAR_RULE_JSON_NUMBER);

  return retval;
}

static int test_json_basic_fails()
{
  int retval = 0;

  FAIL_ON_PASS(json_parser_parse((const uint8_t*)"X", 1, STACK_INVALID_DATA));
  FAIL_ON_PASS(json_parser_parse((const uint8_t*)"1..5", 1, STACK_INVALID_DATA));
  FAIL_ON_PASS(json_parser_parse((const uint8_t*)"trueX", 1, STACK_INVALID_DATA));
  FAIL_ON_PASS(json_parser_parse((const uint8_t*)":null:", 1, STACK_INVALID_DATA));
  FAIL_ON_PASS(json_parser_parse((const uint8_t*)"-.4", 1, STACK_INVALID_DATA));
  FAIL_ON_PASS(json_parser_parse((const uint8_t*)"098", 1, STACK_INVALID_DATA));

  return retval;
}

static int test_json_escapes() 
{
  int retval = 0;

  retval |= json_parser_parse((const uint8_t*)"\\\"", 2, GRAMMAR_RULE_ESC_QUOTE);
  retval |= json_parser_parse((const uint8_t*)"\\\\", 2, GRAMMAR_RULE_ESC_BACKSLASH);
  retval |= json_parser_parse((const uint8_t*)"\\/", 2, GRAMMAR_RULE_ESC_SLASH);
  retval |= json_parser_parse((const uint8_t*)"\\b", 2, GRAMMAR_RULE_ESC_BACKSPACE);
  retval |= json_parser_parse((const uint8_t*)"\\f", 2, GRAMMAR_RULE_ESC_FF);
  retval |= json_parser_parse((const uint8_t*)"\\n", 2, GRAMMAR_RULE_ESC_LF);
  retval |= json_parser_parse((const uint8_t*)"\\r", 2, GRAMMAR_RULE_ESC_CR);
  retval |= json_parser_parse((const uint8_t*)"\\t", 2, GRAMMAR_RULE_ESC_TAB);

  return retval;
}

static int test_json_string() {
  int retval = 0;

  retval |= json_parser_parse((const uint8_t*)"\"\"", 2, GRAMMAR_RULE_JSON_STRING);
  retval |= json_parser_parse((const uint8_t*)"\"String\"", 8, GRAMMAR_RULE_JSON_STRING);
  retval |= json_parser_parse((const uint8_t*)"\"String\\rwith control chars\"", 28, GRAMMAR_RULE_JSON_STRING);
  retval |= json_parser_parse((const uint8_t*)"\"String\\r\\nwith control chars\"", 30, GRAMMAR_RULE_JSON_STRING);
  retval |= json_parser_parse((const uint8_t*)"\"98.8764e+2\"", 12, GRAMMAR_RULE_JSON_STRING);
  retval |= json_parser_parse((const uint8_t*)"\"true\"", 6, GRAMMAR_RULE_JSON_STRING);
  retval |= json_parser_parse((const uint8_t*)"\"null\"", 6, GRAMMAR_RULE_JSON_STRING);
  retval |= json_parser_parse((const uint8_t*)"\" [ \"", 5, GRAMMAR_RULE_JSON_STRING);
  retval |= json_parser_parse((const uint8_t*)"\" } \"", 5, GRAMMAR_RULE_JSON_STRING);

  return retval;
}

static int test_json_string_fails()
{
  int retval = 0;

  FAIL_ON_PASS(json_parser_parse((const uint8_t*)"\"", 1, STACK_INVALID_DATA));

  return retval;
}

int register_json_tests() {
  FAIL_ON_FAIL(test_json_ws());
  FAIL_ON_FAIL(test_json_brackets());
  FAIL_ON_FAIL(test_json_colon());
  FAIL_ON_FAIL(test_json_comma());
  FAIL_ON_FAIL(test_json_literals());
  FAIL_ON_FAIL(test_json_number());

  int retval = test_json_basic_fails();
  
  FAIL_ON_FAIL(test_json_escapes());
  FAIL_ON_FAIL(test_json_string());

  retval |= test_json_string_fails();
  
  //FAIL_ON_FAIL(test_json_object());
  //FAIL_ON_FAIL(test_json_array());
  //FAIL_ON_PASS(test_json_fail());
  return retval;
}

/*
#define STR_LEN(str) sizeof(str)

static const char* json_str0 = "{\"board\" : \"dd1000\",\n\"proj_dir\" : \"/media/psf/repos/jim_dev/diode_core/dd1000/xc7s50ftgb1961\",\n\"log_dir\" : \"/media/psf/repos/jim_dev/microarx_secure_boot/7series\",\n\"key_storage\" : \"bbram\",\n\"prog_type\" : \"flash\",\n\"bitgen\" : \"remote\",\n\"user_acct\" : \"jim.brock@websensing.com\",\n\"user_pw\" : \"UARXtest1\",\n\"user_id\" : \"1\"\n}";
static const char* json_str1 = "   {\n      \"Image\": {\n          \"Width\":  800,\n          \"Height\": 600,\n          \"Title\":  \"View from 15th Floor\",\n          \"Thumbnail\": {\n              \"Url\":    \"http://www.example.com/image/481989943\",\n              \"Height\": 125,\n              \"Width\":  \"100\"\n          },\n          \"IDs\": [116, 943, 234, 38793]\n        }\n   }";
static const char* json_str2 = "[\n      {\n         \"precision\": \"zip\",\n         \"Latitude\":  37.7668,\n         \"Longitude\": -122.3959,\n         \"Address\":   \"\",\n         \"City\":      \"SAN FRANCISCO\",\n         \"State\":     \"CA\",\n         \"Zip\":       \"94107\",\n         \"Country\":   \"US\"\n      },\n      {\n         \"precision\": \"zip\",\n         \"Latitude\":  37.371991,\n         \"Longitude\": -122.026020,\n         \"Address\":   \"\",\n         \"City\":      \"SUNNYVALE\",\n         \"State\":     \"CA\",\n         \"Zip\":       \"94085\",\n         \"Country\":   \"US\"\n      }\n]";

static const char* test = "{\n\"board\" : \"dd1000\"\n}";

HParserTestcase testcases[] = {
  {(unsigned char*)"1",  1, "(null (u0x31 ()) null null)"},
  {(unsigned char*)"true",  4, "<74.72.75.65>"},
  {(unsigned char*)"false",  5, "<66.61.6c.73.65>"},
  {(unsigned char*)"null",  4, "<6e.75.6c.6c>"},
  {(unsigned char*)"1234", 4, "(null (u0x31 (u0x32 u0x33 u0x34)) null null)"},
  {(unsigned char*)"-0.1234", 7, "(u0x2d u0x30 (u0x2e (u0x31 u0x32 u0x33 u0x34)) null)"},
  {(unsigned char*)"98.8764e+2", 10, "(null (u0x39 (u0x38)) (u0x2e (u0x38 u0x37 u0x36 u0x34)) (u0x65 u0x2b (u0x32)))"},

  {(unsigned char*)"{\"board\" : \"dd1000\"}", STR_LEN("{\n\"board\" : \"dd1000\"\n}"), "(null (u0x39 (u0x38)) (u0x2e (u0x38 u0x37 u0x36 u0x34)) (u0x65 u0x2b (u0x32)))"},

  {(unsigned char*)"{\"board\" : \"dd1000\",\n\"proj_dir\" : \"/media/psf/repos/jim_dev/diode_core/dd1000/xc7s50ftgb1961\",\n\"log_dir\" : \"/media/psf/repos/jim_dev/microarx_secure_boot/7series\",\n\"key_storage\" : \"bbram\",\n\"prog_type\" : \"flash\",\n\"bitgen\" : \"remote\",\n\"user_acct\" : \"jim.brock@websensing.com\",\n\"user_pw\" : \"UARXtest1\",\n\"user_id\" : \"1\"\n}", STR_LEN(json_str0), "(null (u0x39 (u0x38)) (u0x2e (u0x38 u0x37 u0x36 u0x34)) (u0x65 u0x2b (u0x32)))"},
  {(unsigned char*)"   {\n      \"Image\": {\n          \"Width\":  800,\n          \"Height\": 600,\n          \"Title\":  \"View from 15th Floor\",\n          \"Thumbnail\": {\n              \"Url\":    \"http://www.example.com/image/481989943\",\n              \"Height\": 125,\n              \"Width\":  \"100\"\n          },\n          \"IDs\": [116, 943, 234, 38793]\n        }\n   }", STR_LEN(json_str1), "(null (u0x39 (u0x38)) (u0x2e (u0x38 u0x37 u0x36 u0x34)) (u0x65 u0x2b (u0x32)))"},
  {(unsigned char*)"[\n      {\n         \"precision\": \"zip\",\n         \"Latitude\":  37.7668,\n         \"Longitude\": -122.3959,\n         \"Address\":   \"\",\n         \"City\":      \"SAN FRANCISCO\",\n         \"State\":     \"CA\",\n         \"Zip\":       \"94107\",\n         \"Country\":   \"US\"\n      },\n      {\n         \"precision\": \"zip\",\n         \"Latitude\":  37.371991,\n         \"Longitude\": -122.026020,\n         \"Address\":   \"\",\n         \"City\":      \"SUNNYVALE\",\n         \"State\":     \"CA\",\n         \"Zip\":       \"94085\",\n         \"Country\":   \"US\"\n      }\n]", STR_LEN(json_str2), "(null (u0x39 (u0x38)) (u0x2e (u0x38 u0x37 u0x36 u0x34)) (u0x65 u0x2b (u0x32)))"},
  { NULL, 0, NULL }
};
*/

int main(int argc, char** argv) {
  int retval = 0;
  //init_parser();
  //printf("TOTAL: %ld\n", sys_heap_ptr);
  retval = register_json_tests();
  //printf("TOTAL: %ld\n", sys_heap_ptr);
  printf("\n%d\n", retval);
  return retval;
}
