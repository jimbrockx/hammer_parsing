

#pragma once

#ifndef PARSER_HPP
#define PARSER_HPP

#include <hls_stream.h>
#include "ap_axi_sdata.h"
#include "ap_int.h"
#include <inttypes.h>
#include "grammar.hpp"

#define AXI_DATA_WIDTH_BITS     8

/** \brief */
typedef ap_axis <AXI_DATA_WIDTH_BITS, 0, 0, 0> axi_bus_t;

/** \brief */
typedef hls::stream<axi_bus_t> axi_stream_t;



/** \brief */
typedef hls::stream<rule_t> axi_rule_stream_t;



/** \brief */
typedef ap_axis <1,0,0,0> axi_valid_bus_t;

/** \brief */
typedef hls::stream<axi_valid_bus_t> axi_valid_stream_t;


void grammar_parser( axi_stream_t& data_in,
                     rule_t* grammar_in,
                     uint64_t final_flag,
                     axi_valid_stream_t& valid_out );

#endif
