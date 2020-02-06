all: emulator
	
emulator: 
	aoc -march=emulator -g channels.cl 
	
early:
	aoc -c channels.cl 
	
submit:
	#aoc contrast.cl
	sshpass -p $(CAPAPH_PASSWORD) ssh -p $(CAPAPH_PORT) $(CAPAPH_USER)@$(CAPAPH_HOST) -C mkdir -p LAB4.1
	sshpass -p $(CAPAPH_PASSWORD) scp -P $(CAPAPH_PORT) cluster/*.sh $(CAPAPH_USER)@$(CAPAPH_HOST):/home/capaph/$(CAPAPH_USER)/LAB4.1/
	sshpass -p $(CAPAPH_PASSWORD) scp -P $(CAPAPH_PORT) contrast.cl $(CAPAPH_USER)@$(CAPAPH_HOST):/home/capaph/$(CAPAPH_USER)/LAB4.1/
	sshpass -p $(CAPAPH_PASSWORD) ssh -p $(CAPAPH_PORT) $(CAPAPH_USER)@$(CAPAPH_HOST) -C chmod +x LAB4.1/submit.sh
	sshpass -p $(CAPAPH_PASSWORD) ssh -p $(CAPAPH_PORT) $(CAPAPH_USER)@$(CAPAPH_HOST) -C chmod +x LAB4.1/status.sh
	sshpass -p $(CAPAPH_PASSWORD) ssh -p $(CAPAPH_PORT) $(CAPAPH_USER)@$(CAPAPH_HOST) -C LAB4.1/submit.sh
	
status:
	sshpass -p $(CAPAPH_PASSWORD) ssh -p $(CAPAPH_PORT) $(CAPAPH_USER)@$(CAPAPH_HOST) -C LAB4.1/status.sh

download:
	sshpass -p $(CAPAPH_PASSWORD) scp -P $(CAPAPH_PORT) $(CAPAPH_USER)@$(CAPAPH_HOST):/home/capaph/$(CAPAPH_USER)/LAB4.1/channels.aocx .
