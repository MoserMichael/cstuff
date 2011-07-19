

#-
# Test run may need the shared libraries produced by this build. so add them to shared library search path.
#-
ifeq "$(FULL_ROOT_DIR)" ""
LD_LIBRARY_PATH:=$(BIN_ROOT_DIR)/lib:$(BIN_ROOT_DIR)/test:$(LD_LIBRARY_PATH)
export LD_LIBRARY_PATH

PATH:=$(BIN_ROOT_DIR)/bin:$(BIN_ROOT_DIR)/test:$(TOPDIR):$(PATH)
export PATH

endif

ARFLAGS:=rv


#-
#  Always add debugging information.
#-
CFLAGS+=-g
CXXFLAGS+=-g


#-
# Macro makes name of a shared library.
#-
make_shared_lib_name=$(BIN_ROOT_DIR)/$(2)/lib$(1).so

#-
# Macro adds option for compiling shared library code
#-
add-solib-compiler-flags=CFLAGS+=-fPIC 

#-
# Macro installs a file.
#-
define install-a-file
  $(info installing $(1) to directory $(2))
  $(shell install -m 0755 -t $(2) $(1))
endef

define install-mkdir
  $$(if $$(subst $(1),,$$(wildcard $(1))),,$$(info Create installation directory $(1))$$(shell install -d $(1) -m 775))
endef

# flag for producing preprocessor output
PREPROCESSOR_FLAG=-E

# flag for producing assembly listing
ASM_LISTING_FLAG=-S

#-
#  Compile with draconic error level by default. Skipped if OPT_NOSTRICT environment is set.
#-
ifndef NOSTRICT
CFLAGS+=-Wall -Werror -Wextra
CXXFLAGS+=-Wall -Werror -Wextra
endif


#-
# Add optimization
#-
ifdef OPT
  ifeq "$(strip $(OPT))" "3"
    CFLAGS+=-O2
    CXXFLAGS+=-O2
  else
    ifeq "$(strip $(OPT))" "1"
      CFLAGS+=-O1
      CXXFLAGS+=-O1
    else
      CFLAGS+=-O2
      CXXFLAGS+=-O2
    endif
  endif
endif


#-
# Add architecture specific options
#-
ifeq "$(ARCH)" "i686"
  CFLAGS+=-m32
  CXXFLAGS+=-m32
  LDFLAGS+=-m32
else
  ifeq "$(ARCH)" "x86_64"
    CFLAGS+=-m64
    CXXFLAGS+=-m64
    LDFLAGS+=-m64
  else
    $(error architecture $(ARCH) is not supported on $(OS_TYPE) )
  endif
endif
