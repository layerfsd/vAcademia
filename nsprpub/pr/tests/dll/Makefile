# Generated automatically from Makefile.in by configure.
# 
# ***** BEGIN LICENSE BLOCK *****
# Version: MPL 1.1/GPL 2.0/LGPL 2.1
#
# The contents of this file are subject to the Mozilla Public License Version
# 1.1 (the "License"); you may not use this file except in compliance with
# the License. You may obtain a copy of the License at
# http://www.mozilla.org/MPL/
#
# Software distributed under the License is distributed on an "AS IS" basis,
# WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
# for the specific language governing rights and limitations under the
# License.
#
# The Original Code is the Netscape Portable Runtime (NSPR).
#
# The Initial Developer of the Original Code is
# Netscape Communications Corporation.
# Portions created by the Initial Developer are Copyright (C) 1998-2000
# the Initial Developer. All Rights Reserved.
#
# Contributor(s):
#
# Alternatively, the contents of this file may be used under the terms of
# either the GNU General Public License Version 2 or later (the "GPL"), or
# the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
# in which case the provisions of the GPL or the LGPL are applicable instead
# of those above. If you wish to allow use of your version of this file only
# under the terms of either the GPL or the LGPL, and not to allow others to
# use your version of this file under the terms of the MPL, indicate your
# decision by deleting the provisions above and replace them with the notice
# and other provisions required by the GPL or the LGPL. If you do not delete
# the provisions above, a recipient may use your version of this file under
# the terms of any one of the MPL, the GPL or the LGPL.
#
# ***** END LICENSE BLOCK *****

#! gmake

MOD_DEPTH	= ../../..
topsrcdir	= ../../..
srcdir		= .

include $(MOD_DEPTH)/config/autoconf.mk

include $(topsrcdir)/config/config.mk

# Disable optimization of the nspr on SunOS4.1.3
ifeq ($(OS_ARCH),SunOS)
ifeq ($(OS_RELEASE),4.1.3_U1)
OPTIMIZER =
endif
endif

CSRCS = mygetval.c mysetval.c

INCLUDES = -I$(dist_includedir)

OBJS = $(OBJDIR)/mygetval.$(OBJ_SUFFIX) \
	$(OBJDIR)/mysetval.$(OBJ_SUFFIX)

ifeq ($(OS_TARGET), WIN16)
W16OBJS = $(subst $(space),$(comma)$(space),$(OBJS))
endif

ifeq ($(OS_ARCH), WINNT)
ifeq ($(OS_TARGET), WIN16)
# do nothing
else
RES=$(OBJDIR)/my.res
RESNAME=../../../pr/src/nspr.rc
endif
endif

ifeq (,$(filter-out WINNT OS2,$(OS_ARCH)))
IMPORT_LIBRARY	= $(OBJDIR)/my.$(LIB_SUFFIX)
SHARED_LIBRARY	= $(OBJDIR)/my.dll
ifeq ($(OS_ARCH), OS2)
MAPFILE		= $(OBJDIR)/my.def
GARBAGE		+= $(MAPFILE)
MKSHLIB		+= $(MAPFILE)
endif
TARGETS		= $(SHARED_LIBRARY) $(IMPORT_LIBRARY)
else
ifdef MKSHLIB
SHARED_LIBRARY	= $(OBJDIR)/libmy.$(DLL_SUFFIX)
endif
TARGETS		= $(SHARED_LIBRARY)
endif

#
# To create a loadable module on Darwin, we must override
# -dynamiclib with -bundle.
#
ifeq ($(OS_ARCH),Darwin)
DSO_LDOPTS = -bundle
endif

include $(topsrcdir)/config/rules.mk

ifeq ($(OS_TARGET), WIN16)
# Note: The Win16 target: my.dll requires these macros
# to be overridden to build the test .dll
# default values in win16...mk are for release targets.
#
OS_DLL_OPTION = NOCASEEXACT
OS_LIB_FLAGS = -irn
endif

ifdef SHARED_LIBRARY
export:: $(TARGETS)

clean::
	rm -rf $(TARGETS)
endif
