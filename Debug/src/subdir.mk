################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
CPP_SRCS += \
../src/BoardTester.cpp 

C_SRCS += \
../src/uartprint.c 

COMPILER_OBJS += \
src/BoardTester.obj \
src/uartprint.obj 

CPP_DEPS += \
src/BoardTester.d 

C_DEPS += \
src/uartprint.d 

# Each subdirectory must supply rules for building sources it contributes
src/%.obj: ../src/%.cpp 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo src\cppDepSubCommand.tmp=
	@sed -e "s/^/ /" "src\cppDepSubCommand.tmp"
	ccrx -subcommand="src\cppDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" "$<"
	@echo src\cppSubCommand.tmp=
	@sed -e "s/^/ /" "src\cppSubCommand.tmp"
	ccrx -subcommand="src\cppSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

src/%.obj: ../src/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo src\cDepSubCommand.tmp=
	@sed -e "s/^/ /" "src\cDepSubCommand.tmp"
	ccrx -subcommand="src\cDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" "$<"
	@echo src\cSubCommand.tmp=
	@sed -e "s/^/ /" "src\cSubCommand.tmp"
	ccrx -subcommand="src\cSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

