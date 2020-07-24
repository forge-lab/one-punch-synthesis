# one-punch-synthesis
Program Synthesis



**How to install Z3 for Windows**

Used on 64bit system with Visual Studio Community Edition 2019, and Z3 version 4.8.8 64bit
### Step 1:
Download Z3

### Step 2:
Create new project

### Step 3:
In Debug at the top toolbar, goto config properties (should be last option)

### Step 4:
At top of page, you should select x64 as Platform. Then click configuration manager. Here, select Active Solution Platform as x64 and for the project list, make sure the configuration is set to debug and the platform is in x64, and that the box below Build is ticked, and the box below deploy is unticked.

### Step 5:
In Configuartion properties, in debugging, double click environment and enter the following directory:PATH=%PATH%;C:\Users\[YourUsername]\Desktop\z3-4.8.8-x64-win\bin\

### Step 6:
In Linker, in Input, click additional dependencys and in the dropdown menu select edit. Here, past in the following directory:C:\Users\[YourUsername]\Desktop\z3-4.8.8-x64-win\bin\libz3.lib
 ve
### Step 7 (May not be applicable):
Open z3++.h, and there will be a line that includes z3.h. Change <z3.h> to "z3.h", replacing the <> with quotation marks.

### Step 8:
Run the sample code provided by Microsoft to ensure it is working correctly with no errors, and you should be good to go!




