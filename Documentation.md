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

## Checkpoint 4: conan_run on debian and local development preset
I was able to run the conan_run on debian. I decided to move even more installations into conan. Ninja and cmake are now installed via conan. 
That created some hurdles, as now I am using the generators VirtualBuildEnv and VirtualRunEnv. I played around with them and as long as i extend the run_conan scripts with activates/deactivates, it should work. I look forward to test the pipeline on github actions soon.


## Checkpoint 5: Github Actions pipeline functional
This has been quite a journey. It is very rewarding to see the pipeline building on github. Yesterday I've been working quite unfocused. Most likely because I was spending the day outside in the heat. Today I reflected on my work and was able to explore the pitfalls that hindered me yesterday.

### Learnings
    - compiler.libcxx=libstdc++11 : I was not aware that the standard comes with libstdc++11. The wrong setting here was the main hurdle for the linux pipelines.
    - VirtualBuildEnv and VirtualRunEnv outdated: I was using them because i saw them in other conan files. I was thinking that it allows me more control over the environement during build and run. The current approach seems more modern and fits to the examples.
    - shared depedencies : It took me some experimentation to really enforce a setup where both main such as test are having all dependencies available. Now the full dependency tree is copied to the bin folder on windows. 
    - The general shift from conan, please do everything with environment to my cmake setting up the environment. This will be better because now developing locally is easier and the install steps should be more easier too.
    - creating a pipeline for github is very different then creating a pipeline for a runner, where you manually install everything. I'm looking forward to the interview to learn about how you are doing this.


## Checkpoint 6: install and linting
  - I put my focus towards delivering a installation which can run self contained. I was able to create this on windows and Ubuntu but only with the clang compiler. I did not find the reason for it. 
  ```
  CPack: - Install project: challenge []
CMake Error at /home/runner/work/ag_tech_challenge_rt/ag_tech_challenge_rt/build/Release/cmake_install.cmake:74 (file):
  file Could not resolve runtime dependencies:

    libboost_filesystem.so.1.91.0


CPack Error: Error when generating package: challenge
FAILED: [code=1] CMakeFiles/package.util 
cd /home/runner/work/ag_tech_challenge_rt/ag_tech_challenge_rt/build/Release && /usr/local/bin/cpack --config ./CPackConfig.cmake
``` 
 - At some point i shifted my attention towards being able to have artifacts on github. To set up the release staging I have not tested yet. And also to enhance the pipeline with quality checks. As requested we run clang-format and clang-tidy. These run in a open branch /linting . I did not merge this because to fix the pipeline i would need to format things. 
 - Task 5 asks for having precommit hooks, which is a bit alienating to me. Personally I would think that clang-format and clang-tidy are consumed by IDEs and ideally enforced by the CICD.
 - Rather import now is to create another plugin, to ideally find a fix for the gcc install issue. And demonstrate that fixing the linting allows for passing lint checks. 