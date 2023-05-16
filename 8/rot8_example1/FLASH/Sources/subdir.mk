################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/ISR.c" \
"../Sources/MCG.c" \
"../Sources/SIM.c" \
"../Sources/TPM.c" \
"../Sources/main.c" \
"../Sources/sa_mtb.c" \

C_SRCS += \
../Sources/ISR.c \
../Sources/MCG.c \
../Sources/SIM.c \
../Sources/TPM.c \
../Sources/main.c \
../Sources/sa_mtb.c \

OBJS += \
./Sources/ISR.o \
./Sources/MCG.o \
./Sources/SIM.o \
./Sources/TPM.o \
./Sources/main.o \
./Sources/sa_mtb.o \

C_DEPS += \
./Sources/ISR.d \
./Sources/MCG.d \
./Sources/SIM.d \
./Sources/TPM.d \
./Sources/main.d \
./Sources/sa_mtb.d \

OBJS_QUOTED += \
"./Sources/ISR.o" \
"./Sources/MCG.o" \
"./Sources/SIM.o" \
"./Sources/TPM.o" \
"./Sources/main.o" \
"./Sources/sa_mtb.o" \

C_DEPS_QUOTED += \
"./Sources/ISR.d" \
"./Sources/MCG.d" \
"./Sources/SIM.d" \
"./Sources/TPM.d" \
"./Sources/main.d" \
"./Sources/sa_mtb.d" \

OBJS_OS_FORMAT += \
./Sources/ISR.o \
./Sources/MCG.o \
./Sources/SIM.o \
./Sources/TPM.o \
./Sources/main.o \
./Sources/sa_mtb.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/ISR.o: ../Sources/ISR.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/ISR.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/ISR.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/MCG.o: ../Sources/MCG.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/MCG.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/MCG.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/SIM.o: ../Sources/SIM.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/SIM.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/SIM.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/TPM.o: ../Sources/TPM.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/TPM.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/TPM.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/main.o: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/main.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/main.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/sa_mtb.o: ../Sources/sa_mtb.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/sa_mtb.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/sa_mtb.o"
	@echo 'Finished building: $<'
	@echo ' '


