MODULES = module/freertos
BOARD ?= edu-ciaa-nxp
MUJU ?= ./muju

include $(MUJU)/module/base/makefile

OUT_DIR = ./build
DOC_DIR = $(OUT_DIR)/doc

$(OUT_DIR):
	mkdir $(OUT_DIR)

$(DOC_DIR) : $(OUT_DIR)
	mkdir $(DOC_DIR)

doc: $(DOC_DIR)
	doxygen doxyfile