#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>devkitPPC")
endif

include $(DEVKITPPC)/wii_rules

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------
TARGET		:=	$(notdir $(CURDIR))
BUILD		:=	build
SOURCES		:=	source \
				source/cheats \
				source/data \
				source/gui \
				source/loader \
				source/channel \
				source/memory \
				source/menu \
				source/music \
				source/network \
				source/libs/libfat \
				source/libs/libntfs \
				source/libs/libwbfs
DATA		:=	data  
INCLUDES	:=	source \
				source/cheats \
				source/gui \
				source/loader \
				source/channel \
				source/memory \
				source/menu \
				source/music \
				source/network \
				source/libs \
				source/libs/libfat \
				source/libs/libntfs \
				source/libs/libwbfs
#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------

CFLAGS	 = -g -O2 -Wall -Wno-char-subscripts $(MACHDEP) $(INCLUDE) -DHAVE_CONFIG_H -DMAIN_IOS=249
CXXFLAGS = -g -O2 -Wall -Wno-char-subscripts -Wextra -Wno-multichar $(MACHDEP) $(INCLUDE) -DHAVE_CONFIG_H

LDFLAGS	 = -g $(MACHDEP) -Wl,-Map,$(notdir $@).map,--section-start,.init=0x80B00000,-wrap,malloc,-wrap,free,-wrap,memalign,-wrap,calloc,-wrap,realloc,-wrap,malloc_usable_size -T../rvl.ld

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
LIBS	:=  -ltremor -lfreetype -lwiiuse -lbte -lasnd -lpng -lz -logc -lm -lmad -ljpeg

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=	$(CURDIR) \
			$(PORTLIBS)

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/$(TARGET)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
					$(foreach dir,$(DATA),$(CURDIR)/$(dir))

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

#---------------------------------------------------------------------------------
# automatically build a list of object files for our project
#---------------------------------------------------------------------------------
SVNREV		:=	$(shell bash ./svnrev.sh)
CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
sFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.S)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
	export LD	:=	$(CC)
else
	export LD	:=	$(CXX)
endif

export OFILES	:=	$(addsuffix .o,$(BINFILES)) \
					$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) \
					$(sFILES:.s=.o) $(SFILES:.S=.o)

#---------------------------------------------------------------------------------
# build a list of include paths
#---------------------------------------------------------------------------------
export INCLUDE	:=	$(foreach dir,$(INCLUDES), -iquote $(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD) \
					-I$(LIBOGC_INC)

#---------------------------------------------------------------------------------
# build a list of library paths
#---------------------------------------------------------------------------------
export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib) \
					-L$(LIBOGC_LIB)

export OUTPUT	:=	$(CURDIR)/$(TARGET)
.PHONY: $(BUILD) clean

#---------------------------------------------------------------------------------
$(BUILD):
	@bash ./buildtype.sh
	@[ -d $@ ] || mkdir -p $@
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
ios222:
	@bash ./buildtype.sh 222
	@[ -d $(BUILD) ] || mkdir -p $(BUILD)
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile
	@cp $(OUTPUT).dol wii/apps/wiiflow/ios222/boot.dol
	@cp $(OUTPUT).elf wii/apps/wiiflow/ios222/boot.elf
	
#---------------------------------------------------------------------------------
ios223:
	@bash ./buildtype.sh 223
	@[ -d $(BUILD) ] || mkdir -p $(BUILD)
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
ios224:
	@bash ./buildtype.sh 224
	@[ -d $(BUILD) ] || mkdir -p $(BUILD)
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
ios249:
	@bash ./buildtype.sh 249
	@[ -d $(BUILD) ] || mkdir -p $(BUILD)
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile
	@cp $(OUTPUT).dol wii/apps/wiiflow/ios249/boot.dol
	@cp $(OUTPUT).elf wii/apps/wiiflow/ios249/boot.elf
	
#---------------------------------------------------------------------------------
ios250:
	@bash ./buildtype.sh 250
	@[ -d $(BUILD) ] || mkdir -p $(BUILD)
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
beta249:
	@bash ./buildtype.sh 249
	@[ -d $(BUILD) ] || mkdir -p $(BUILD)
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile
	@[ -d beta ] || mkdir -p beta
	@cp $(OUTPUT).dol beta/249-boot.dol
	@cp $(OUTPUT).elf beta/249-boot.elf
	@cp $(BUILD)/*.map beta/

#---------------------------------------------------------------------------------
beta250:
	@bash ./buildtype.sh 250
	@[ -d $(BUILD) ] || mkdir -p $(BUILD)
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile
	@[ -d beta ] || mkdir -p beta
	@cp $(OUTPUT).dol beta/250-boot.dol
	@cp $(OUTPUT).elf beta/250-boot.elf
	@cp $(BUILD)/*.map beta/

#---------------------------------------------------------------------------------
beta222:
	@bash ./buildtype.sh 222
	@[ -d $(BUILD) ] || mkdir -p $(BUILD)
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile
	@[ -d beta ] || mkdir -p beta
	@cp $(OUTPUT).dol beta/222-boot.dol
	@cp $(OUTPUT).elf beta/222-boot.elf
	@cp $(BUILD)/*.map beta/

#---------------------------------------------------------------------------------
beta223:
	@bash ./buildtype.sh 223
	@[ -d $(BUILD) ] || mkdir -p $(BUILD)
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile
	@[ -d beta ] || mkdir -p beta
	@cp $(OUTPUT).dol beta/223-boot.dol
	@cp $(OUTPUT).elf beta/223-boot.elf
	@cp $(BUILD)/*.map beta/

#---------------------------------------------------------------------------------
beta224:
	@bash ./buildtype.sh 224
	@[ -d $(BUILD) ] || mkdir -p $(BUILD)
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile
	@[ -d beta ] || mkdir -p beta
	@cp $(OUTPUT).dol beta/224-boot.dol
	@cp $(OUTPUT).elf beta/224-boot.elf
	@cp $(BUILD)/*.map beta/

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(OUTPUT).elf $(OUTPUT).dol

#---------------------------------------------------------------------------------
run:
	wiiload $(TARGET).dol


#---------------------------------------------------------------------------------
else

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(OUTPUT).dol: $(OUTPUT).elf
$(OUTPUT).elf: $(OFILES)

#---------------------------------------------------------------------------------
# This rule links in binary data with the .png extension
#---------------------------------------------------------------------------------
%.png.o	:	%.png
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@bin2s -a 32 $< | $(AS) -o $(@)

-include $(DEPENDS)

#---------------------------------------------------------------------------------
# This rule links in binary data with the .ttf extension
#---------------------------------------------------------------------------------
%.ttf.o	:	%.ttf
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@bin2s -a 32 $< | $(AS) -o $(@)

-include $(DEPENDS)

#---------------------------------------------------------------------------------
# This rule links in binary data with the .wav extension
#---------------------------------------------------------------------------------
%.wav.o	:	%.wav
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@bin2s -a 32 $< | $(AS) -o $(@)

-include $(DEPENDS)

#---------------------------------------------------------------------------------
# This rule links in binary data with the .bin extension
#---------------------------------------------------------------------------------
%.bin.o	:	%.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@bin2s -a 32 $< | $(AS) -o $(@)

-include $(DEPENDS)

#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------
