################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
CPP_SRCS += \
../timer/timer.cpp 

COMPILER_OBJS += \
timer/timer.obj 

CPP_DEPS += \
timer/timer.d 

# Each subdirectory must supply rules for building sources it contributes
timer/%.obj: ../timer/%.cpp 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo timer\cppDepSubCommand.tmp=
	@sed -e "s/^/ /" "timer\cppDepSubCommand.tmp"
	ccrx -subcommand="timer\cppDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" "$<"
	@echo timer\cppSubCommand.tmp=
	@sed -e "s/^/ /" "timer\cppSubCommand.tmp"
	ccrx -subcommand="timer\cppSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

