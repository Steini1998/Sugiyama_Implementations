# Compiler and Linker, with Options (e.g. for "COMPILER.cpp")
CXX := cl
CXXFLAGS := -nologo -W3 -Zm300 -TP -EHsc -Ox -MT

LDFLAGS = -LIBPATH:$(LEDA_LIBRARY_DIR) -LIBPATH:$(SUGIYAMA_LIBRARY_DIR)

# Project-Directory-Structure
INCLUDE_DIR := incl
SOURCE_DIR := src
INTERMEDIATE_DIR := intermediates
BINARY_DIR := bin

# Relevant Source-Files
SOURCE_FILE_NAMES_STEPS := input.cpp cycle_breaking.cpp leveling.cpp remake_graph.cpp vertex_positioning.cpp crossing_reduction.cpp
SOURCE_FILE_NAMES_UIS := cmd_ui.cpp panel_ui.cpp

SOURCE_FILES_STEPS := $(addprefix $(SOURCE_DIR)/steps/, $(SOURCE_FILE_NAMES_STEPS))
SOURCE_FILES_UIS := $(addprefix $(SOURCE_DIR)/user_interfaces/, $(SOURCE_FILE_NAMES_UIS))

OBJECT_FILES_STEPS := $(addprefix $(INTERMEDIATE_DIR)/steps/, $(patsubst %.cpp, %.obj, $(SOURCE_FILE_NAMES_STEPS)))
OBJECT_FILES_UIS := $(addprefix $(INTERMEDIATE_DIR)/user_interfaces/, $(patsubst %.cpp, %.obj, $(SOURCE_FILE_NAMES_UIS)))

# Dependencies
SYSTEM_LIBS := user32.lib gdi32.lib msimg32.lib comdlg32.lib shell32.lib advapi32.lib wsock32.lib

LEDA_INCLUDE_DIR := ..\incl
LEDA_LIBRARY_DIR := ..\lib
LEDA_LIB := leda.lib

SUGIYAMA_INCLUDE_DIR := ..\architecture\incl
SUGIYAMA_LIBRARY_DIR := ..\architecture\bin
SUGIYAMA_LIB := sugiyama.lib

# Libraries to create
LIBRARY_NAME := sugi_impl
STATIC_LIBRARY := $(BINARY_DIR)/$(LIBRARY_NAME).lib
DYNAMIC_LIBRARY := $(BINARY_DIR)/$(LIBRARY_NAME).dll


.PHONY: all
all: $(STATIC_LIBRARY) $(DYNAMIC_LIBRARY)

$(STATIC_LIBRARY) $(DYNAMIC_LIBRARY): $(SUGIYAMA_LIBRARY_DIR)\$(SUGIYAMA_LIB)

# Static Library only contains this components binaries and not the system-libraries or leda-library
.PHONY: lib
lib: $(STATIC_LIBRARY)

$(STATIC_LIBRARY): override CPPFLAGS += -I $(LEDA_INCLUDE_DIR) -I $(SUGIYAMA_INCLUDE_DIR) -I $(INCLUDE_DIR)

$(STATIC_LIBRARY): $(OBJECT_FILES_STEPS) $(OBJECT_FILES_UIS)
	LIB.EXE /OUT:$@ $^
	

.PHONY: dll
dll: $(DYNAMIC_LIBRARY)

$(DYNAMIC_LIBRARY): $(OBJECT_FILES) $(OBJECT_FILES_UIS)
	LINK.EXE /DLL /OUT:$@ $(LDFLAGS) $(SYSTEM_LIBS) $(LEDA_LIB) $(SUGIYAMA_LIB) $^


$(INTERMEDIATE_DIR)/steps/%.obj: $(SOURCE_DIR)/steps/%.cpp
	$(COMPILE.cpp) $< -Fo$@


$(INTERMEDIATE_DIR)/user_interfaces/%.obj: $(SOURCE_DIR)/user_interfaces/%.cpp
	$(COMPILE.cpp) $< -Fo$@


.PHONY: clean
clean:
	rm -f $(BINARY_DIR)/*
	rm -f $(INTERMEDIATE_DIR)/steps/*
	rm -f $(INTERMEDIATE_DIR)/user_interfaces/*