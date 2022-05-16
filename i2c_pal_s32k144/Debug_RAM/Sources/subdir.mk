################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/library.c" \
"../Sources/main.c" \

C_SRCS += \
../Sources/library.c \
../Sources/main.c \

OBJS_OS_FORMAT += \
./Sources/library.o \
./Sources/main.o \

C_DEPS_QUOTED += \
"./Sources/library.d" \
"./Sources/main.d" \

OBJS += \
./Sources/library.o \
./Sources/main.o \

OBJS_QUOTED += \
"./Sources/library.o" \
"./Sources/main.o" \

C_DEPS += \
./Sources/library.d \
./Sources/main.d \


# Each subdirectory must supply rules for building sources it contributes
Sources/library.o: ../Sources/library.c
	@echo 'Building file: $<'
	@echo 'Executing target #24 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/library.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "Sources/library.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/main.o: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #25 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/main.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "Sources/main.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


