all: emulator
	
emulator:
	aoc -march=emulator -g contrast.cl
	
early:
	aoc -c contrast.cl
	
fpga:
	#aoc contrast.cl
	sshpass -p $(CAPAPH_PASSWORD) ssh -p $(CAPAPH_PORT) $(CAPAPH_USER)@$(CAPAPH_HOST) -C ls