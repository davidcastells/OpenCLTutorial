all: emulator
	
emulator:
	export CL_CONTEXT_EMULATOR_DEVICE_ALTERA=1
	aoc -march=emulator -g median.cl
	
early:
	aoc -c median.cl
	
submit:
	#aoc contrast.cl
	sshpass -p $(CAPAPH_PASSWORD) ssh -p $(CAPAPH_PORT) $(CAPAPH_USER)@$(CAPAPH_HOST) -C mkdir -p LAB3.1
	sshpass -p $(CAPAPH_PASSWORD) scp -P $(CAPAPH_PORT) cluster/*.sh $(CAPAPH_USER)@$(CAPAPH_HOST):/home/capaph/$(CAPAPH_USER)/LAB3.1/
	sshpass -p $(CAPAPH_PASSWORD) scp -P $(CAPAPH_PORT) median.cl $(CAPAPH_USER)@$(CAPAPH_HOST):/home/capaph/$(CAPAPH_USER)/LAB3.1/
	sshpass -p $(CAPAPH_PASSWORD) ssh -p $(CAPAPH_PORT) $(CAPAPH_USER)@$(CAPAPH_HOST) -C chmod +x LAB3.1/submit.sh
	sshpass -p $(CAPAPH_PASSWORD) ssh -p $(CAPAPH_PORT) $(CAPAPH_USER)@$(CAPAPH_HOST) -C chmod +x LAB3.1/status.sh
	sshpass -p $(CAPAPH_PASSWORD) ssh -p $(CAPAPH_PORT) $(CAPAPH_USER)@$(CAPAPH_HOST) -C LAB3.1/submit.sh
	
status:
	sshpass -p $(CAPAPH_PASSWORD) ssh -p $(CAPAPH_PORT) $(CAPAPH_USER)@$(CAPAPH_HOST) -C LAB3.1/status.sh
	
download:
	unset CL_CONTEXT_EMULATOR_DEVICE_ALTERA
	sshpass -p $(CAPAPH_PASSWORD) scp -P $(CAPAPH_PORT) $(CAPAPH_USER)@$(CAPAPH_HOST):/home/capaph/$(CAPAPH_USER)/LAB3.1/median.aocx .
	
deploy:
	sshpass -p $(CAPAPH_PASSWORD) ssh -p $(CAPAPH_PORT) $(CAPAPH_USER)@$(CAPAPH_FPGA_SYSTEM)
