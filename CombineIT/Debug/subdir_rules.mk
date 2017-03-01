################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
main.obj: ../main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/Program Files (x86)/TI/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --opt_for_speed=3 --include_path="C:/Program Files (x86)/TI/ccsv6/ccs_base/arm/include" --include_path="C:/Program Files (x86)/TI/ccsv6/driverlib/MSP432P4xx" --include_path="C:/Program Files (x86)/TI/ccsv6/ccs_base/arm/include/CMSIS" --include_path="C:/Program Files (x86)/TI/ccsv6/tools/compiler/arm_15.12.3.LTS/include" --advice:power="all" -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --diag_warning=225 --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="main.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

startup_msp432p401r_ccs.obj: ../startup_msp432p401r_ccs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/Program Files (x86)/TI/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --opt_for_speed=3 --include_path="C:/Program Files (x86)/TI/ccsv6/ccs_base/arm/include" --include_path="C:/Program Files (x86)/TI/ccsv6/driverlib/MSP432P4xx" --include_path="C:/Program Files (x86)/TI/ccsv6/ccs_base/arm/include/CMSIS" --include_path="C:/Program Files (x86)/TI/ccsv6/tools/compiler/arm_15.12.3.LTS/include" --advice:power="all" -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --diag_warning=225 --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="startup_msp432p401r_ccs.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

system_msp432p401r.obj: ../system_msp432p401r.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/Program Files (x86)/TI/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --opt_for_speed=3 --include_path="C:/Program Files (x86)/TI/ccsv6/ccs_base/arm/include" --include_path="C:/Program Files (x86)/TI/ccsv6/driverlib/MSP432P4xx" --include_path="C:/Program Files (x86)/TI/ccsv6/ccs_base/arm/include/CMSIS" --include_path="C:/Program Files (x86)/TI/ccsv6/tools/compiler/arm_15.12.3.LTS/include" --advice:power="all" -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --diag_warning=225 --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="system_msp432p401r.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


