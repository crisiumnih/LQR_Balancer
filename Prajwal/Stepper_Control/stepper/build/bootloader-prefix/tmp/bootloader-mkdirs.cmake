# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Espressif/frameworks/esp-idf-v5.1.2/components/bootloader/subproject"
  "C:/Users/prajw/OneDrive/Desktop/Eklavya/Main/Prajwal/Stepper_Control/stepper/build/bootloader"
  "C:/Users/prajw/OneDrive/Desktop/Eklavya/Main/Prajwal/Stepper_Control/stepper/build/bootloader-prefix"
  "C:/Users/prajw/OneDrive/Desktop/Eklavya/Main/Prajwal/Stepper_Control/stepper/build/bootloader-prefix/tmp"
  "C:/Users/prajw/OneDrive/Desktop/Eklavya/Main/Prajwal/Stepper_Control/stepper/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Users/prajw/OneDrive/Desktop/Eklavya/Main/Prajwal/Stepper_Control/stepper/build/bootloader-prefix/src"
  "C:/Users/prajw/OneDrive/Desktop/Eklavya/Main/Prajwal/Stepper_Control/stepper/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/prajw/OneDrive/Desktop/Eklavya/Main/Prajwal/Stepper_Control/stepper/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/prajw/OneDrive/Desktop/Eklavya/Main/Prajwal/Stepper_Control/stepper/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
