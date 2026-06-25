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