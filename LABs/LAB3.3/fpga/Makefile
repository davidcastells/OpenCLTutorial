all: emulator
	
emulator:
	export CL_CONTEXT_EMULATOR_DEVICE_ALTERA=1
	aoc -march=emulator -g median.cl
	
early:
	aoc -c median.cl
	
fpga:
	aoc median.cl
	
submit:
	sshpass -p $(CAPAPH_PASSWORD) ssh -p $(CAPAPH_PORT) $(CAPAPH_USER)@$(CAPAPH_HOST) -C mkdir -p LAB3.3
	sshpass -p $(CAPAPH_PASSWORD) scp -P $(CAPAPH_PORT) cluster/*.sh $(CAPAPH_USER)@$(CAPAPH_HOST):/home/capaph/$(CAPAPH_USER)/LAB3.3/
	sshpass -p $(CAPAPH_PASSWORD) scp -P $(CAPAPH_PORT) median.cl $(CAPAPH_USER)@$(CAPAPH_HOST):/home/capaph/$(CAPAPH_USER)/LAB3.3/
	sshpass -p $(CAPAPH_PASSWORD) ssh -p $(CAPAPH_PORT) $(CAPAPH_USER)@$(CAPAPH_HOST) -C chmod +x LAB3.3/submit.sh
	sshpass -p $(CAPAPH_PASSWORD) ssh -p $(CAPAPH_PORT) $(CAPAPH_USER)@$(CAPAPH_HOST) -C chmod +x LAB3.3/status.sh
	sshpass -p $(CAPAPH_PASSWORD) ssh -p $(CAPAPH_PORT) $(CAPAPH_USER)@$(CAPAPH_HOST) -C LAB3.3/submit.sh
	
status:
	sshpass -p $(CAPAPH_PASSWORD) ssh -p $(CAPAPH_PORT) $(CAPAPH_USER)@$(CAPAPH_HOST) -C LAB3.3/status.sh

download:
	unset CL_CONTEXT_EMULATOR_DEVICE_ALTERA
	sshpass -p $(CAPAPH_PASSWORD) scp -P $(CAPAPH_PORT) $(CAPAPH_USER)@$(CAPAPH_HOST):/home/capaph/$(CAPAPH_USER)/LAB3.3/median.aocx .