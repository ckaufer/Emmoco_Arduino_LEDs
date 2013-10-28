OUTDIR = Output
EMBUILDER ?=
SCHEMAFILE = $(APPNAME).ems
MAIN = $(APPNAME)-Prog
OUTFILE = $(OUTDIR)/$(MAIN).out
HEXFILE = $(OUTDIR)/$(MAIN).hex
OBJECTS = $(OUTDIR)/$(MAIN).obj $(OUTDIR)/$(APPNAME).obj $(OUTDIR)/Hal.obj 

TOOLS = c:/progs/WinAVR-20100110
MCU = atmega328p
CC = $(TOOLS)/bin/avr-gcc
OBJCOPY = $(TOOLS)/bin/avr-objcopy
SIZE = $(TOOLS)/bin/avr-size
LOAD = c:/Emmoco/Em-Builder/eclipse/workspace/Platform-Arduino-Uno/Avr/avrdude -C $(PLATFORM)/Avr/avrdude.conf -c stk500v1 -p $(MCU) -P COM4
COPTS = -std=gnu99 -w -g -O3 -fpack-struct=1 -ffunction-sections -fdata-sections -c -mmcu=$(MCU)
CFLAGS = -I$(PLATFORM)/Hal -IEm -I$(TOOLS)/avr/include $(COPTS)
LFLAGS = -mmcu=$(MCU) --entry __init -u __init -nostdlib -Wl,-Map=$(OUTDIR)/$(MAIN).map,--gc-sections
RMFILES = *.out *.map *.bin *.obj

load: $(OUTFILE)
	$(OBJCOPY) -O ihex $(OUTFILE) $(HEXFILE)
	$(LOAD) -q -q -V -F -U flash:w:$(HEXFILE) 2>&1

build: $(OUTDIR) $(OUTFILE)

$(OUTFILE): $(OBJECTS)
	$(CC) $(LFLAGS) -o $@ $(PLATFORM)/Avr/crt0.o $^ -T $(PLATFORM)/Avr/lnk.cmd
	$(SIZE) $@

$(OUTDIR):
ifeq (,$(findstring Windows,$(OS)))
	mkdir $(OUTDIR)
else
	cmd /c mkdir $(OUTDIR)
endif

$(OUTDIR)/$(MAIN).obj: $(MAIN).c Em/$(APPNAME).c
	$(CC) $< -o $@ $(CFLAGS) 

$(OUTDIR)/$(APPNAME).obj: Em/$(APPNAME).c
	$(CC) $< -o $@ $(CFLAGS) 

Em/$(APPNAME).c: $(SCHEMAFILE)
ifneq (,$(EMBUILDER))
	$(EMBUILDER) -v --root=$(<D) --outdir=Em --jsondir=Em $<
else
	@echo terminating because of prior schema errors 1>&2
	@exit 1
endif

$(OUTDIR)/Hal.obj: $(PLATFORM)/Hal/Hal.c
	$(CC) $< -o $@ $(CFLAGS) 

local-clean:
ifeq (,$(findstring Windows,$(OS)))
	rm -rf $(OUTDIR)
else
ifneq (,$(wildcard $(OUTDIR)))
	cmd /c rmdir /q /s $(subst /,\,$(OUTDIR))
endif
endif

clean: local-clean
ifeq (,$(findstring Windows,$(OS)))
	rm -rf $(EM)
else
ifneq (,$(wildcard $(EM)))
	cmd /c rmdir /q /s $(subst /,\,$(EM))
endif
endif

out-check:
ifeq (,$(wildcard $(OUTFILE)))
	@echo error: $(OUTFILE): No such file or directory 1>&2
	@exit 1
endif

.PHONY: all load clean local-clean out-check
