SYSTEMC_DIR =/usr/local/systemc
PROJECT     = test
BUILDFLAGS  = -g3
CXX         = g++

SC_INC			:=	-I$(SYSTEMC_DIR)/include
SC_LIB			:=	-L$(SYSTEMC_DIR)/lib-linux64
INC				:=	$(SC_INC) 
LIB				:=	$(SC_LIB) -lsystemc
INC				+=	-I./include

OPT_FLAG		:=	-O2 -march=native -fomit-frame-pointer -pipe -fvisibility-inlines-hidden #-flto
MVP_FLAG		:=	-DSC_INCLUDE_DYNAMIC_PROCESSES
FUCKGCC_FLAG	:=	-fpermissive -Wno-write-strings -w
RELEASE_FLAG	:=	$(OPT_FLAG) $(MVP_FLAG) $(FUCKGCC_FLAG) $(COMMON_FLAG) -DRELEASE
CPPFLAG			+=	$(RELEASE_FLAG)


INCFLAGS    = -I. -I${SYSTEMC_DIR}/include
LDFLAGS     = -L${SYSTEMC_DIR}/lib-linux64 -lsystemc -lm
SRC_CPPHEAD = ahb_master_if ahb_slave_if dram fake_master top DMA
SRC_CPP     = 
HEADERS     =
MAIN        = tb.cpp
OBJECTS     = $(SRC_CPPHEAD:=.o) $(SRC_CPP:cpp=o)

EXE = $(PROJECT)

all: $(EXE)

$(EXE): $(MAIN) $(OBJECTS) $(HEADERS)
	@echo "$@ building..."
	$(CXX) $(CPPFLAG) $(INC) $(MAIN) $(OBJECTS) $(LIB) -o $@
	@echo ""
	@echo "$@ build done successfully..."
	@echo ""

%.o: %.cpp
	@echo '	CPP $<'
	@$(CXX) $(CPPFLAG) $(INC) -M -MF $(@:.o=.d) -MP -MT $@ $<
	@$(CXX) $(CPPFLAG) $(INC) $< -c -o $@

clean:
	rm -f $(EXE)
	rm -f *.o
	rm -f *.d

