################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
CPP_SRCS += \
../cpwm/cpwm.cpp 

COMPILER_OBJS += \
cpwm/cpwm.obj 

CPP_DEPS += \
cpwm/cpwm.d 

# Each subdirectory must supply rules for building sources it contributes
cpwm/%.obj: ../cpwm/%.cpp 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo cpwm\cppDepSubCommand.tmp=
	@sed -e "s/^/ /" "cpwm\cppDepSubCommand.tmp"
	ccrx -subcommand="cpwm\cppDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" "$<"
	@echo cpwm\cppSubCommand.tmp=
	@sed -e "s/^/ /" "cpwm\cppSubCommand.tmp"
	ccrx -subcommand="cpwm\cppSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

