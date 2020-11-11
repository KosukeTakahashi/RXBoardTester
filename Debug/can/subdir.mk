################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
CPP_SRCS += \
../can/canwrapper.cpp 

COMPILER_OBJS += \
can/canwrapper.obj 

CPP_DEPS += \
can/canwrapper.d 

# Each subdirectory must supply rules for building sources it contributes
can/%.obj: ../can/%.cpp 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo can\cppDepSubCommand.tmp=
	@sed -e "s/^/ /" "can\cppDepSubCommand.tmp"
	ccrx -subcommand="can\cppDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" "$<"
	@echo can\cppSubCommand.tmp=
	@sed -e "s/^/ /" "can\cppSubCommand.tmp"
	ccrx -subcommand="can\cppSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

