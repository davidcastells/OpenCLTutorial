all: emulator
	
clean:
	rm doContrast.aoco
	rm doContrast.aoclib

doContrast.aoco:
	aoc -c doContrast.xml -o doContrast.aoco

doContrast.aoclib: doContrast.aoco
	aocl library create -o doContrast.aoclib doContrast.aoco

emulator: doContrast.aoclib
	export CL_CONTEXT_EMULATOR_DEVICE_ALTERA=1
	aoc -march=emulator -g contrast.cl -l doContrast.aoclib
	
early: doContrast.aoclib
	aoc -c contrast.cl -l doContrast.aoclib
	
submit:
	#aoc contrast.cl
	sshpass -p $(CAPAPH_PASSWORD) ssh -p $(CAPAPH_PORT) $(CAPAPH_USER)@$(CAPAPH_HOST) -C mkdir -p LAB5.1
	sshpass -p $(CAPAPH_PASSWORD) scp -P $(CAPAPH_PORT) cluster/*.sh $(CAPAPH_USER)@$(CAPAPH_HOST):/home/capaph/$(CAPAPH_USER)/LAB5.1/
	sshpass -p $(CAPAPH_PASSWORD) scp -P $(CAPAPH_PORT) contrast.cl $(CAPAPH_USER)@$(CAPAPH_HOST):/home/capaph/$(CAPAPH_USER)/LAB5.1/
	sshpass -p $(CAPAPH_PASSWORD) scp -P $(CAPAPH_PORT) doContrast.aoclib $(CAPAPH_USER)@$(CAPAPH_HOST):/home/capaph/$(CAPAPH_USER)/LAB5.1/
	sshpass -p $(CAPAPH_PASSWORD) ssh -p $(CAPAPH_PORT) $(CAPAPH_USER)@$(CAPAPH_HOST) -C chmod +x LAB5.1/submit.sh
	sshpass -p $(CAPAPH_PASSWORD) ssh -p $(CAPAPH_PORT) $(CAPAPH_USER)@$(CAPAPH_HOST) -C chmod +x LAB5.1/status.sh
	sshpass -p $(CAPAPH_PASSWORD) ssh -p $(CAPAPH_PORT) $(CAPAPH_USER)@$(CAPAPH_HOST) -C LAB5.1/submit.sh
	
status:
	sshpass -p $(CAPAPH_PASSWORD) ssh -p $(CAPAPH_PORT) $(CAPAPH_USER)@$(CAPAPH_HOST) -C LAB5.1/status.sh

download:
	unset CL_CONTEXT_EMULATOR_DEVICE_ALTERA
	sshpass -p $(CAPAPH_PASSWORD) scp -P $(CAPAPH_PORT) $(CAPAPH_USER)@$(CAPAPH_HOST):/home/capaph/$(CAPAPH_USER)/LAB5.1/contrast.aocx .
