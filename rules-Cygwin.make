
IS_CYGWIN:=1
CC:=gcc

#-
# Test run may need the shared libraries produced by this build. so add them to shared library search path.
#-
ifeq "$(FULL_ROOT_DIR)" ""

PATH:=$(BIN_ROOT_DIR)/lib:$(BIN_ROOT_DIR)/test:$(TOPDIR):$(PATH)

endif

ARFLAGS:=rv

#-
#  Always add debugging information.
#-
CFLAGS+=-g
CXXFLAGS+=-g

#-
# Make shared library name
#-
make_shared_lib_name=$(BIN_ROOT_DIR)/$(2)/lib$(1).dll

#-
#  Add options for shared libraries
#  with CYGWIN: don't add -fPIC to compiler options for shared library.
#- 
add-solib-compiler-flags=
add-solib-compiler-flags-cpp=



#-
# Install a file
#-
install-a-file=$(shell install -m 0755 -t $(2) $(1))

#-
# Install a directory
#-
define install-mkdir
  $$(if $$(subst $(1),,$$(wildcard $(1))),,$$(shell install -d $(1) -m 775))
endef


# flag for producing preprocessor output
PREPROCESSOR_FLAG=-E

# flag for producing assembly listing
ASM_LISTING_FLAG=-S



#-
# Add optimization
#-
ifeq "$(strip $(MODE))" "release"
    CFLAGS+=-O3
    CXXFLAGS+=-O3
endif

ifeq "$(strip $(MODE))" "release2"
    CFLAGS+=-O2
    CXXFLAGS+=-O2
endif

ifeq "$(strip $(MODE))" "releaseBuild"
    CFLAGS+=-O3 -Wall
    CXXFLAGS+=-O3 -Wall
    NOSTRICT:=1
endif


#-
#  Compile with draconic error level by default. Skipped if OPT_NOSTRICT environment is set.
#-
ifndef NOSTRICT
CFLAGS+=-Wall -Werror -Wextra
CXXFLAGS+=-Wall -Werror -Wextra
endif


#-
# Add architecture specific options
#-
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
    $(warning architecture $(ARCH) is not explicitly supported on $(OS_TYPE) )
  endif
endif
