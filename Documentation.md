# Documenting my work on the tech challenge project


## Checkpoint 1: Use Cmake build system
I reached my first mental checkpoint where i was able to create the CMakelists strcuture i had in mind.
Main is at root.
plugin in build as shared library in a cmake subdirectory.
Such as tests. 

Without AI assistance, I would have not created the cmake files that fast. I would have spent to much time on looking up the syntax. But I remember enough to know what I want. 

Interestingly we have in our current project a mix of loading shared libraries at runtime, but also linking them at compile time. That got me confused and I read about the difference. The later stages will tell if using BOOST_ALL_DYN_LINK is correct or not. I will find some time the next days to read more about it.

### Some thoughts to share early
    - i dont enjoy using clang format per precommit hock. I would have prevered to choose any, format everything. You clearly state not to modifiy the source. Let's see if there is enough time. 
    - I wonder if conan allows me to not need write out cmake presets.

### next steps
  - Conan 2.0


## Checkpoint 2: Conan 2.0
Yesterday I made some of the official tutorial to get used working with conan.
Today I created the conanfile.py. While it is loading boost, I do enjoy conan's python way. 
The conan run is creating quite some outputs I'll have to understand.
I also need to improve conan to only load boost logs, if that is possible.

### Important observation
    - I got warnings that this approach is using conan 1.X features. So this is not fully conan 2.X

### next steps 
    - fix the pipeline 
    - Verify build is running on my windows machine and WSL2 Linux debian + gcc
    - only build boost logs if possible
    - I will skip Task 3 and do Task 4 first. 


## Checkpoint 3: conan_run locally working and discovering and executing the tests
Today I was able to create the exe on windows fully automated with conan. The ctest is also running and discovering the tests.
I focused on the automation of the build and test process since i want to focus my attention to the runners and the CICD. The run_conan.bat is creating the first stage of the pipeline. 

### Tough learnings
   - For a while I ignored the fact that i used the microsoft cmake generator. This was quite costly in time because its not cross platform friendly. Looking back I moved into that direction, since conan tutorials use it too. Im happy that i was able switch to ninja.
   - There is also the confusion about conans generators. I was thinking that is a smart idea to use them for the project itself too. But of course, a cmake preset should be provided that allows normal development. 

### next steps
   - test conan on debian
   - setup a development, local friendly cmake preset
