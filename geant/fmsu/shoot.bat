Executable = /home/dilks/sims_mposka/geant/bin/Linux-g++/simutesting
Universe = vanilla
notification = never
getenv = True
+Experiment = "star"
+Job_Type = "cas"

Arguments = macros/macro_1.mac gFiles/gFile_1
Log = logfiles/Out1.log
Output = logfiles/Out1.out
Error = logfiles/Out1.err
Queue

Arguments = macros/macro_2.mac gFiles/gFile_2
Log = logfiles/Out2.log
Output = logfiles/Out2.out
Error = logfiles/Out2.err
Queue

Arguments = macros/macro_3.mac gFiles/gFile_3
Log = logfiles/Out3.log
Output = logfiles/Out3.out
Error = logfiles/Out3.err
Queue

Arguments = macros/macro_4.mac gFiles/gFile_4
Log = logfiles/Out4.log
Output = logfiles/Out4.out
Error = logfiles/Out4.err
Queue

Arguments = macros/macro_5.mac gFiles/gFile_5
Log = logfiles/Out5.log
Output = logfiles/Out5.out
Error = logfiles/Out5.err
Queue

