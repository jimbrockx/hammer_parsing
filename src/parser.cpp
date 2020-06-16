

#include "parser.hpp"


static inline void check_charset(rule_t rule, stack_datum_t datum, bool* changed)
{
  bool found = false;
  //rule_charset_t charset_rule = rule.ruleset.charset;
  for (int8_t idx = 0; idx < rule.len; idx++)
  {
    if (rule.data[idx] == datum.data)
    {
      found = true;
      break;
    }
  }

  if ((rule.in_or_not) && found)
  {
    if (!CHECK_TOKEN(rule.value.tokens))
    {
      STACK_TRANSFORM(0x1, rule.value.tokens);
      (*changed) = true;
    }
  }
  if ((!rule.in_or_not) && (!found))
  {
    STACK_TRANSFORM(0x1, rule.value.tokens);
    (*changed) = true;
  }

}

static inline void check_choice(rule_t rule, stack_datum_t datum, bool* changed)
{
  //rule_choice_t choice_rule = rule.ruleset.choice;
  for (int8_t idx = 0; idx < rule.len; idx++)
  {
    if (CHECK_TOKEN(rule.refs[idx].token))
    {
      STACK_TRANSFORM(0x1, rule.value.tokens);
      (*changed) = true;
      break;
    }
  }

}

/*
static inline void check_ignore(rule_t* rule, stack_datum_t datum, bool* changed)
{
  //int8_t len;
  // TODO: What to do here?
}

static inline void check_indirect(rule_t* rule, stack_datum_t datum, bool* changed)
{
  //int8_t len;
  // TODO: What to do here?
}

static inline void check_length(rule_t* rule, stack_datum_t datum, bool* changed)
{
  //int8_t len;
  // TODO: What to do here?
}


static inline void check_many(rule_t* rule, stack_datum_t datum, bool* changed)
{
  //int8_t len;
  //rule_type_t sep;
  //int8_t count;
  //int8_t min;
}
*/

static inline void check_sequence(rule_t rule, stack_datum_t datum, bool* changed)
{
  //rule_sequence_t sequence_rule = rule.ruleset.sequence;
  bool pass = true;
  int8_t stkoff = 0;
  int8_t streak = 0;
  bool prev_start = true;

  for ( int8_t start = (rule.len - 1); start >= 0; start-- )
  {
    if (prev_start)
    {
      prev_start = rule.refs[start].optional;
      stkoff = 0;
      streak = 0;
      pass = true;

      for ( int8_t seqidx = start; seqidx >= 0; /* N/A */ )
      {
        stack_datum_t cur_datum = GET_STACK_VALUE(stkoff);
        rule_ref_t seq_ref = rule.refs[seqidx];

        // Check if current sequence token matches the current 
        if (!(seq_ref.token & cur_datum.tokens))
        {   
          if (seq_ref.many)
          {
            if (streak >= seq_ref.min)
            {
              streak = 0;
              stkoff--;
            }
            else
            {
              pass = false;
              break;
            }
          }
          else if (!seq_ref.optional)
          {
            pass = false;
            break;
          }
          else
          {
            stkoff--;
          }
          seqidx--;
          stkoff++;
        }
        else
        {
          if (seq_ref.many)
          {
            streak++;
          }
          else
          {
       	    seqidx--;
          }
          stkoff++;
        }  
      } // for seqidx ...
    } // if prev_start
  } // for start ...

  if (pass)
  {
    STACK_TRANSFORM(stkoff, rule.value.tokens);
    (*changed) = true;
  }

}


static inline void check_continuation(rule_t rule, stack_datum_t datum, bool* changed)
{
  stack_datum_t cur_datum = GET_STACK_VALUE(0x0);
  stack_datum_t prev_datum = GET_STACK_VALUE(0x1);
  rule_ref_t seq_ref = rule.refs[rule.len-1];

  if (prev_datum.tokens & rule.value.tokens)
  {
    if ((seq_ref.many) && (seq_ref.token & cur_datum.tokens))
    {
      //STACK_POP( 0x1 );
      STACK_TRANSFORM(0x2, rule.value.tokens);
      (*changed) = true;
    }
  }
}


static inline void check_token(rule_t rule, stack_datum_t datum, bool* changed)
{
  bool found = true;
  int8_t stkoff = 0;

  if (GET_STACK_PTR() >= rule.len)
  {
    for (int8_t tokidx = (rule.len-1); tokidx >= 0; tokidx-- )
    {
      stack_datum_t cur_datum = GET_STACK_VALUE(stkoff);
      if (cur_datum.data != rule.data[tokidx])
      {
        found = false;
      }
      stkoff++;
    }

    if (found)
    {
      STACK_TRANSFORM(rule.len, rule.value.tokens);
      (*changed) = true;
    }
  }

}


static inline void grammar_check( rule_t* grammar_in,
                                  bool* changed )
{
  bool tmp_changed = false;
  (*changed) = false;

  for (uint8_t idx = 0; idx < MAX_GRAMMAR_DEPTH; idx++)
  {
    if (grammar_in[idx].type == RULE_TYPE_INVALID)
    {
      break;
    }
    stack_datum_t datum = GET_STACK_TOP();

    if (!CHECK_TOKEN(grammar_in[idx].value.tokens))
    {
      switch (grammar_in[idx].type)
      {
        case (RULE_TYPE_CHARSET):
          check_charset(grammar_in[idx], datum, changed);
          break;
        case (RULE_TYPE_CHOICE):
          check_choice(grammar_in[idx], datum, changed);
          break;
    /*
        case (RULE_TYPE_IGNORE):
          check_ignore(&(grammar_in[idx]), changed);
          break;
        case (RULE_TYPE_INDIRECT):
          check_indirect(&(grammar_in[idx]), changed);
          break;
        case (RULE_TYPE_MANY):
          check_many(&(grammar_in[idx]), changed);
          break;
        case (RULE_TYPE_LENGTH):
          check_length(&(grammar_in[idx]), changed);
          break;
    */
        case (RULE_TYPE_SEQUENCE):
          tmp_changed = false;
          check_continuation(grammar_in[idx], datum, &tmp_changed);
          if (!tmp_changed)
          {
            check_sequence(grammar_in[idx], datum, changed);
          }
          else
          {
            (*changed) = tmp_changed;
          }
          break;
        case (RULE_TYPE_TOKEN):
          check_token(grammar_in[idx], datum, changed);
          break;
        default:
          STACK_INVALIDATE();
          break;
      }
    }
  }

}


void parse( axi_stream_t& data_in,
            rule_t grammar_in[MAX_GRAMMAR_DEPTH],
            uint64_t final_flag,
            axi_valid_stream_t& valid_out )
{
#pragma HLS INTERFACE axis port=data_in
#pragma HLS INTERFACE bram port=grammar_in
#pragma HLS INTERFACE axis port=valid_out

  axi_bus_t axis_chunk;
  bool reading = true;
  bool changed = true;
  axi_valid_bus_t valid_flag;
  valid_flag.last = 1;
  valid_flag.keep = 0x1;
  valid_flag.data = true;

  STACK_RESET();
  while (reading)
  {
    axis_chunk = data_in.read();
    STACK_PUSH( 0x0, (uint8_t)(axis_chunk.data.to_int()));

    // As long as the stack is changing, keep checking
    changed = true;
    while (changed)
    {
      // Run rule checks
      grammar_check(grammar_in, &changed);
    }
    //CLEAR_STACK_TOP();

    // End of stream, so calculate and write valid flag
    if (axis_chunk.last == 1)
    {
      reading = false;
      if ( CHECK_TOKEN(final_flag) &&
           (GET_STACK_PTR() == 0x1) )
      {
        valid_flag.data = true;
      }
      else
      {
        valid_flag.data = false;
      }
    }
  }
  valid_out.write(valid_flag);
}



