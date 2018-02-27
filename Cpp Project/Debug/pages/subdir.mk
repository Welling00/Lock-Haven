################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../pages/CPUInfoPage.cpp \
../pages/DirectoryPage.cpp \
../pages/ErrorPage.cpp \
../pages/IPage.cpp \
../pages/StatusPage.cpp 

OBJS += \
./pages/CPUInfoPage.o \
./pages/DirectoryPage.o \
./pages/ErrorPage.o \
./pages/IPage.o \
./pages/StatusPage.o 

CPP_DEPS += \
./pages/CPUInfoPage.d \
./pages/DirectoryPage.d \
./pages/ErrorPage.d \
./pages/IPage.d \
./pages/StatusPage.d 


# Each subdirectory must supply rules for building sources it contributes
pages/%.o: ../pages/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<" -std=gnu++14
	@echo 'Finished building: $<'
	@echo ' '


