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
