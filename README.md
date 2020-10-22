# cpusm2
# cpusm
cpu statistics monitor for linux cli

cpusm is a cli-based system performance monitor for linux. Developed using C, cpusm imparts a relatively small system processing penalty, making it the ideal cli application for realtime performance monitoring.

![cpusm example 1](https://i.imgur.com/QjI1Mbv.gif "cpusm example 1")

cpusm captures and presents data for the following: cpu uttilization, core utilization, memory utilization, cpu frequency, core frequency, network utilization, system temperature, kernel performance, and more. Execution is multi-threaded with respect to data collection/analysis, statistics reporting, and program command/control. 


cpusm build instructions:

	git clone https://github.com/j2ckson/cpusm2.git
	cd ./cpusm
	make
	sudo make install
  
  cpusm usage:
  
  cpusm --help
  
  cpusm [option]<value>...[option]<value>      
_______________________________________________________________________________________________________________
command-line options [* default]             

-a || --core: cpusm core affinity            
-b || --core-opt: cpusm core optimization [<*0> kernel, <1> min.cpu, <2> max.cpu, <3> random] 
-c || --colour: stdout/terminal display colour [*0 - 7]
-d || --duration: cpusm execution duration [x.y seconds] 
-e || --slip: deferal interval for process intensive statistics acquisition [frequency, temperature, rtt] [slip>=1, *4]
-f || --file: <filename>: log destination    
-g || --coefficient: statistics sampling interval coefficient [g * t, *1]
-h || --help: application usage/execution assistance          
-i || --shortmode: cpusm start information mode [*0 - normal, 1 - abbreviated] 
-j || --step: cpusm statistics output step interval [1/j * r * u] 
-k || --mode: cpusm statistics output mode [*0 - uni, 1 - multi] 
-l || --log cpusm output mode [*0 - stdout/terminal, 1 - stdout/terminal+file, 2 - file] 
-m || --display-size: [cpusm statistics mode [*0 - normal, 1 - abbreviated] 
-n || --network: network interface to monitor [-k1 || -k0 -v1] [*interface bound to 0.0.0.0/0]
-o || --priority: cpusm application execution priority [*0 - 99]
-p || --timing: time tracking alogrithm [0* - dynamic, 1 - static]
-q || --precision: time tracking precision [0 - off, *1 - on]
-r || --rate: sample rate per second [1 - 1000, *10]
-s || --scrolling: output display mode [*0 - fixed-line, 1 - scrolling]
-t || --interval: sample interval [s.n seconds, *0.1, minimum == 0.001] 
-u || --rolling: cpusm statistics rolling average interval [*1 - 999] seconds 
-v || --category: cpusm statistics category 
        0: cpu.kernel mode                  
        1: network mode [use in conjunction with -n]
        2: cpu.core mode                    
        3: thermal mode                     
        *4: core.frequency mode             
-w || --scheduler: cpusm process scheduling algorithm [*0 - 6]
-x || --header-interval: periodic column heading display [*0 == off, *40 == runtime switch default]
-y || --nice: cpusm execution nice value [-20 to 20, *0] 
-z || --samples: total samples [>=0, *0 - no limit] 
-# || --ipaddr: destination ip.address for round.trip.time [rtt] network testing [*208.67.222.222]
_______________________________________________________________________________________________________________
runtime options                             

<h>: show runtime help                      
<a>: switch core affinity [base process only - established threads will not switch
<b>: switch 'count' statistic [running-samples*, remaining-samples]
<c>: switch stdout colour                   
<q>: quit cpusm                             
<j>: enable/disable statistics step display 
<k>: enable/disable variable colour-mode focus scanning 
     - <+>: increase focus scanning interval 
     - <->: descrease focus scanning interval
<l>: enable/disable fixed colour-mode focus scanning 
<m>: enable/switch colour-mode focus        
<n>: switch 'runtime' statistic [running-time*, remaining-time]
<p>: switch cpu utilization calculation     
     - pcpu: full process including all threads
     - pcpu.0: parent process               
     - pcpu.1: comand and control thread    
     - pcpu.2: data acquisition thread      
     - pcpu.3: network rtt thread           
<s>: display cpusm interim summary statistics 
<v>: switch terminal display width          
<x>: enable/disable periodic header printing 
<z>: restart cpusm                          
<=>: display interim cpusm min/max/avg statistics
</>: switch terminal scrolling mode [fixed-line/scrolling]
<.>: display column headings                
<`>: switch pcpu accounting [per-core/aggregate]
<,>: enable/diable stdout [file logging-mode]]
<1>: enable/disable precise time-tracking   
<SPACE>: switch display mode [cpu.kernel/cpu.core/core.frequency/thermal/network]
