## -------------------------------------------------------------------------------
# Copyright 2019 Websensing
#
# file      build_soln.tcl
# authors   Jim Brock
# date      14 Jan 2019
# copyright <COPYRIGHT GOES HERE>
#
# brief TODO
#


set PROJ_NAME  "hls_grammar_parser"
set TOP_NAME   "grammar_parser"
set CORE_NAME_BASE "Grammar Parser"
set SRCDIR     "src"
set RELEASEDIR "releases"
set SRC_FILES  {grammar.hpp json_grammar.hpp stack.hpp parser.hpp parser.cpp}
set TB_FILES   {json_test.cpp}
set PART_BUILD_LIST {}
set CUR_DIR [pwd]


proc checkPart { PART_NAMES ALLFLAG } {
  global PART_BUILD_LIST

  set PART_LIST {}
  set FID [open "parts.txt" r]
  set PART_TEXT [read ${FID}]
  close ${FID}

  set PART_LIST [split ${PART_TEXT} "\n"]

  if { ${ALLFLAG} == 1 } {
    set PART_BUILD_LIST ${PART_LIST}
  } else {
    for { set idx 0} { $idx < [llength ${PART_NAMES}] } { incr idx } {
      set CUR_PART [lindex ${PART_NAMES} $idx]
      set PART_FOUND [lsearch ${PART_LIST} ${CUR_PART}]
      if {${PART_FOUND} == -1} {
        puts "ERROR: the specified part name ${CUR_PART} was not found in the list of existing cores."
        quit
      } else {
        lappend PART_BUILD_LIST ${CUR_PART}
      }
    }
  }
}


proc addSources { } {
  global SRCDIR
  global SRC_FILES
  global TB_FILES

  foreach FILENAME $SRC_FILES {
    add_files [file join ${SRCDIR} ${FILENAME}] -cflags ""
  }

  foreach FILENAME $TB_FILES {
    add_files -tb [file join ${SRCDIR} ${FILENAME}] -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
  }
}


if { $argc < 4 } {
  puts "The build_soln.tcl script requires command line arguments."
  puts "Useage: build_soln.tcl <version> <exportflag> <keepflag> <part(s)|all>"
} else {
  # set default hw_server connection 
  set VERSION [lindex $argv 0]
  set EXPORT [lindex $argv 1]
  set KEEP [lindex $argv 2]

  set PARTS [lindex $argv 3]
  set ALLFLAG 0
  if {[string match ${PARTS} "all"]} {
    set ALLFLAG 1
  } elseif { $argc > 4 } {
    for { set idx 4 } { $idx < $argc} { incr idx } {
      lappend PARTS [lindex $argv $idx]
    }
  }

  checkPart ${PARTS} ${ALLFLAG}

  open_project ${PROJ_NAME}

  for { set idx 0 } { $idx < [llength ${PART_BUILD_LIST}] } { incr idx } {
    set CUR_PART [lindex ${PART_BUILD_LIST} $idx]
    regsub -all {(.)-} ${CUR_PART} {\1} SOLN
    set CORE_NAME ${CORE_NAME_BASE}
    set DISP_NAME ${TOP_NAME}
 
    # Open project and set top module
    set_top ${DISP_NAME}

    addSources

    open_solution ${SOLN}
    set_part ${CUR_PART}

    # Set parameters
    create_clock -period 8 -name default
    config_compile -no_signed_zeros=0 -unsafe_math_optimizations=0
    config_schedule -effort medium -enable_dsp_full_reg=0 -relax_ii_for_timing=0 -verbose=0
    config_bind -effort medium
    config_export -description ${CORE_NAME} -display_name ${DISP_NAME} -format ip_catalog -library "diode_plumbing" -rtl verilog -taxonomy "/diode_plumbing" -vendor "websensing.com" -version ${VERSION} -vivado_optimization_level 2 -vivado_phys_opt place -vivado_report_level 0
    set_clock_uncertainty 12.5%
  
    csim_design -O -clean
    csynth_design
    cosim_design -O

    if {${EXPORT} == 1} {
      export_design -description ${CORE_NAME} -display_name ${DISP_NAME} -format ip_catalog -library "diode_plumbing" -rtl verilog -taxonomy "/diode_plumbing" -vendor "websensing.com" -version ${VERSION}
      regsub -all {\.} $VERSION {_} VER_NAME
      file copy -force [file join ${PROJ_NAME} ${SOLN} "impl" "ip" "websensing_com_vin_${DISP_NAME}_${VER_NAME}.zip"] [file join ${RELEASEDIR} "websensing_com_vin_${DISP_NAME}_${VER_NAME}_${SOLN}.zip"]
    }

    close_solution
  }

  close_project

  if {${KEEP} != 1} {
    file delete -force -- ${PROJ_NAME}
  }
}

quit

