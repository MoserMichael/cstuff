

#--------------------------
# Setup variables and stuff
#--------------------------


#-
# ROOT_DIR - where the rules.make file is (in most projects it is the top directory of the project; but this may be overriden by setting
# the root directory from the environment)
#-
ifeq "$(strip $(ROOT_DIR))" ""
ROOT_DIR=$(TOPDIR)
endif

ifeq "$(OS_TYPE)" ""
OS_TYPE:=$(shell uname -o | sed -e 's\#/\#-\#g' -e 's\#(\#-\#g' -e 's\#)\#-\#g' )
export OS_TYPE
endif

ifeq "$(OS_VERSION)" ""
OS_VERSION:=$(shell uname -r | sed -e 's\#/\#-\#g' -e 's\#(\#-\#g' -e 's\#)\#-\#g' )
export OS_VERSION
endif

ifeq "$(ARCH)" ""
ARCH=$(shell uname -m)
export ARCH
endif



#-
# Name of directory where built results are put in.
#-
PLATFORM_NAME:=$(OS_TYPE)-$(OS_VERSION)-$(ARCH)

#-
# Path of directory where built results are put in
#-
BIN_ROOT_DIR:=$(ROOT_DIR)/$(PLATFORM_NAME)
export BIN_ROOT_DIR

#
#  search path for dependent path
#-
DEPENDENCY_PATH+=$(BIN_ROOT_DIR)/lib

#-
# What is the current directory ?
# $(PWD) - can't use with cygwin, as PWD we get from the calling shell.
# $(shell pwd) - don't want to run another shell
# $(CURDIR) - built in of gmake. well enough. Not all versions have it though.
#-
CURWD=$(CURDIR)
ifeq "$(CURWD)" ""
CURWD=$(shell pwd)
endif 

#-
#  object directory. Each target will place objects into subdirectory of this one.
#-
OBJECT_DIR=$(BIN_ROOT_DIR)/obj$(CURWD)

# Uncomment in order to show generated rules.
#SHOW_RULES=1

# default installation prefix, set value if it is not defined.
DEFAULT_INSTALL_PREFIX?=/usr/local

###
# This is an obscure exercise: 
# Each target gets defined again, but in the way of doing
# so we copy its name into the OP global variable, when we have to recurse into a subdirectory, then
# the recursion rule knows, what the current target is, by looking at the OP variable, and so
# the current build target gets passed to the invoked makefile that sits in the subdirectory.
###
clean:OP:=clean
cleanall:OP:=cleanall
test:OP:=test
install:OP:=install

#-
# Include platform specific override script (file must not exist, is optional)
#-
-include $(ROOT_DIR)/rules-$(OS_TYPE).make
-include $(ROOT_DIR)/rules-$(OS_TYPE)-$(OS_VERSION).make
-include $(ROOT_DIR)/rules-$(OS_TYPE)-$(OS_VERSION)-$(ARCH).make
	

ifneq "$(FULL_ROOT_DIR)" ""
FULL_ROOT_DIR=$(abspath $(TOPDIR))
endif

#$(subst $FULL_ROOT_DIR,,$(abspath $(TOPDIR)))

#------------------
# Macro definitions
#------------------


# --- dependency creation ---

define make-depend

gcc   	 -MM            \
         -MF $3         \
         -MP            \
         -MT $2         \
         $(CFLAGS)      \
         $(CPPFLAGS)    \
	 -I. -I$(ROOT_DIR) \
         $1

endef


# --- check if directory exists, if not create it. 
# --- this trick does fewer shell invocations, shell is run only when the directory does not exist - that is important for cygwin.

define make_dir_iff
  $$(if $$(subst $(1),,$$(wildcard $(1))),,$$(shell mkdir -p $(1)))
endef


#---
# Show a message when starting to build a target.
#---
define display_build_banner
  $(info )
  $(info  ****)
  $(info  * Target:    $(1))
  $(info  * Type:      $(2))
  $(info  * Host:      os: $(OS_TYPE) os-version: $(OS_VERSION) architecture: $(ARCH))
  $(info  * Directory: $(3)) 
  $(info  ****)
endef




define PROGRAM_target_setup_template

#---
# Generated common rules $(1)
#---

# - check for valid project type
ifeq "$(call $(1)_TYPE)" "shlib"
else
  ifeq "$(strip $(call $(1)_TYPE))" "exe"
  else 
    ifeq "$(strip $(call $(1)_TYPE))" "lib"
    else
       $$(error the value of $(1)_TYPE most be one of the following shlib , exe, lib. Current value is $(call $(1)_TYPE) )
    endif
  endif
endif


# - creation of subdirectory for object files (each target has its own)
.PHONY: $(1)_objdir_create
$(1)_objdir_create :
	$$(eval $$(call make_dir_iff,$$(OBJECT_DIR)/$(1)))

# - define implicit rules that place targets into subdirectory per target.

$$(OBJECT_DIR)/$(1)/%.o: %.c
	$$(call make-depend,$$<,$$@,$$(subst .o,.d,$$@))
	$$(CC) $$(CPPFLAGS) $$(CFLAGS) $$($(1)_CFLAGS) $$(EXTERNAL_CFLAGS) -c $$(OUTPUT_OPTION) -I. -I$(ROOT_DIR) $$<
ifneq "$(strip $(CPP_LISTING))" ""
	$$(CC) $$(PREPROCESSOR_FLAG) $$(CPPFLAGS) $$(CFLAGS) $$($(1)_CFLAGS) $$(EXTERNAL_CFLAGS) -c -I. -I$(ROOT_DIR) $$< -o $$(addsuffix .$(1).i,$$(basename $$<)) 
endif
ifneq "$(strip $(ASM_LISTING))" ""
	$$(CC)  $$(ASM_LISTING_FLAG) $$(CPPFLAGS) $$(CFLAGS) $$($(1)_CFLAGS) $$(EXTERNAL_CFLAGS) -c -I. -I$(ROOT_DIR) $$< -o $$(addsuffix .$(1).S,$$(basename $$<))
endif


$$(OBJECT_DIR)/$(1)/%.o: %.cpp
	$$(call make-depend,$$<,$$@,$$(subst .o,.d,$$@))
	$$(CXX) $$(CPPFLAGS) $$(CXXFLAGS) $$($(1)_CFLAGS) $$($(1)_CXXFLAGS) $$(EXTERNAL_CFLAGS) $$(EXTERNAL_CXXFLAGS) -c $$(OUTPUT_OPTION) -I. -I$(ROOT_DIR) $$<
ifneq "$(strip $(CPP_LISTING))" ""
	$$(CXX) $$(PREPROCESSOR_FLAG) $$(CPPFLAGS) $$(CXXFLAGS) $$($(1)_CFLAGS) $$($(1)_CXXFLAGS) $$(EXTERNAL_CFLAGS) $$(EXTERNAL_CXXFLAGS) -c -I. -I$(ROOT_DIR) $$< -o $$(addsuffix .$(1).i,$$(basename $$<))
endif
ifneq "$(strip $(ASM_LISTING))" ""
	$$(CXX) $$(ASM_LISTING_FLAG) $$(CPPFLAGS) $$(CXXFLAGS) $$($(1)_CFLAGS) $$($(1)_CXXFLAGS) $$(EXTERNAL_CFLAGS) $$(EXTERNAL_CXXFLAGS) -c -I. -I$(ROOT_DIR) $$< -o $$(addsuffix .$(1).S,$$(basename $$<))
endif




# - list of source files -
ifneq "$(strip $(call $(1)_SRC))" ""
  sources_$(1)=$$($(1)_SRC)
else
  ifneq "$(strip $(call $(1)_SRC_EXTENSION))" ""
      sources_$(1)=$$(shell ls *.$(call $(1)_SRC_EXTENSION))
  else 
      $$(error Either list source files $(1)_SRC or specify source extension in $(1)_SRC_EXTENSION)
  endif
endif 


# - define prebuild target - executed before build. Prebuild step gets source file names as argument.

$(1)_pre_build :
ifneq "$$(strip $$($(1)_PREBUILD))" ""
	$$($(1)_PREBUILD) $$(sources_$(1))
endif

# - define postbuild target - executed before build. Prebuild step gets following arguments: path of resulting executable, source file names


$(1)_post_build :
ifneq "$$(strip $$($(1)_POSTBUILD))" ""
	$$($(1)_POSTBUILD) $$(PRJ_RESULT_FILE) $$(sources_$(1))
endif


objects_$(1):=$$(addprefix $$(BIN_ROOT_DIR)/obj$$(CURWD)/$(1)/, $$(addsuffix .o, $$(basename $$(sources_$(1)) ) ) )


-include $$(addprefix $$(BIN_ROOT_DIR)/obj$$(CURWD)/$(1)/, $$(addsuffix .d, $$(basename $$(sources_$(1)) ) ) )

.PHONY: banner_xxx_$(1)
banner_xxx_$(1) :
	$$(eval $$(call display_build_banner,$(1),$$(PRJ_TYPE_NAME),$$(CURWD)))


# rule tha cleans this target
$(1)__clean_xxx :
	rm -rf $$(OBJECT_DIR)/$(1)
	rm -f $$(PRJ_RESULT_FILE)


.DEFAULT: $(1)_INSTALL
$(1)_INSTALL:

endef




# --- define shared library rules ---

define PROGRAM_shared_library_template

#---
# Generated shared library rules $(1)
#---

.PHONY: build_xxx__$(1)
build_xxx__$(1) : PRJ_NAME:=$(1) 

build_xxx__$(1) : $(call add-solib-compiler-flags) 

build_xxx__$(1) : PRJ_TYPE_NAME:=Shared library

build_xxx__$(1) : PRJ_RESULT_FILE=$(call make_shared_lib_name,$(1),$(2))

build_xxx__$(1) : directory_setup $(1)_objdir_create banner_xxx_$(1)  $(call make_shared_lib_name,$(1),$(2))

 
$(call make_shared_lib_name,$(1),$(2)) : $$(objects_$(1)) 
	$$(eval $$(call display_build_banner,$(1),$$(PRJ_TYPE_NAME),$$(CURWD)))
	$(CC) -shared -Wl,-soname,lib$(1).so  -o $$@ $(LDFLAGS) $$(EXTERNAL_LDFLAGS) $$($(1)_LDFLAGS) $$(objects_$(1))  $$(addprefix -L,$$(DEPENDENCY_PATH)) $$(addprefix -l,$$($(1)_LIBS)) 

.PHONY: $(1)__clean_xxx
$(1)__clean_xxx : PRJ_RESULT_FILE=$(BIN_ROOT_DIR)/$(2)/lib$(1).so

.PHONY: $(1)__run_xxx
$(1)__run_xxx :

.PHONY: install_target_xx__$(1)
install_target_xx__$(1) : $($(1)_INSTALL)
	$$(eval $$(call install-a-file,$$(call make_shared_lib_name,$(1),$(2)),$$(PREFIX)/lib))
	
endef

$


# --- define static library rules ---

define PROGRAM_static_library_template

#---
# Generated static library $(1)
#---

.PHONY: build_xxx__$(1)
build_xxx__$(1) : PRJ_NAME:=$(1) 

build_xxx__$(1) : PRJ_TYPE_NAME:=Static library

build_xxx__$(1) : PRJ_RESULT_FILE = $(BIN_ROOT_DIR)/$(2)/lib$(1).a

build_xxx__$(1) : directory_setup $(1)_objdir_create banner_xxx_$(1) $(1)_pre_build $$(BIN_ROOT_DIR)/$(2)/lib$(1).a $(1)_post_build 

$(BIN_ROOT_DIR)/$(2)/lib$(1).a : $(objects_$(1)) 
	$$(eval $$(call display_build_banner,$(1),$$(PRJ_TYPE_NAME),$$(CURWD)))
	rm -f $(BIN_ROOT_DIR)/$(2)/lib$(1).a ; $(AR) $(ARFLAGS) $(BIN_ROOT_DIR)/$(2)/lib$(1).a $$(objects_$(1))  

.PHONY: $(1)__clean_xxx
$(1)__clean_xxx : PRJ_RESULT_FILE = $(BIN_ROOT_DIR)/$(2)/lib$(1).a

.PHONY: $(1)__run_xxx
$(1)__run_xxx :

.PHONY: install_target_xx__$(1)
install_target_xx__$(1) : $($(1)_INSTALL)
	$$(eval $$(call install-a-file,$$(BIN_ROOT_DIR)/$(2)/lib$(1).a,$$(PREFIX)/lib))

endef



# --- define executable library rules ---

define PROGRAM_exe_template


#---
# Generated executable rules $(1)
#---

# - rule for target -
.PHONY: build_xxx__$(1)
build_xxx__$(1) : PRJ_NAME:=$(1) 

build_xxx__$(1) : PRJ_TYPE_NAME:=Executable

build_xxx__$(1) : PRJ_RESULT_FILE=$(BIN_ROOT_DIR)/bin/$(1)

build_xxx__$(1) : directory_setup $(1)_objdir_create banner_xxx_$(1) $(1)_pre_build $(BIN_ROOT_DIR)/bin/$(1) $(1)_post_build  

$(BIN_ROOT_DIR)/bin/$(1) : $$(objects_$(1))  
	$(CC)  -o $(BIN_ROOT_DIR)/bin/$(1) $(LDFLAGS) $$($(1)_LDFLAGS) $$(EXTERNAL_LDFLAGS) $$(objects_$(1)) $$(addprefix -L,$$(DEPENDENCY_PATH)) $$(addprefix -l,$$($(1)_LIBS)) 

.PHONY: $(1)__clean_xxx
$(1)__clean_xxx : PRJ_RESULT_FILE=$(BIN_ROOT_DIR)/bin/$(1)

.PHONY: $(1)__run_xxx
$(1)__run_xxx :

.PHONY: install_target_xx__$(1)
install_target_xx__$(1) : $($(1)_INSTALL)
	$$(eval $$(call install-a-file,$$(BIN_ROOT_DIR)/bin/$(1),$$(PREFIX)/bin))


endef


# --- define executable library rules ---

define TEST_exe_template

#---
# Generated executable rules $(1)
#---

# - rule for target -
.PHONY: build_xxx__$(1)
build_xxx__$(1) : PRJ_NAME:=$(1) 

build_xxx__$(1) : PRJ_TYPE_NAME:=Executable test program

build_xxx__$(1) : DEPENDENCY_PATH+=$$(BIN_ROOT_DIR)/test

build_xxx__$(1) : PRJ_RESULT_FILE=$(BIN_ROOT_DIR)/test/$(1)

build_xxx__$(1) : $(1)_objdir_create directory_setup banner_xxx_$(1) $(1)_pre_build $(BIN_ROOT_DIR)/test/$(1) $(1)_post_build  

$(BIN_ROOT_DIR)/test/$(1) : $$(objects_$(1)) 
	$$(CC)  -o $$(BIN_ROOT_DIR)/test/$(1) $$(LDFLAGS) $$($(1)_LDFLAGS) $$(EXTERNAL_LDFLAGS) $$(objects_$(1)) $$(addprefix -L,$$(DEPENDENCY_PATH)) $$(addprefix -l,$$($(1)_LIBS))

                                                            
.PHONY: $(1)__clean_xxx
$(1)__clean_xxx : PRJ_RESULT_FILE=$(BIN_ROOT_DIR)/test/$(1)


.PHONY: $(1)__run_xxx
$(1)__run_xxx :
	$$(info )
	$$(info *** running test $(1) ***)
	$$(info )
	$$(TEST_TOOL) $(BIN_ROOT_DIR)/test/$(1) 	



endef



#----------------------
# Top level targets
#----------------------


# -- make build target --

.PHONY: all
all: build_pre_subdirs $(addprefix build_xxx__,$(TARGETS)) build_post_subdirs

ifneq "$(SHOW_RULES)" ""

$(foreach prog, $(TARGETS), $(info $(call PROGRAM_target_setup_template,$(prog))))

$(foreach prog, $(TARGETS), $(if $(subst shlib,,$($(prog)_TYPE)),,$(info $(call PROGRAM_shared_library_template,$(prog),lib))))

$(foreach prog, $(TARGETS), $(if $(subst lib,,$($(prog)_TYPE)),,$(info $(call PROGRAM_static_library_template,$(prog),lib))))

$(foreach prog, $(TARGETS), $(if $(subst exe,,$($(prog)_TYPE)),,$(info $(call PROGRAM_exe_template,$(prog)))))

endif


$(foreach prog, $(TARGETS), $(eval $(call PROGRAM_target_setup_template,$(prog))))

$(foreach prog, $(TARGETS), $(if $(subst shlib,,$($(prog)_TYPE)),,$(eval $(call PROGRAM_shared_library_template,$(prog),lib))))

$(foreach prog, $(TARGETS), $(if $(subst lib,,$($(prog)_TYPE)),,$(eval $(call PROGRAM_static_library_template,$(prog),lib))))

$(foreach prog, $(TARGETS), $(if $(subst exe,,$($(prog)_TYPE)),,$(eval $(call PROGRAM_exe_template,$(prog)))))

# -- make main target --

.PHONY: test
test: all build_pre_subdirs $(addprefix build_xxx__,$(TESTS)) $(addsuffix __run_xxx,$(TESTS)) build_post_subdirs 


ifneq "$(SHOW_RULES)" ""

$(foreach prog, $(TESTS), $(info $(call PROGRAM_target_setup_template,$(prog))))

$(foreach prog, $(TESTS), $(if $(subst shlib,,$($(prog)_TYPE)),,$(info $(call PROGRAM_shared_library_template,$(prog),test))))

$(foreach prog, $(TESTS), $(if $(subst lib,,$($(prog)_TYPE)),,$(info $(call PROGRAM_static_library_template,$(prog),test))))

$(foreach prog, $(TESTS), $(if $(subst exe,,$($(prog)_TYPE)),,$(info $(call TEST_exe_template,$(prog)))))

endif



$(foreach prog, $(TESTS), $(eval $(call PROGRAM_target_setup_template,$(prog))))

$(foreach prog, $(TESTS), $(if $(subst shlib,,$($(prog)_TYPE)),,$(eval $(call PROGRAM_shared_library_template,$(prog),test))))

$(foreach prog, $(TESTS), $(if $(subst lib,,$($(prog)_TYPE)),,$(eval $(call PROGRAM_static_library_template,$(prog),test))))

$(foreach prog, $(TESTS), $(if $(subst exe,,$($(prog)_TYPE)),,$(eval $(call TEST_exe_template,$(prog)))))

# -- make clean target --

.PHONY: clean
clean: build_pre_subdirs $(addsuffix __clean_xxx,$(TARGETS) $(TESTS)) build_post_subdirs  


# -- make install target --
.PHONY: install
install: PREFIX?=$(DEFAULT_INSTALL_PREFIX)

install: build_pre_subdirs $(addprefix install_target_xx__,$(TARGETS)) build_post_subdirs  
	$(eval $(call install-mkdir,$(PREFIX)/bin))
	$(eval $(call install-mkdir,$(PREFIX)/lib))
	$(eval $(call install-mkdir,$(PREFIX)/etc))

#----------------------
# common rules
#----------------------

# --- make root directories ---

.PHONY: directory_setup 
directory_setup :
	$(eval $(call make_dir_iff,$(BIN_ROOT_DIR)/lib))
	$(eval $(call make_dir_iff,$(BIN_ROOT_DIR)/bin))
	$(eval $(call make_dir_iff,$(BIN_ROOT_DIR)/test))

# --- recurse into subdirectory (prebuild) ---

.PHONY: build_pre_subdirs
build_pre_subdirs : $(addsuffix _build_pre_subdirs_dir, $(PREBUILD_SUBDIRS))


.PHONY: %_build_pre_subdirs_dir
%_build_pre_subdirs_dir :
	@cd $* ; $(MAKE) $(OP)


.PHONY: build_post_subdirs
build_post_subdirs : $(addsuffix _build_post_subdirs_dir, $(POSTBUILD_SUBDIRS))

.PHONY: %_build_post_subdirs_dir
%_build_post_subdirs_dir :
	@cd $* ; $(MAKE) $(OP)


.PHONY: cleanall
cleanall :
	rm -rf $(BIN_ROOT_DIR)

