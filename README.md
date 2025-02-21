# projetPROG5
An ARM simulator

This is yet another ARM processor simulator
More precisely, this is an ARMv5 instruction set emulator. Thus, it is neither
cycle accurate, nor very efficient. Has been completed from a skeleton program designed by Guillaume Huard (http://polaris.imag.fr/guillaume.huard/).


The main program, arm_simulator acts as a gdb server, so a typical use session
would look like:
./arm_simulator
this starts the simulator which waits for two kinds of incoming connections:
- a connection from a gdb client
- a connection from the external command used to send irqs to the simulator
the simulator prints on the screen the ports numbers on which it listens for
these connections.

To use the simulator, you must connect to it a gdb client:
- arm-elf-gdb
- file Examples/foo (for instance)
- target remote localhost:<port number given by the simulator>
- load
- step
- break 8
- cont
- ... and so on

Debugging messages and traces outputed by the simulator can be chosen at
compile-time using compilation flags. Just comment the undesired flags settings
in the first lines of Makefile.am, then make clean && make.

The simulator sources are organized as follows (<- denotes dependences) :
- messages : debug and warning messages functions
      <- nothing
- memory : memory area management with byte/half/word accesses and per access choosable endianess
      <- nothing
- arm_constants : some definitions about arm execution modes
         <- nothing
- arm_core : arm state management (registers and memory). Provides access to proper registers and memory depending on cpsr content
        <- memory, trace, arm_constants
- trace : trace infrastructure for memory/registers accesses and processor state monitoring. Can be configured using compile-time flags
     <- arm_core
- arm_exception : arm exceptions raising module and exception vector provider
             <- arm_core
- arm_data_processing : specialized decoding functions for data processing instructions
                   <- messages, arm_core, arm_exception
- arm_load_store : specialized decoding functions for load and store instructions
              <- messages, arm_core, arm_exception
- arm_branch_other : specialized decoding functions for branch and other miscellaneous instructions
                <- arm_core, arm_exception
- arm_instruction : arm instruction execution. Does basic decoding (data_proc, load/store, branch, and so on) and call the matching specialized decoder
               <- arm_core, arm_exception, arm_data_processing, arm_load_store, arm_branch_other
- gdb_protocol : implementation of gdb remote protocol for arm processor
            <- messages, trace, arm_core, arm_instruction
- scanner : scanner for gdb packets
       <- gdb_protocol
- arm_simulator : main simulator that acts as a gdb server
             <- arm_core, memory, gdb_scanner, gdb_protocol
- send_irq : small command to send exception to a running simulator
        <- nothing
