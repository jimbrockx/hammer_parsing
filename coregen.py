#!/usr/bin/env python

## -------------------------------------------------------------------------------
# Copyright 2019 Websensing
#
# file      coregen.py
# authors   Jim Brock
# date      12 May 2020
# copyright <COPYRIGHT GOES HERE>
#
# brief TODO
#
#

import os, sys
import string
import subprocess
import datetime
import shutil
import re

# Debug variables
debug_flag = 1;

# Global variables
command = "";
author = "";
core_name = "grammar_parser";
parts = "";
part = "all";
soln = "";
revtype = "minor";
ip_dir = "";
cwd = os.getcwd();
release_dir = "releases";

# Usage
def printError( err_string ):
  """Print an error message, script usage, and then exit the program."""
  print("ERROR: ", err_string);
  print("Useage:");
  print("python3 coregen.py release <major|minor>");
  print("python3 coregen.py debug <part>")
  print("python3 coregen.py push <part> <ip_dir>");  
  quit();


def debugMsg( msg_string ):
  """Print a debug message if debugging is enabled"""
  global debug_flag;

  if (debug_flag == 1):
    print("DEBUG:" + msg_string);


def parseScriptArgs():
  """Parse the script command line arguments."""
  global command;
  global author;
  global part;
  global revtype;
  global ip_dir;

  if (len(sys.argv) < 2):
    printError("Incorrect number of command line arguments.");

  command = sys.argv[1];

  # BUILD CORE COMMAND
  if (command == 'release'):
    if (len(sys.argv) != 3):
      printError("Incorrect number of command line arguments for 'release' command.");

    revtype = sys.argv[2];
    if ((revtype != 'major') and (revtype != 'minor')):
      printError("Invalid entry for revision type. Please enter either 'major' or 'minor'.");

    debugMsg("ARG[1] : " + revtype);

  # DEBUG CORE COMMAND
  elif (command == 'debug'):
    if (len(sys.argv) != 3):
      printError("Incorrect number of command line arguments for 'debug' command.");

    part = sys.argv[2];
    debugMsg("ARG[1] : " + part);

  # PUSH CORE COMMAND
  elif (command == 'push'):
    if (len(sys.argv) != 4):
      printError("Incorrect number of command line arguments for 'push' command.");

    part = sys.argv[2];
    ip_dir = sys.argv[3];

    debugMsg("ARG[1] : " + part);
    debugMsg("ARG[2] : " + ip_dir);

  # INVALID CORE COMMAND
  else:
    printError("Invalid command: '" + command + "'' as first command line argument.");
  

def getParts():
  """ TBD """
  global cwd;
  global part;

  if (part == "all"):
    partsfile = os.path.join(cwd, "parts.txt");
    if (os.path.exists(partsfile)):
      fid = open(partsfile, "r");
      parts = fid.read();
      fid.close();
    parts = parts.replace("\n"," ");
  else:
    parts = part;

  return parts;


def processVersion( ):
  """ TBD """
  global cwd;
  global core_name;
  global release_dir;
  global soln;

  fname = "";
  oldver = "0.0";
  newver = "0.0";
  vpat = re.compile('_');

  # For each file in the release directory
  for file in os.listdir(os.path.join(cwd, release_dir)):
    pat = re.compile("websensing_com_vin_grammar_parser_([0-9]+_[0-9]+)_([a-zA-Z0-9]+).zip");
    mat = pat.match(file);

    # If this file is for the specified core
    if (mat != None):
      v_str = mat.group(2);
      # If the version string has not been set yet, set it
      if (oldver == "0.0"):
        oldver = v_str;
        vpat.sub('.', oldver);

      # Get the version number of this file, and compare it to the other
      # files for this core
      vpat.sub('.', v_str);
      if (oldver != v_str):
        printError("Invalid core version number found on file " + file);
  
      # Check the part of this file against the specified part number
      # If so, set the file name
      if ( (soln == mat.group(3)) ):
        fname = file;

  if (oldver == "0.0"):
    oldver = "0_0";

  ver, rev = oldver.split('_');
  oldver = ver + "." + rev;
  if (revtype == 'major'):
    ver = str(int(ver)+1);
    rev = str(0);
  elif (revtype == 'minor'):
    rev = str(int(rev)+1);
  else:
    printError("Should never get here! Invalid revision type in processRevision " + revtype);

  newver = ver + "." + rev;
  return oldver, newver, fname;


def removeOldVersionFiles( oldver ):
  """ TBD """
  global cwd;
  global core_name;
  global release_dir;

  fname = "";
  ver, rev = oldver.split('.');
  oldver_str = ver + "_" + rev;

  # For each file in the release directory
  for file in os.listdir(os.path.join(cwd, release_dir)):
    pat = re.compile("websensing_com_vin_grammar_parser_" + oldver_str + "_([a-zA-Z0-9]+).zip");
    mat = pat.match(file);
 
    # If this file is for the specified core
    if (mat != None):
      subprocess.call("rm " + os.path.join(cwd, release_dir, file), shell=True);


##############################
# Main Script entry point
##############################

parseScriptArgs();

soln = part.replace("-", "");
oldver, newver, fname = processVersion();
parts = getParts();

if (command == 'release'):
  # vivado_hls command: build_soln.tcl <core_name> <version> <exportflag> <keepflag> <parts>
  cmd = "vivado_hls " + os.path.join(cwd,"build_soln.tcl") + " " + newver + " 1 0 " + parts;
  debugMsg("cmd: " + cmd);
  if (subprocess.call(cmd, shell=True) != 0):
    quit();
  removeOldVersionFiles( oldver );
elif (command == 'debug'):
  # vivado_hls command: build_soln.tcl <core_name> <version> <exportflag> <keepflag> <parts>
  cmd = "vivado_hls " + os.path.join(cwd,"build_soln.tcl") + " " + oldver + " 0 1 " + parts; 
  debugMsg("cmd: " + cmd);
  subprocess.call(cmd, shell=True);
elif (command == 'push'):
  src_file = os.path.join(cwd, release_dir, fname);
  dst_file = os.path.join(ip_dir, fname);
  shutil.copyfile( src_file, dst_file );
  unzip_dir = os.path.join(ip_dir, core_name);
  unzip_cmd = "unzip -o -d " + unzip_dir + " " + dst_file;
  subprocess.call(unzip_cmd, shell=True);
  subprocess.call("rm -rf " + dst_file, shell=True);
else:
  printError("Should never get here! Invalid command after command processing!??!!?!?!?");  


# END OF SCRIPT
