all: emulator
	
emulator:
	export CL_CONTEXT_EMULATOR_DEVICE_ALTERA=1
	aoc -march=emulator -g gamma.cl
	
early:
	aoc -c gamma.cl
	
submit:
	#aoc contrast.cl
	sshpass -p $(CAPAPH_PASSWORD) ssh -p $(CAPAPH_PORT) $(CAPAPH_USER)@$(CAPAPH_HOST) -C mkdir -p LAB2.2
	sshpass -p $(CAPAPH_PASSWORD) scp -P $(CAPAPH_PORT) cluster/*.sh $(CAPAPH_USER)@$(CAPAPH_HOST):/home/capaph/$(CAPAPH_USER)/LAB2.2/
	sshpass -p $(CAPAPH_PASSWORD) scp -P $(CAPAPH_PORT) gamma.cl $(CAPAPH_USER)@$(CAPAPH_HOST):/home/capaph/$(CAPAPH_USER)/LAB2.2/
	sshpass -p $(CAPAPH_PASSWORD) ssh -p $(CAPAPH_PORT) $(CAPAPH_USER)@$(CAPAPH_HOST) -C chmod +x LAB2.2/submit.sh
	sshpass -p $(CAPAPH_PASSWORD) ssh -p $(CAPAPH_PORT) $(CAPAPH_USER)@$(CAPAPH_HOST) -C chmod +x LAB2.2/status.sh
	sshpass -p $(CAPAPH_PASSWORD) ssh -p $(CAPAPH_PORT) $(CAPAPH_USER)@$(CAPAPH_HOST) -C LAB2.2/submit.sh
	
status:
	sshpass -p $(CAPAPH_PASSWORD) ssh -p $(CAPAPH_PORT) $(CAPAPH_USER)@$(CAPAPH_HOST) -C LAB2.2/status.sh
	
download:
	unset CL_CONTEXT_EMULATOR_DEVICE_ALTERA
	sshpass -p $(CAPAPH_PASSWORD) scp -P $(CAPAPH_PORT) $(CAPAPH_USER)@$(CAPAPH_HOST):/home/capaph/$(CAPAPH_USER)/LAB3.2/gamma.aocx .
	
deploy:
	sshpass -p $(CAPAPH_PASSWORD) ssh -p 4480 $(CAPAPH_USER)@mountain.cephis.uab.cat