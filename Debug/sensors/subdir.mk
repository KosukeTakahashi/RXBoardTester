################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
CPP_SRCS += \
../sensors/sensors.cpp 

COMPILER_OBJS += \
sensors/sensors.obj 

CPP_DEPS += \
sensors/sensors.d 

# Each subdirectory must supply rules for building sources it contributes
sensors/%.obj: ../sensors/%.cpp 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo sensors\cppDepSubCommand.tmp=
	@sed -e "s/^/ /" "sensors\cppDepSubCommand.tmp"
	ccrx -subcommand="sensors\cppDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" "$<"
	@echo sensors\cppSubCommand.tmp=
	@sed -e "s/^/ /" "sensors\cppSubCommand.tmp"
	ccrx -subcommand="sensors\cppSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

